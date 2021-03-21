void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0){
    String data = Serial.readStringUntil('\n');
    //delay(1000);
    Serial.write("mE2e4");
    //Serial.println(data);
  }
}

enterCoordinates(){
  //receive column first
  
  }
}
