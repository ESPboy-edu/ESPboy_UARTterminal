void setup() {
Serial.begin(115200);
}

void loop() {
  uint8_t dta[101];
  for (uint8_t i=0; i<100; i++) dta[i]=random(255);
  for (uint8_t i=0; i<random(80); i++) Serial.write(dta[i]);
  delay(2000);

  if(Serial.available()){
    Serial.flush();
    while(Serial.available()) Serial.read();
    Serial.print("READED!");
    }
}
