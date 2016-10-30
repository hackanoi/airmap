// DUST PM2.5
//pin 1 connect Cap, Res into VCC = 5V
//pin 2 & 4 connect CAP then connect GND
//pin 3 connect pin D12
//pin 5 connect A0
//pin 6 coonect VCC = 5V

const int measurePin = 0;
const int ledPower = 12;
 
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);// pm2.5
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // DOC CO
  value= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
  Serial.print("CO value: ");
  Serial.print(value);//prints the CO value
  Serial.println(" ppm");
//  Serial.print("Limit: ");
//  Serial.println(limit);//prints the limit reached as either LOW or HIGH (above or underneath)

  // Read PM2.5
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
 
  // 0 - 3.3V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024);// aduino uno is 5v, and aduino fio is 3.3
 
  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = (0.17 * calcVoltage - 0.1)*1000;
 
//  Serial.print("Raw Signal Value (0-1023): ");
//  Serial.print(voMeasured);
 
//  Serial.print("Voltage: ");
//  Serial.print(calcVoltage);
 
  Serial.print("Dust PM2.5: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3");
  
  delay(2000);
  

  
}
