#include <stdio.h>
#include "DHT.h"
#include "LiquidCrystal_I2C.h"

#define DHTPIN 4
#define LIGHT_PIN 6
#define PUMP_PIN 5
#define LED_PIN 3

#define OFF_STATE HIGH
#define ON_STATE  LOW

#define DHTTYPE DHT11
DHT     dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

long    light_sensor;
long    moisture_sensor;
int     stateControl = 0;

int     rate = 50;
long    n;
int     temperature1;
int     humidity1;
char    str[50], stm[4];
String  data, data1;

void setup(){
  Serial3.begin(9600);
  Serial.begin(115200);
  dht.begin();

  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  digitalWrite(PUMP_PIN, OFF_STATE);
  digitalWrite(LIGHT_PIN,OFF_STATE);
  
  lcd.begin(16,2);              // initialize the lcd for 16 chars 2 lines, turn on backlight
  lcd.backlight();
  lcd.noBlink();                //stop blinking cursor*/
  lcd.setBacklight(HIGH);       // Backlight on
  lcd.clear();                  // clear display, set cursor position to zero
  lcd.setCursor (0,0);          //col 0 of line 1
  lcd.print(" HELLO!");
  lcd.print(" WELCOME");
  lcd.setCursor (0,1);          //col 0 of line 1
  lcd.print("  TO HACKANOI");
  data1 ="No";
  
  digitalWrite(LED_PIN,HIGH);
  delay(5000);
  digitalWrite(LED_PIN,LOW);
}

void loop(){
  if (Serial3.available() > 0) {
    data = Serial3.readString();
    data.trim();
    Serial.println(data);
    
    // Turn on water pump
    if( data.equals("PUMP ON"))
    {
      Serial.println("OK ON");
      digitalWrite(PUMP_PIN,ON_STATE);
      stateControl = 1;
    }

    // Turn off water pump
    if( data.equals("PUMP OFF"))
    {
      Serial.println("OK OFF");
      digitalWrite(PUMP_PIN,OFF_STATE);
      stateControl = 0;
    }

    // Notifi Error network
    if( data.equals("FALL"))
    {
      Serial.println("ERROR");
      digitalWrite(PUMP_PIN,OFF_STATE);
      digitalWrite(LED_PIN,LOW);
    }

    // Comfirm send data to cloud successful
    if( data.equals("OK"))
    {
      data1 ="Ok";
    }

    // change the water pump automatically
    // "RATE75" : automatic pump when below 75%
    if( data.substring(0,4) == "RATE" )
    {
      data.remove(0,4);
      rate = data.toInt();
      
      lcd.clear();              // clear display, set cursor position to zero
      lcd.setCursor (0,0);      // col 0 of line 1
      lcd.print("New Rate: ");  // print new rate receive from Cloud
      sprintf(stm,"%d",rate);
      lcd.print(stm);
      lcd.print("%");
      delay(3000);
    }
   }

  // Send data to cloud each 4 sec
  n++;
  if( n == 300000 ){
      n = 0;
      
      light_sensor    = (1024 - analogRead(A0))*100/1024;
      moisture_sensor = (1024 - analogRead(A1))*100/1024;

      humidity1     = (int)dht.readHumidity();
      temperature1  = (int)dht.readTemperature();
      
//      sprintf(str,"%d %d %d %d\n",temperature1,humidity1,moisture_sensor,light_sensor);
      sprintf(str,"\"object_temp\": \"%d\", \"humidity\": \"%d\", \"light\": \"%d\"",temperature1,humidity1,light_sensor);
      Serial3.print(str);
      Serial.print(str);

      // lcd.clear();              // clear display, set cursor position to zero
      // lcd.setCursor (0,0);      //col 0 of line 1
      // lcd.print("Moisture: ");
      // sprintf(stm,"%d",moisture_sensor);
      // lcd.print(stm);
      // lcd.print("%");
      

      lcd.clear();              // clear display, set cursor position to zero
      lcd.setCursor (0,0);      //col 0 of line 1
      lcd.print("Humidity: ");
      sprintf(stm,"%d",humidity1);
      lcd.print(stm);
      lcd.print("%");  

      lcd.setCursor (0,1);       // go col 0 of line 2
      sprintf(stm,"%d",temperature1);
      lcd.print(stm);
      lcd.write(0xDF);
      lcd.print("C - Data:");
      lcd.print(data1);
      data1 ="No";

      // Automatic control water pump
      if ( moisture_sensor > rate && stateControl == 0 )
      {
        digitalWrite(PUMP_PIN,OFF_STATE);
      }
      else
      {
        digitalWrite(PUMP_PIN,ON_STATE);
      }
  }
}



