
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <String.h>
#include <Servo.h>
#include <global.h>

 
/**** CREATE MQTT CLIENT ****/
WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_PORT, MQTT_USERNAME, MQTT_PASSWORD); 


/**** DECLARE MQTT FEEDS ****/

//SUBSCRIPTIONS
Adafruit_MQTT_Subscribe blind_status_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/blind_status"); 
Adafruit_MQTT_Subscribe security_system_subscription = Adafruit_MQTT_Subscribe(&mqtt, MQTT_USERNAME "home/security_power"); 

//PUBLISHING
Adafruit_MQTT_Publish light_intesity_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/light_intesity"); 
Adafruit_MQTT_Publish security_motion_publish = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "home/security_motion"); 

bool security_flag = false;
int last_light_intensity = 0;
char* last_blind_status = "closed"; 
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
    
}
/*************************************************************************************************/

void controlBlinds(char* message){
  if(message == "closed"){
    myservo.attach(SERVO);
    myservo.writeMicroseconds(800);
    delay(4000);
    myservo.detach();
    Serial.println("Lowering Blinds");
  }
  else if(message == "open"){
    myservo.attach(SERVO);
    myservo.writeMicroseconds(1800);
    delay(4000);
    myservo.detach();
    Serial.println("Raising Blinds");
  }
}
/*************************************************************************************************/
void soundAlarm(){
  for(int i = 0; i <20; i++){
    digitalWrite(ALARM, HIGH);
    delay(25);
    digitalWrite(ALARM, LOW);
    delay(25); 
  }
}
/*************************************************************************************************/
void loop() {
  mqttConnect(mqtt);

  if(security_flag){
    if(digitalRead(SECURITY_MOTION) == HIGH) {
      alarm_triggered = true;
      security_motion_publish.publish("x");   
    }
  }
  if(alarm_triggered){
    soundAlarm(); 
  }
  int new_light_intensity = (int)analogRead(PHOTOCELL)/5.35;
  if(last_light_intensity != new_light_intensity){
    light_intesity_publish.publish(new_light_intensity);
    last_light_intensity = new_light_intensity;
  }

  //READ SUBSCRIPTION
  Adafruit_MQTT_Subscribe *subscription; 
  while ((subscription = mqtt.readSubscription(3000))) { 
    if (subscription == &blind_status_subscription) { 
      char *message = (char *)blind_status_subscription.lastread; 
      controlBlinds((char*)message);
    }
    else if(subscription == &security_system_subscription){
      char *message = (char *)blind_status_subscription.lastread;
      if(message == "on"){
        security_flag = true;
      }
      else if(message == "off"){
        security_flag = false;
        alarm_triggered = false;
      }
    }
  }
}
/*************************************************************************************************/
