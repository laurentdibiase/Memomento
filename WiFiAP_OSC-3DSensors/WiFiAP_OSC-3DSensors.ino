/*
  Arduino MKR1010 Access point
  Send 3D capacitive sensor values to OSC UDP

  A simple web server that lets you send OSC messages over a closed network.
  This sketch will create a new access point with password.
  It will then launch a new server, print out the IP address
  to the Serial monitor, and then send an OSC message over UDP to a specific IP address. 
  

  based on 
  WiFi Access Point for OSC Communication by Federico Peliti
  WiFi Simple Web Server by Tom Igoe
  WiFi UDP Send and Receive String by dlf
  UDP Send Message by Adrian Freed

  A simple web server that lets you send OSC messages over a closed network.
  This sketch will create a new access point with password.
  It will then launch a new server, print out the IP address
  to the Serial monitor, and then send an OSC message over UDP to a specific IP address. 


  based on 
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
#include <OSCMessage.h>
#include <CapacitiveSensor.h>

int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;     // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password

//You can specify the Arduino's IP
//IPAddress arduIp(128, 32, 122, 252);
//destination IP
IPAddress outIp(192, 168, 4, 2); //IP de Cristina 

//ports to listen for and send OSC messages
unsigned int localPort = 9995;
const unsigned int outPort = 8005;

WiFiServer server(80);
WiFiUDP Udp;

//Declare Sensors
CapacitiveSensor   cs_2_3 = CapacitiveSensor(2,3);        // 680k resistor between pins 2 & 3, pin 3 is sensor pin, add a wire and or foil if desired
CapacitiveSensor   cs_2_4 = CapacitiveSensor(2,4);        // 680k resistor between pins 2 & 4, pin 4 is sensor pin, add a wire and or foil
CapacitiveSensor   cs_2_5 = CapacitiveSensor(2,5);        // 680k resistor between pins 2 & 5, pin 5 is sensor pin, add a wire and or foil

void setup() {
  //Initialize serial and wait for port to open:
  // You can comment out the next 4 lines to use the Arduino without a USB cable
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

  // by default the local IP address of will be 192.168.4.1
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

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}


void loop() {
    long start = millis();
    long total1 =  cs_2_3.capacitiveSensor(30);
    long total2 =  cs_2_4.capacitiveSensor(30);
    long total3 =  cs_2_5.capacitiveSensor(30);
    
    Serial.print(millis() - start);        // check on performance in milliseconds
    Serial.print("\t");                    // tab character for debug windown spacing

    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.println(total3);                // print sensor output 3


  // Send OSC message
  OSCMessage msg("/Aquarium");
  //msg.add((float)q.w);
  msg.add((float)total1);
  msg.add((float)total2);
  msg.add((float)total3);
  
  Udp.beginPacket(outIp, outPort);
    msg.send(Udp); // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  msg.empty(); // free space occupied by message

  delay(200);
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
