//Arduino MK1010 Wifi connection
//Send TF-LUNA and MPU6050 sensors values to OSC


#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <OSCBundle.h> //Use OSCBundle for multi OSC messages names pack
#include <SharpDistSensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;

//char ssid[] = "Studio_Msh";       //  your network SSID (name) between the " "
//char pass[] = "STUDIO@CICM";      // your network password between the " "
char ssid[] = "MemomentoNetwork";       //  your network SSID (name) between the " "
char pass[] = "MemomentoPass";      // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status

//You can specify the Arduino's IP
//IPAddress arduIp(128, 32, 122, 252);

//destination IP !!!
IPAddress outIp(192, 168, 4, 2);

//ports to listen for and send OSC messages
unsigned int localPort = 9993;
const unsigned int outPort = 8003;


WiFiServer server(80);            //server socket
WiFiUDP Udp;

//Declare this instances, not necessary
uint16_t count = 0;
uint16_t error = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("OSC WIFI Local Connexion");
    
  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();

  Serial.begin(115200);
  Serial1.begin(115200);

  //UDP starting
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort); 

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 Found!");
  mpu_temp = mpu.getTemperatureSensor();
  mpu_temp->printSensorDetails();

  mpu_accel = mpu.getAccelerometerSensor();
  mpu_accel->printSensorDetails();

  mpu_gyro = mpu.getGyroSensor();
  mpu_gyro->printSensorDetails();

}
  
void loop() {
  //TF-Luna
  uint16_t distance = readTFMini1();
  Serial.print(F("Time : "));
  Serial.println(millis());
  Serial.print(F("Count : "));
  Serial.println(count);
  Serial.print(F("Error : "));
  Serial.println(error);
  Serial.print(F("Distance : "));
  Serial.println(distance);
  Serial.println("");

  //MPU6050
   //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  mpu_temp->getEvent(&temp);
  mpu_accel->getEvent(&accel);
  mpu_gyro->getEvent(&gyro);

  Serial.print("\t\tTemperature ");
  Serial.print(temp.temperature);
  Serial.println(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" radians/s ");
  Serial.println();

  //declare the bundle
  OSCBundle bndl;

  //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
  bndl.add("/BeltLuna").add((float)distance);// Luna
  bndl.add("/BeltAccel").add((float)accel.acceleration.x).add((float)accel.acceleration.y).add((float)accel.acceleration.z).add((float)gyro.gyro.x).add((float)gyro.gyro.y).add((float)gyro.gyro.z).add((float)temp.temperature);//MPU6050 Accel
 
  Udp.beginPacket(outIp, outPort);
  bndl.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  bndl.empty(); // empty the bundle to free room for a new one
  delay(100);
}

uint16_t readTFMini1() {
 uint8_t current;
 static uint8_t n = 0;
 static uint8_t sum = 0;
 static uint16_t distance = 0;
 static uint16_t distanceOut = 0;
 
 while(Serial1.available()) {
  current = Serial1.read();

  switch(n) {

   case 0:
    if(current == 0x59) {
     sum += current;
     n++;
    }
    break;

   case 1:
    if(current == 0x59) {
     sum += current;
     n++;
    } else
     n = 0;
    break;

   case 2:
    distance = current;
    sum += current;
    n++;
    break;

   case 3:
    distance += current << 8;
    sum += current;
    n++;
    break;

   case 4:
    sum += current;
    n++;
    break;

   case 5:
    sum += current;
    n++;
    break;

   case 6:
    sum += current;
    n++;
    break;

   case 7:
    sum += current;
    n++;
    break;

   case 8:
    if(sum == current) {
     if(distance != 65535) {
      distanceOut = distance;
      // We get a new measure chose what you want to do here 
      count++; // I choose to increment a global value ... 
     }
    } else {
     // Error occured, choose what you want to do here ... 
      error++; // I choose to increment a global value ... 
    }
    sum = 0;
    n = 0;
    break;

  }
 }
 return distanceOut;  // Allways return the latest good value 
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void enable_WiFi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void connect_WiFi() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
}
