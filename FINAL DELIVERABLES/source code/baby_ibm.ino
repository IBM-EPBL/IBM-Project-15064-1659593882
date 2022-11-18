#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h> 


const char* ssid = "SMART-G";
const char* password = "10112019";

#define WT A0
#define CY D0 


#define ID "yachlf" 
#define DEVICE_TYPE "ESP8266" 
#define DEVICE_ID "CHILD" 
#define TOKEN "IOT-1234567" 

char server[] = ID ".messaging.internetofthings.ibmcloud.com";
char publish_Topic1[] = "iot-2/evt/Data1/fmt/json";
char publish_Topic2[] = "iot-2/evt/Data2/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ID ":" DEVICE_TYPE ":" DEVICE_ID;

WiFiClient wifiClient;
PubSubClient client(server, 1883, NULL, wifiClient);

void setup() {
    Serial.begin(115200);
//    dht.begin();
    Serial.println();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    } 
    Serial.println("");
    Serial.println(WiFi.localIP());

    if (!client.connected()) {
        Serial.print("Reconnecting client to ");
        Serial.println(server);
        while (!client.connect(clientId, authMethod, token)) {
            Serial.print(".");
            delay(500);
        }
        Serial.println("Connected TO IBM IoT cloud!");
    }
}

long previous_message = 0;
void loop() {
    client.loop();
    long current = millis();
    if (current - previous_message > 500) {
        previous_message = current;
         float hum = map(analogRead(A0), 0, 1023, 100, 0);
         float temp = map(digitalRead(D0), 0, 1, 100, 0);
         if (isnan(hum) || isnan(temp)  ){
    Serial.println(F("Failed to read sensor!"));
    return;
  }

  Serial.print("Cry Sound: ");
  Serial.print(temp);
  Serial.print(" Wed Level: ");
  Serial.print(hum);
  Serial.print("%");
 
       String payload = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload += ",\"Cry Sound\":";
              payload += temp;
              payload += "}}";
       
        Serial.print("Sending payload: ");
        Serial.println(payload);

        if (client.publish(publish_Topic1, (char*) payload.c_str())) { 
            Serial.println("Published successfully");
        } else {
            Serial.println("Failed");
        }
       String payload1 = "{\"d\":{\"Name\":\"" DEVICE_ID "\"";
              payload1 += ",\"Wed Level\":";
              payload1 += hum;
              payload1 += "}}";
              Serial.print("Sending payload: ");
              Serial.println(payload1);
              Serial.println('\n');
       
         if (client.publish(publish_Topic2, (char*) payload1.c_str())) {
            Serial.println("Published successfully");
        } else {
            Serial.println("Failed");
        }
    }
}
