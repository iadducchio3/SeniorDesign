
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <String.h>
#include <Servo.h>
#include <global.h>

#define SECURITY_MOTION 13
#define ALARM 12
#define SERVO 0
#define PHOTOCELL A0
 
/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 


/**** DECLARE MQTT FEEDS ****/

//SUBSCRIPTIONS
Adafruit_MQTT_Subscribe blind_status_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/blind_status"); 
Adafruit_MQTT_Subscribe security_system_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/security_power"); 

//PUBLISHING
Adafruit_MQTT_Publish light_intesity_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/light_intensity"); 
Adafruit_MQTT_Publish security_motion_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/security_motion"); 
Adafruit_MQTT_Publish security_triggered_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/security_triggered"); 
Adafruit_MQTT_Publish NodeMCU1_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/security_triggered"); 


bool security_flag = false;
int last_light_intensity = 0;
String last_blind_status = "open"; 
bool alarm_triggered = false; 
Servo myservo;
/*************************************************************************************************/
void setup() {
    Serial.begin(9600);
    connectWiFi();
    //SUBSCRIBE TO FEED
    mqtt.subscribe(&blind_status_subscription);
    mqtt.subscribe(&security_system_subscription); 
    pinMode(SERVO,OUTPUT);
    pinMode(ALARM,OUTPUT);
    pinMode(PHOTOCELL,INPUT);
    pinMode(SECURITY_MOTION,INPUT);
    mqttConnect(mqtt);
}
/*************************************************************************************************/

void controlBlinds(String message){
  Serial.println(message);
  if(message == "closed" && last_blind_status != "closed"){
    myservo.attach(SERVO);
    myservo.writeMicroseconds(800);
    delay(4000);
    myservo.detach();
    last_blind_status = "closed";
    Serial.println("Lowering Blinds");
  }
  else if(message == "open" && last_blind_status != "open"){
    myservo.attach(SERVO);
    myservo.writeMicroseconds(1800);
    delay(4000);
    myservo.detach();
    last_blind_status = "open";

  }
}
/*************************************************************************************************/
void soundAlarm(){
  for(int i = 0; i <20; i++){
    digitalWrite(ALARM, HIGH);
    delay(75);
    digitalWrite(ALARM, LOW);
    delay(75); 
  }
}
/*************************************************************************************************/
void loop() {
  delay(1000);
  if(security_flag){
    Serial.println("Security System On");
    if(digitalRead(SECURITY_MOTION) == HIGH) {
      Serial.println("Motion Detected");
      alarm_triggered = true;
      security_motion_publish.publish("x");
      security_triggered_publish.publish("red");   
    }
  }
  if(alarm_triggered){
    soundAlarm(); 
  }
  
  int new_light_intensity = (1024 - (int)analogRead(PHOTOCELL))/10.25;
  Serial.println(new_light_intensity);
  if(last_light_intensity+3 < new_light_intensity || last_light_intensity -3 > new_light_intensity){
    light_intesity_publish.publish(new_light_intensity);
    last_light_intensity = new_light_intensity;
    if(new_light_intensity > 80){
      last_blind_status = "open";
    }
  }

  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(1000))) { 
    if (subscription == &blind_status_subscription) { 
      char *message = (char *)blind_status_subscription.lastread; 
      controlBlinds((String)message);
    }
    else if(subscription == &security_system_subscription){
      char *message = (char *)security_system_subscription.lastread;
      if((String)message == "on"){
        Serial.println("security system turned on");
        security_flag = true;
      }
      else if((String)message == "off"){
        Serial.println("security system turned off");
        security_flag = false;
        alarm_triggered = false;
        security_triggered_publish.publish("normal");
      }
    }
  }
  if(!mqtt.connected()){
    mqttConnect(mqtt);
  }
}
/*************************************************************************************************/
