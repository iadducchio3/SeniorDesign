#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <String.h>
#include <global.h>
#include "DHT.h"

 
/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 


/**** DECLARE MQTT FEEDS ****/

//SUBSCRIPTIONS
Adafruit_MQTT_Subscribe air_conditioning_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/fan_power"); 
Adafruit_MQTT_Subscribe light_power_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/light_power"); 

//PUBLISHING
Adafruit_MQTT_Publish temperature_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/temperature_sensor"); 
Adafruit_MQTT_Publish motion_publsih = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/home_motion"); 

int last_temp = 0; 
DHT dht(DHTPIN, DHTTYPE);

/*************************************************************************************************/
void setup() {
    Serial.begin(9600);
    dht.begin();
    connectWiFi();
    //SUBSCRIBE TO FEED
    mqtt.subscribe(&air_conditioning_subscription);
    mqtt.subscribe(&light_power_subscription); 
    pinMode(FAN,OUTPUT);
    pinMode(LIGHT,OUTPUT);
    pinMode(HOME_MOTION,INPUT);
    
}
/*************************************************************************************************/
void loop() {
  mqttConnect(mqtt);
  
  float new_temp = dht.readTemperature(true);
  if(isnan(new_temp)){
    new_temp = last_temp;
  }
  if(new_temp!= last_temp){
    last_temp = new_temp;
    temperature_publish.publish(new_temp);
  }

  
  
  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(3000))) { 
    if (subscription == &air_conditioning_subscription) { 
      char *message = (char *)air_conditioning_subscription.lastread; 
      if(message == "on"){
        digitalWrite(FAN,HIGH);
      }
      else if(message == "off"){
        digitalWrite(FAN,LOW);
      }
    }
    else if(subscription == &light_power_subscription){
      char *message = (char *)light_power_subscription.lastread;
      if(message == "on"){
        digitalWrite(LIGHT,HIGH);
      }
      else if(message == "off"){
        digitalWrite(LIGHT,LOW);
      }
    }
  }
}
/*************************************************************************************************/
