#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  
  myservo.attach(13);
  myservo.writeMicroseconds(800);
  delay(4000);
  myservo.detach(); 
} 

void loop() {
Serial.println("Hello World");
delay(2000);  
} 
