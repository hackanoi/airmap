/**
  IBM IoT Foundation managed Device

  Author: Ant Elder
  License: Apache License v2
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient/releases/tag/v2.3
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson/releases/tag/v5.0.7

//-------- Customise these values -----------
//const char* ssid            = "";
//const char* password        = "";

const char* ssid            = "SPARC LAB";
const char* password        = "labthaydung";

//#define ORG "pr1qjl"
//#define DEVICE_TYPE "ESP8266" //"raspberrypi"
//#define DEVICE_ID "5CCF7F1179AF" //"b827ebd5250c"
//#define TOKEN "QT-YZqrb4&sa4p1byT"

 #define ORG "s2vwfm"
 #define DEVICE_TYPE "arduino" //"raspberrypi"
 #define DEVICE_ID "5CCF7F1179AF" //"b827ebd5250c"
 #define TOKEN "Kz1L+Y-4mCFa(mbHfE"


//-------- Customise the above values --------

char server[]               = ORG ".messaging.internetofthings.ibmcloud.com";
char authMethod[]           = "use-token-auth";
char token[]                = TOKEN;
char clientId[]             = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;

const char publishTopic[]   = "iot-2/evt/status/fmt/json";
const char responseTopic[]  = "iotdm-1/response";
const char manageTopic[]    = "iotdevice-1/mgmt/manage";
const char updateTopic[]    = "iotdm-1/device/update";
const char rebootTopic[]    = "iotdm-1/mgmt/initiate/device/reboot";
const char topic[]          = "iot-2/evt/status/fmt/json";
const char subtopic[]       = "iot-2/cmd/fly/fmt/json";
char* dataReceive;

WiFiClient wifiClient;

int publishInterval         = 20000; // 20 seconds
long lastPublishMillis;

String payload = "{\"d\":{\"counter\":";
String data;


void handleUpdate(byte* payload) {
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& root            = jsonBuffer.parseObject((char*)payload);
  if (!root.success()) {
    //Serial.println("handleUpdate: payload parse FAILED");
    return;
  }
  //Serial.println("handleUpdate payload:"); root.prettyPrintTo(Serial); //Serial.println();

  JsonObject& d               = root["d"];
  JsonArray& fields           = d["fields"];
  for (JsonArray::iterator it  = fields.begin(); it != fields.end(); ++it) {
    JsonObject& field           = *it;
    const char* fieldName       = field["field"];
    if (strcmp (fieldName, "metadata") == 0) {
      JsonObject& fieldValue      = field["value"];
      if (fieldValue.containsKey("publishInterval")) {
        publishInterval             = fieldValue["publishInterval"];
        //Serial.print("publishInterval:"); //Serial.println(publishInterval);
      }
    }
  }
}



void callback(char* topic, byte* payload, unsigned int payloadLength) {
  //Serial.print("callback invoked for topic: "); //Serial.println(topic);
  if (strcmp (subtopic, topic) == 0)
  {
    StaticJsonBuffer<600> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject((char*)payload);
    if (!root.success())
    {
      //Serial.println("handle fly cmd: payload parse FAILED");
      return;
    }
    //Serial.println("fly payload:"); root.prettyPrintTo(Serial); //Serial.println();
  }
  if (strcmp (responseTopic, topic) == 0)  return; // just print of response for now
  if (strcmp (rebootTopic, topic) == 0)
  {
    //Serial.println("Rebooting...");
    ESP.restart();
  }
  if (strcmp (updateTopic, topic) == 0)
  {
    handleUpdate(payload);
  }
  // data receive
  dataReceive = (char*)payload;
  Serial.print(dataReceive);
}


PubSubClient client(server, 1883, callback, wifiClient);
void wifiConnect() {
  Serial.print("Connecting to "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  Serial.print("\nWiFi connected, IP address: "); 
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!!!client.connected()) {
    //Serial.print("Reconnecting MQTT client to "); //Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) {
      //Serial.print(".");
      delay(500);
    }
    //Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(subtopic)) {
    //Serial.println("subscribe to all event OK");
  } else {
    Serial.println("FAIL");
  }

  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& root            = jsonBuffer.createObject();
  JsonObject& d               = root.createNestedObject("d");
  JsonObject& metadata        = d.createNestedObject("metadata");
  metadata["publishInterval"] = publishInterval;
  JsonObject& supports        = d.createNestedObject("supports");
  supports["deviceActions"]   = true;

  char buff[600];
  root.printTo(buff, sizeof(buff));
  //Serial.println("publishing device metadata:"); //Serial.println(buff);
  if (client.publish(manageTopic, buff)) {
    //Serial.println("device Publish ok");
  } else {
    //Serial.print("device Publish failed:");
  }
}


void setup() {
  Serial.begin(9600); //Serial.println();

  wifiConnect();
  mqttConnect();
  initManagedDevice();
}



void publishData() {
  // payload = "{\"d\":\"";
  //   payload += data;
  //   payload += "\"}";

  payload = "{\"d\":{";
  payload += data;
  payload += "}}";

  //Serial.print("Sending payload: ");
  //Serial.println(payload);

  if (client.publish(publishTopic, (char*) payload.c_str())) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }
}

void loop() {
  if (Serial.available() > 0) {
    data = Serial.readString();
    data.trim();
    //Serial.print(data);
    publishData();
  }
  if (!client.loop()) {
    mqttConnect();
  }
}

