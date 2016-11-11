#include "SX1272.h"
#include <SPI.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial ss(6,4);
static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

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
  
  ss.begin(9600);
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
  e = sx1272.setNodeAddress(3);
  Serial.print(F("Setting node address: state "));
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.println(F("SX1272 successfully configured"));
  Serial.println();
}

void loop() {
  float flat , flon ;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  int year;
  byte month, day, hour, minute, second, hundredths;
  char dateGPS[10];
  char timeDate[8];
  
  gps.f_get_position(&flat, &flon, &age);
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  sprintf(dateGPS, "%02d/%02d/%02d",month, day, year);// add  date
  sprintf(timeDate, "%02d:%02d:%02d ",hour, minute, second);// add 
  String flat_String = String(flat, 6);
  String flon_String = String(flon, 6);
  
  Serial.println(flat_String);
  Serial.println(flon_String);
//  Serial.println(latitude);
//  Serial.println(longitude);
  Serial.println(dateGPS);
//  print_date(gps);
  gps.stats(&chars, &sentences, &failed);
  smartdelay(1000);
  
  value= (analogRead(AOUTpin));//reads the analaog value from the CO sensor's AOUT pin
  limit= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
  Serial.print("CO value: ");
  Serial.print(value);//prints the CO value
  Serial.println(" ppm");
  
  int valueSo2 = analogRead(AOSo2);
  Serial.print("So2 value: ");
  Serial.print(valueSo2);//prints the CO value
  Serial.println(" ppb");
  
 
  // Read PM2.5
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024);// aduino uno is 5v, and aduino fio is 3.3

  dustDensity = 115+(0.17 * calcVoltage - 0.1)*1000;
  Serial.print("Dust PM2.5: ");
  Serial.print(dustDensity);
  Serial.println(" ug/m3");
//  String message = "\"CO\":\""+ String(value) + "\",\"Dust\":\""+
//         String(dustDensity)+  "\",\"SO2\":\"" + String(valueSo2) + 
//      "\",\"Latitude\":\"" + String(flat_String) + "\",\"Longitude\":\"" +
//      String(flon_String)+"\"" + ",\"date\":\"" + String(dateGPS) +
//      ",\"time\":\"" + String(timeDate) + "\"";
//Serial.println(message);
String message = "\"node\":\"1\",\"CO\":\""+ String(value) + "\",\"Dust\":\""+
         String(dustDensity)+  "\",\"SO2\":\"" + String(valueSo2)+
         "\",\"Lat\":\"" + String(flat_String) +
         "\",\"Lon\":\"" + String(flon_String)+
         "\",\"time\":\"" + String(timeDate)+ "\"";
Serial.println(message);
  char messageArray[message.length()+1];
  message.toCharArray(messageArray, message.length()+1);
  e = sx1272.sendPacketTimeout(0, messageArray);
  if(e ==1){
    digitalWrite(rsLora, HIGH);
    Serial.println("OK");
    digitalWrite(rsLora, LOW);
  }
  Serial.print(F("State:"));
  Serial.println(e, DEC);
//  Serial.print("\t");
//  Serial.println(message);
  delay(5000);
}
static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
//      Serial.print('1');
    Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}

