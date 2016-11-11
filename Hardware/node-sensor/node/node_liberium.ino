#include "SX1272.h"
#include <SPI.h>

long previousMillis = 0;        // will store last time updateTemp was updated
long interval = 40000;  
int i = 0;
// DUST PM2.5
//pin 1 connect Cap, Res into VCC = 5V
//pin 2 & 4 connect CAP then connect GND
//pin 3 connect pin D9
//pin 5 connect A0
//pin 6 coonect VCC = 5V

const int measurePin = 0;//A0
const int ledPower = 9;//D9
 
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
// Khoi tao gia tri bui PM2.5
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

// CO_Sensor
const int AOUTpin=1;//the AOUT pin of the CO sensor goes into analog pin A1 of the arduino
const int DOUTpin=8;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino
// Initialization CO value

//So2_Sénor
const int AOSo2 = 2;
const int DOSo2 = 7;
const int rsLora = 5;
int limit;
int value;
int e;

void setup() {
  pinMode(rsLora, OUTPUT);
  digitalWrite(rsLora, LOW);
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(ledPower,OUTPUT);// pm2.5 9
  pinMode(DOUTpin, INPUT);// 8sets the pin as an input to the arduino
  pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
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
  e = sx1272.setNodeAddress(10);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.println(F("SX1272 successfully configured"));
  Serial.println();
}

void loop() {
  value= (analogRead(AOUTpin));//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
  
  int valueSo2 = analogRead(AOSo2);
 
  // Read PM2.5
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024);// aduino uno is 5v, and aduino fio is 3.3

  dustDensity = 115+(0.17 * calcVoltage - 0.1)*1000;
String message = "\"node\":\"10\",\"co\":\""+ String(value) + "\",\"pm25\":\""+
         String(dustDensity)+  "\",\"so2\":\"" + String(valueSo2)+
         "\",\"lat\":\"" + "21.022345" +
         "\",\"lon\":\"" + "105.085234"+ "\"";
//String message = "node 1";

  char messageArray[message.length()+1];
  message.toCharArray(messageArray, message.length()+1);
  
  if(e ==1){
    digitalWrite(rsLora, HIGH);
    Serial.println("OK");
    digitalWrite(rsLora, LOW);
  }
  
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) > interval)
  {
    previousMillis = currentMillis;
    Serial.println(message);
    Serial.print(F("State:"));
    Serial.println(e, DEC);
    e = sx1272.sendPacketTimeout(0, messageArray);
    i++;
  }
}
