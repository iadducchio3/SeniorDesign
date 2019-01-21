/*************************************************************************************
 *                                                                                   *
 *                                                                                   *
 *                                                                                   *
 *                                                                                   *
 ************************************************************************************/
 
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>
#include <String.h>

/**** PREPROCESSOR DIRECTIVES ****/
#define ROUTER_SSID "ikeNet"
#define ROUTER_PASSWORD "d1x0ut4harambe"
#define MQTT_SERVER "192.168.200.186"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""
#define LED D0

/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 

/**** DECLARE MQTT FEEDS ****/
//PUBLSH 
Adafruit_MQTT_Publish light_intensity_feed = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/light_sensor"); 
//SUBSCRIBE
Adafruit_MQTT_Subscribe light_power = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/light_power"); 

/*************************************************************************************************/
void setup() {
    Serial.begin(9600);
    connectWiFi();
    //SUBSCRIBE TO FEED
    mqtt.subscribe(&light_power); 
    pinMode(LED, OUTPUT);

    
}
/*************************************************************************************************/
void loop() {
  //CONNECT TO MQTT
  mqttConnect();
  
  //GET LIGHT INTESITY
  int light_intensity = (int)analogRead(A0)/5.35; //PLACEHOLDER
  //IF THRESHOLD BROKEN, PUBLISH VALUE TO MQTT SERVER
  if(light_intensity > 50 || light_intensity <10){
    light_intensity_feed.publish(light_intensity);  
  }
  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(10000))) { 
    if (subscription == &light_power) { 
      char *message = (char *)light_power.lastread; 
      if((String)message == "on"){
        digitalWrite(LED,HIGH);
      }
      else if((String)message == "off"){
        digitalWrite(LED,LOW);
      }
    }
  }
  float val = analogRead(A0);
  Serial.println(val/5.35);
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
