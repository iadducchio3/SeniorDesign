int sensor = 13;  // Digital pin D7

void setup() {

  pinMode(sensor, INPUT);   // declare sensor as input
}

void loop() {
Serial.begin(9600);
  long state = digitalRead(sensor);
    if(state == HIGH) {
      digitalWrite (sensor, HIGH);
      Serial.println("Motion detected!");
    }else{
      Serial.println("No Motion Detected");
    }
      delay(300);
    
   
}
