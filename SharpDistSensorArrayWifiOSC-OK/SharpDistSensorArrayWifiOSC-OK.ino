//MK1010 local Wifi connect
//Send 2 sensors SHARP IR values to OSC


#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <SharpDistSensor.h>


//char ssid[] = "";       //  your network SSID (name) between the " "
//char pass[] = "";      // your network password between the " "
char ssid[] = "";       //  your network SSID (name) between the " "
char pass[] = "";      // your network password between the " "
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;      //connection status

//You can specify the Arduino's IP
//IPAddress arduIp(128, 32, 122, 252);

//destination IP !!!
IPAddress outIp(192, 168, 4, 2);  //IP adress


//ports to listen for and send OSC messages
unsigned int localPort = 9992;
const unsigned int outPort = 8002;


WiFiServer server(80);            //server socket
WiFiUDP Udp;

// Define the number of sensors in the array as a constant
const byte nbSensors = 2;

// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

// Define the array of SharpDistSensor objects
SharpDistSensor sensorArray[] = {
  SharpDistSensor(A0, medianFilterWindowSize), // First sensor using pin A0
  SharpDistSensor(A2, medianFilterWindowSize), // Second sensor using pin A2
  // Add as many sensors as required
};

// Alternatively, the array can be defined this way
// SharpDistSensor sensorArray[nbSensors] = {{A1, medianFilterWindowSize},
//                                           {A2, medianFilterWindowSize}};

// Define an array of integers that will store the measured distances
uint16_t distArray[nbSensors];

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("OSC WIFI Local Connexion");
    
  enable_WiFi();
  connect_WiFi();

  server.begin();
  printWifiStatus();

   // Set some parameters for each sensor in array of SHARP GP2Y0A41SK0F
  for (byte i = 0; i < nbSensors; i++) {
    sensorArray[i].setModel(SharpDistSensor:: GP2Y0A41SK0F_5V_DS);  // Set sensor model
    // Set other parameters as required
  }

   //UDP starting
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort); 
}

void loop() {
    // Read distance for each sensor in array into an array of distances
  for (byte i = 0; i < nbSensors; i++) {
    distArray[i] = sensorArray[i].getDist();
  }

  // The measured distances can now be accessed through the different indices of the
  // distArray array, i.e. distArray[0] for the first sensor, distArray[1] for the
  // sencond sensor ...
  
  // Print distance to Serial
  Serial.print("IR-1:");
  Serial.println(distArray[0]);
  Serial.print("IR-2:");
  Serial.println(distArray[1]);

  // Send OSC message
  OSCMessage msg("/Plateau");
  //msg.add((float)q.w);
  msg.add((byte)distArray[0]);
  msg.add((byte)distArray[1]);
  
  Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

  // Wait some time
  delay(10);
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
