#include "DHT.h"
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>
#include <String.h>

#define ROUTER_SSID "ikeNet"
#define ROUTER_PASSWORD "d1x0ut4harambe"
#define MQTT_SERVER "192.168.200.186"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define DHTPIN 13   
#define LED 12
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);


/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 

/**** DECLARE MQTT FEEDS ****/
//PUBLSH 
Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/temperature_sensor"); 
//SUBSCRIBE




void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  dht.begin();

}

void loop() {
  mqttConnect();
  delay(2000);
  float f = dht.readTemperature(true);
 
  if (isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  temp.publish((int)f);
  Serial.println(f);
  digitalWrite(LED,HIGH);
  delay(1000);
  digitalWrite(LED,LOW);
 
}

/*************************************************************************************************/
//FUNCTION|CONNECT TO MQTT SERVER IF NOT CONNECTED
void mqttConnect(){
    if(mqtt.connected()){
      return; 
    }
    else{
      mqtt.connect();
    }
}
/*************************************************************************************************/
//FUNCTION|CONNECT TO WIFI
void connectWiFi(){
      Serial.println("Trying to Connect to WiFi");
      WiFi.begin(ROUTER_SSID,ROUTER_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(400);
    }
    if(WiFi.isConnected()) {
        Serial.println("");
        Serial.println("Connection Successful!");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        Serial.print("Default Gateway IP: ");
        Serial.println(WiFi.gatewayIP().toString());
    }
    else {
        Serial.println("");
        Serial.println("Connection Failed");
    }
}
/*************************************************************************************************/
