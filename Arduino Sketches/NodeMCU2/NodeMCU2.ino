#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <String.h>
#include "global.h"
#include "DHT.h"

#define DHTPIN 13   
#define DHTTYPE DHT11
#define RELAY 12
#define LIGHT 15
#define HOME_MOTION 14



/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 


/**** DECLARE MQTT FEEDS ****/

//SUBSCRIPTIONS
Adafruit_MQTT_Subscribe air_conditioning_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/fan_power"); 
Adafruit_MQTT_Subscribe light_power_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/light_power"); 

//PUBLISHING
Adafruit_MQTT_Publish temperature_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/temperature_sensor"); 
Adafruit_MQTT_Publish motion_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/home_motion"); 



int last_temp = 0; 
String last_light_status = "";
DHT dht(DHTPIN, DHTTYPE);

/*************************************************************************************************/
void setup() {
    Serial.begin(9600);
      

    dht.begin();
    connectWiFi();
    //SUBSCRIBE TO FEED
    mqtt.subscribe(&air_conditioning_subscription);
    mqtt.subscribe(&light_power_subscription); 
    pinMode(RELAY,OUTPUT);
    pinMode(HOME_MOTION,INPUT);
    pinMode(LIGHT,OUTPUT);
    mqttConnect(mqtt);
    
    
}
/*************************************************************************************************/

void loop() {
  delay(1000);
  int new_temp = dht.readTemperature(true);
  //Serial.println(new_temp);
  if(isnan(new_temp)){
    new_temp = last_temp;
    Serial.println(new_temp);
  }
  if(new_temp!= last_temp){
    last_temp = new_temp;
    temperature_publish.publish(new_temp);
  }

  if(digitalRead(HOME_MOTION) == HIGH) {
      Serial.println("Motion Detected");
      motion_publish.publish("x");
  }
   
  
  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(1000))) { 
    if (subscription == &air_conditioning_subscription) { 
      char *message = (char *)air_conditioning_subscription.lastread; 
      if((String)message == "on"){
        digitalWrite(RELAY,HIGH);
      }
      else if((String)message == "off"){
        digitalWrite(RELAY,LOW);
      }
    }
    else if(subscription == &light_power_subscription){
      char *message = (char *)light_power_subscription.lastread;
      if((String)message == "on"){
          digitalWrite(LIGHT,HIGH);
      }
      else if((String)message == "off"){    
        digitalWrite(LIGHT,LOW);
      }
    }
  }
    if(!mqtt.connected()){
    mqttConnect(mqtt);
  }
}
/*************************************************************************************************/
