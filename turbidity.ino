void setupTurbidity() {
  pinMode(TURBIDITY_PIN, INPUT);
}

int loopTurbidity(){
  // Commented code for analog turbidity readings
  //int sensorValue = analogRead(TURBIDITY_PIN);
  //float turbidityValue = sensorValue * (5.0 / 1024.0);
 
  Serial.println(digitalRead(TURBIDITY_PIN));
  if(digitalRead(TURBIDITY_PIN)==LOW){
    return 1;
  }else{
    return 2;
  }
}
