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
#define MotionSensor 13
#define SecurityLight 12
/**** CREATE MQTT CLIENT ****/
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 

/**** DECLARE MQTT FEEDS ****/

Adafruit_MQTT_Publish motion = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/motion"); 
//SUBSCRIBE
Adafruit_MQTT_Subscribe security_light = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/security_light");
 
/*************************************************************************************************/

void setup() {
    Serial.begin(9600);
    connectWiFi();
    //SUBSCRIBE TO FEED
    mqtt.subscribe(&security_light);
    pinMode(MotionSensor, INPUT);
    pinMode(SecurityLight, OUTPUT);
}
/*************************************************************************************************/
void soundAlarm(){
  for(int i = 0; i <20; i++){
    digitalWrite(SecurityLight, HIGH);
    delay(50);
    digitalWrite(SecurityLight, LOW);
    delay(50); 
  }
}
void loop() {
  motion.publish(1);
  delay(300);   
  //CONNECT TO MQTT
  mqttConnect();
  Serial.println(digitalRead(MotionSensor));

  if(digitalRead(MotionSensor) == HIGH){
    motion.publish(1); //publish motion and the RPI will give it a timestamp
    soundAlarm(); 
  }
  else{
    digitalWrite(SecurityLight, LOW);
    
  }
  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(1000))) { 
    if (subscription == &security_light) { 
      char *message = (char *)security_light.lastread; 
      if((String)message == "on"){
        digitalWrite(SecurityLight, HIGH);
      }
      else if((String)message == "off"){
        digitalWrite(SecurityLight, LOW);
        
      }
    }
  }  
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
