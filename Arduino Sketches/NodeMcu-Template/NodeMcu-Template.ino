/*************************************************************************************
 *                                                                                   *
 *                                                                                   *
 *                                                                                   *
 *                                                                                   *
 ************************************************************************************/
 
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>

/**** PREPROCESSOR DIRECTIVES ****/
#define ROUTER_SSID "ikeNet"
#define ROUTER_PASSWORD "d1x0ut4harambe"
#define MQTT_SERVER "192.168.200.186"
#define MQTT_PORT 1883
#define MQTT_USERNAME ""
#define MQTT_PASSWORD ""

/**** DECLARE MQTT FEEDS ****/
//PUBLSH 
//Adafruit_MQTT_Publish light_intesity_feed = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "/sensors/light_intesity"); 
//SUBSCRIBE
//Adafruit_MQTT_Subscribe light_status = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "/status/light"); 


/**** WIFI CONNECTION ****/
void setup() {
    Serial.begin(9600);
    delay(5000); //NOT NEEDED, JUST GIVES TIME TO OPEN SERIAL MONITOR AND SEE WHAT'S HAPPENING

    Serial.print("Trying to Connect to WIFI");
    WiFi.begin(ROUTER_SSID,ROUTER_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
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

//SUBSCRIBE TO FEED
mqtt.subscribe(&light_status); 

}

void loop() {
//CONNECT TO MQTT
  mqttConnect();
  
//TODO
//GET LIGHT INTESITY
int light_intesity = 52; //PLACEHOLDER

if(light_intesity > 52){ //RANDOM VALUE
  //PUBLISH LIGHT INTESITY TO MQTT SERVER
  light_intesity_feed.publish(light_intesity);  
  delay(5000);
}

//READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription())) { 
    if (subscription == &light_status) { 
      char *message = (char *)light_status.lastread; 
      Serial.print("The Following was Retreived from the Subscription"); 
      Serial.println(message);
      //IF message.payload is 'ON' turn on, 'OFF' turn off 
    }
  }
}
  //DEPENDING ON MESSAGE DO SOMETHING HERE 

//CONNECT TO MQTT SERVER IF NOT CONNECTED
  void mqttConnect(){
    if(mqtt.connected()){
      return; 
    }
    else{
      mqtt.connect();
    }
  }
}
