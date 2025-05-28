/*

  Arduino MKR1010 Access point, 
  Send MPU-6050 sensor values to OSC UDP.

  A simple web server that lets you send OSC messages over a closed network.
  This sketch will create a new access point with password.
  It will then launch a new server, print out the IP address
  to the Serial monitor, and then send an OSC message over UDP to a specific IP address. 
  

  based on 
  WiFi Access Point for OSC Communication by Federico Peliti
  WiFi Simple Web Server by Tom Igoe
  WiFi UDP Send and Receive String by dlf
  UDP Send Message by Adrian Freed

  Tested with 
  Arduino MKR1010
  NINA Firmware 1.2.1

  Requires the following libraries:
  WiFi by Arduino
  WiFiNINA by Arduino
  OSC by Adrian Freed, Yotam Mann
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <OSCBundle.h> //Use OSCBundle for multi OSC messages names pack
#include <CapacitiveSensor.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_temp, *mpu_accel, *mpu_gyro;

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password

String readString;

//You can specify the Arduino's IP
//IPAddress arduIp(128, 32, 122, 252);
//destination IP
IPAddress outIp(192, 168, 4, 2);

//ports to listen for and send OSC messages
unsigned int localPort = 54321; //9999
const unsigned int outPort = 8007;

WiFiServer server(80);
WiFiUDP Udp;

//Declare Sensors
CapacitiveSensor   cs_7_8 = CapacitiveSensor(7,8); 
// 680k resistor between pins 7 & 8, pin 8 is sensor pin, add a wire and or foil if desired

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("OSC Access Point");

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

  // by default the local IP address of will be 192.168.4.2
  // you can override it with the following:
  // WiFi.config(arduIp);

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

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

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
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

  //Touch Proximity Sensor
  long start = millis();
  long touch =  cs_7_8.capacitiveSensor(30);

  Serial.print(millis() - start);        // check on performance in milliseconds
  Serial.print("\t");                    // tab character for debug windown spacing

  Serial.print(touch);                  // print sensor output 1
  Serial.print("\t");
  
  //declare the bundle
  OSCBundle bndl;

  //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
  bndl.add("/MugTouch").add((float)touch);// Capacitive Sensor
  bndl.add("/MugAccel").add((float)accel.acceleration.x).add((float)accel.acceleration.y).add((float)accel.acceleration.z).add((float)gyro.gyro.x).add((float)gyro.gyro.y).add((float)gyro.gyro.z).add((float)temp.temperature);//MPU6050 Accel
 
  Udp.beginPacket(outIp, outPort);
  bndl.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  bndl.empty(); // empty the bundle to free room for a new one

  delay(20);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

}
