// Arduino Light sensor code created by AR Builder.
void setup() {
  Serial.begin(9600); // Start Serial.

}

void loop() {
  float A = analogRead(A0);
  Serial.println(A/10.16);// Display serial results in serial monitor.
  
  delay(10000);
  


}
