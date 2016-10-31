
// Include the SX1272 and SPI library:
#include "SX1272.h"
#include <SPI.h>
int e;
char my_packet[100];
String data;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial3.begin(9600);
  Serial.begin(115200);

  // Print a start message
  Serial.println(F("SX1272 module and Arduino: receive packets without ACK"));

  // Power ON the module
  e = sx1272.ON();
  Serial.print(F("Setting power ON: state "));
  Serial.println(e, DEC);
  
  // Set transmission mode and print the result
  e = sx1272.setMode(1);
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
  e = sx1272.setPower('L');
  Serial.print(F("Setting Power: state "));
  Serial.println(e, DEC);
  
  // Set the node address and print the result
  e = sx1272.setNodeAddress(1);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.println(F("SX1272 successfully configured"));
  Serial.println();
}

void loop(void){

  if(Serial3.available() >0){
    data = Serial3.readString();
    data.trim();
    Serial.println(data);
  }

  e = sx1272.receivePacketTimeout(10000);
  if ( e == 0 )
  {
    Serial.print(F(" State \t"));
    Serial.print(e, DEC);
    Serial.print("\t");

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      my_packet[i] = (char)sx1272.packet_received.data[i];
    }
    Serial3.println(my_packet);
    Serial.println(my_packet);
  } else {
    Serial.print(F(" State \t"));
    Serial.println(e, DEC);
  }
}

