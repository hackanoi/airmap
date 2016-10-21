#include "DHT.h"  
#include "SX1272.h"
#include <SPI.h>

int QT1 = A0; //Thiết đặt chân analog đọc quang trở
const int analogIn = A1;//gas
const int DHTPIN = 7;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
int e;

void setup() {
  Serial.begin(9600);
  dht.begin(); 
  Serial.println(F("SX1272 module and Arduino: send packets without Timeout"));
  
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
  e = sx1272.setNodeAddress(3);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.println(F("SX1272 successfully configured"));
  Serial.println();
}

void loop() {
  
  int value1 = analogRead(QT1);
  float h = dht.readHumidity();    //Đọc độ ẩm
  float t = dht.readTemperature(); //Đọc nhiệt độ
  float p = analogRead(A1); //Đọc nồng độ CO2

  String lightString ="tem="+ String(t) + "&humi=" + String(h) + "&light="+
                       String(value1) + "&c02=" + String(p);
  char message1[lightString.length()];
  lightString.toCharArray(message1, lightString.length());
  e = sx1272.sendPacketTimeout(0, message1);
  Serial.print(F("State:"));
  Serial.print(e, DEC);
  Serial.print("\t");
  Serial.println(message1);
  delay(4000);
  }

