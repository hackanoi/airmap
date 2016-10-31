#include "SX1272.h"
#include <SPI.h>

// DUST PM2.5
//pin 1 connect Cap, Res into VCC = 5V
//pin 2 & 4 connect CAP then connect GND
//pin 3 connect pin D12
//pin 5 connect A0
//pin 6 coonect VCC = 5V

const int measurePin = 0;
const int ledPower = 9;
 
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
int limit;
int value;
int e;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(ledPower,OUTPUT);// pm2.5
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
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
  e = sx1272.setNodeAddress(3);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.println(F("SX1272 successfully configured"));
  Serial.println();
}

void loop() {
  
  value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
  Serial.print("CO value: ");
  Serial.print(value);//prints the CO value
  Serial.println(" ppm");
  // Read PM2.5
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024);// aduino uno is 5v, and aduino fio is 3.3

  dustDensity = (0.17 * calcVoltage - 0.1)*1000;
  Serial.print("Dust PM2.5: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3");
  String message = "\"co\":"+ String(value) + ",\"pm25\":"+
                      String(dustDensity)+  ",\"so2\":" + "50"; 
  char messageArray[message.length()+1];
  message.toCharArray(messageArray, message.length()+1);
  e = sx1272.sendPacketTimeout(0, messageArray);
  Serial.print(F("State:"));
  Serial.print(e, DEC);
  Serial.print("\t");
  Serial.println(message);
  delay(2000);
}

