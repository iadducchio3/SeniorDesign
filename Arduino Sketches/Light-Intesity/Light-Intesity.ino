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

/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 

/**** DECLARE MQTT FEEDS ****/
//PUBLSH 
Adafruit_MQTT_Publish light_intesity_feed = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "sensors/light_intesity"); 
//SUBSCRIBE
Adafruit_MQTT_Subscribe light_status = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "status/light"); 

/*************************************************************************************************/
void setup() {
    Serial.begin(9600);
    delay(5000); //NOT NEEDED, JUST GIVES TIME TO OPEN SERIAL MONITOR AND SEE WHAT'S HAPPENING
    connectWiFi();
    //SUBSCRIBE TO FEED
    mqtt.subscribe(&light_status); 
}
/*************************************************************************************************/
void loop() {
  //CONNECT TO MQTT
  mqttConnect();
  
  //GET LIGHT INTESITY
  int light_intesity = 52; //PLACEHOLDER
  //PUBLISH LIGHT INTESITY TO MQTT SERVER

  Serial.println("Publishing Light Intesity");
  light_intesity_feed.publish(light_intesity);  
  delay(10000);
  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(5000))) { 
    Serial.println("Reading Subscription");
    if (subscription == &light_status) { 
      char *message = (char *)light_status.lastread; 
      Serial.print("The Following was Retreived from the Subscription"); 
      Serial.println(message); 
    }
  }
  
  //IF SUBSCRIPTION SAYS 'ON' TURN ON LIGHT
  //HERE 
}
/*************************************************************************************************/

//FUNCTION|CONNECT TO MQTT SERVER IF NOT CONNECTED
void mqttConnect(){

    if(mqtt.connected()){
          Serial.println("MQTT Connected");
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
        delay(200);
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
