#include <TinyGPS.h>
#include <SoftwareSerial.h>
#define TRUE 1
#define FALL 0
long previousMillis = 0;        // will store last time updateTemp was updated
long interval = 10000;           // interval at which to blink (milliseconds)
long previousMillis_send_again = 0;        // will store last time updateTemp was updated
long interval_send_again = 3000;           // interval at which to blink (milliseconds)

int check_send_ok = FALL;
String s;
int i = 0;
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

int limit;
int valueCo;
TinyGPS gps;
SoftwareSerial ss_lora(3, 4);
void setup()
{
  Serial.begin(115200);
  
  ss_lora.begin(115200);
  
  pinMode(ledPower,OUTPUT);// pm2.5
  pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
  pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
  Serial.println("at&v");
  ss_lora.println("at&v");
  delay(200);
  
  Serial.println("at+ack=6");
  ss_lora.println("at+ack=6");
  delay(200);
  
  Serial.println("at&w");
  ss_lora.println("at&w");
  delay(200);
  
  Serial.println("atz");
  ss_lora.println("atz");
  delay(2000);
  
  Serial.println("at&v");
  ss_lora.println("at&v");
  delay(200);
  
  Serial.println("at+join");
  ss_lora.println("at+join");
  delay(2000);
  
  Serial.println("SET_UP OK");
  
}

void loop()
{
  valueCo= (analogRead(AOUTpin));
  int valueSo2 = analogRead(AOSo2);
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
 
  voMeasured = analogRead(measurePin); // read the dust value
 
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (5.0 / 1024);// aduino uno is 5v, and aduino fio is 3.3

  dustDensity = 115+(0.17 * calcVoltage - 0.1)*1000;
  unsigned long currentMillis = millis();
  /*String s2 = "{\"d\":{\"node\":\"5\"," + "\"co\":\"" + String(valueCo)+
  "\",\"pm25\":\"" + String(dustDensity) +  "\",\"so2\":\"" +
  String(valueSo2) + "\"}}";*/

//  String s2 = "{\"d\":{\"node\":\"5\",\"co\":\""+ String(valueCo) + "\",\"pm25\":\""+ String(dustDensity)+  "\",\"so2\":\"" + String(valueSo2) + "\"}}";
 String s2 = "{\"d\":{\"node\":\"8\",\"co\":\""+ String(valueCo) + "\",\"pm25\":\""+
         String(dustDensity)+  "\",\"so2\":\"" + String(valueSo2)+
         "\",\"lat\":\"" + String(flat_String) +
         "\",\"lon\":\"" + String(flon_String)+
         "\",\"time\":\"" + String(timeDate) + "\"}}";
  // when you want to listen on a port, explicitly select it:
  if ((currentMillis - previousMillis > interval))
  {
    previousMillis = currentMillis;
    s = "at+send=" + s2 ;
    ss_lora.println(s);
    Serial.println(s);
    i++;
  }
 /* while(ss.available() > 0)
  {
    String inChar1 = ss.readString();
    if (ss.find("OK"))
    {
      check_send_ok = TRUE;
    }
  }
  unsigned long currentMillis_again = millis();
  if ((currentMillis_again - previousMillis_send_again > interval_send_again) & (check_send_ok == FALL))
  {
      previousMillis_send_again = currentMillis_again;
      ss.println(s);
  }*/
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss_gps.available())
      gps.encode(ss_gps.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
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
        month, day, year, hour + 7, minute, second);
    Serial.print(sz);
  }
//  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
