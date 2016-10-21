#include "SX1272.h"
#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress server(31, 220, 16, 4);
char server[] = "lekienhust94.mybluemix.net";
IPAddress ip(192, 168, 1, 177);
EthernetClient client;
 
int e;
char my_packet[50];
 
void setup() {
   
  // start the serial library:
  Serial.begin(9600);
  // Print a start message
  Serial.println(F("SX1272 module and Arduino: receive packets without ACK"));

  // Power ON the module
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e = sx1272.setMode(4);
  Serial.print(F("Setting Mode: state "));
  Serial.println(e, DEC);
  
  // Set header
  e = sx1272.setHeaderON();
  Serial.print(F("Setting Header ON: state "));
  Serial.println(e, DEC);
  
  // Select frequency channel
  e = sx1272.setChannel(CH_10_868);
  Serial.print(F("Setting Channel: state "));
  Serial.println(e, DEC);
  
  // Set CRC
  e = sx1272.setCRC_ON();
  Serial.print(F("Setting CRC ON: state "));
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e = sx1272.setPower('M');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e = sx1272.setNodeAddress(1);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.println(F("SX1272 successfully configured"));
  Serial.println();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    // no point in carrying on, so do nothing forevermore:
//    for(;;)
//      ;
  Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println(F("connecting..."));
 
  // if you get a connection, report back via serial:
 
}
 
void loop(){

  e = sx1272.receivePacketTimeout(10000);
  if ( e == 0 ){
    for (unsigned int i = 0; i < sx1272.packet_received.length; i++){
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    if (client.connect(server, 80)) {
    Serial.println(F("connected"));
    // Make a HTTP request:
    client.print(F("GET /sendData.php?"));
    client.print(my_packet);
    client.println(F(" HTTP/1.0"));
    client.println(F("Host: lekienhust94.mybluemix.net"));
    client.println(F("Connection: close"));
    client.println(F("Content-Type: application/x-www-form-urlencoded"));

     
    //client.println("GET /ingenieros/comunicaciones/arduino/ethernet.php HTTP/1.0");
    client.println();
    }else {
      // kf you didn't get a connection to the server:
      Serial.println(F("connection failed"));
      }
  
      // if there are incoming bytes available 
      // from the server, read them and print them:
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
 
      // if the server's disconnected, stop the client:
      if (!client.connected()) {
        Serial.println();
        Serial.println(F("disconnecting."));
        client.stop();
      }
  } else {
    Serial.print(F(" State \t"));
    Serial.println(e, DEC);
  }
}
