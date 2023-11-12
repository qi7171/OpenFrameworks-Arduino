
int sensorValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(A0);
  sensorValue = map(sensorValue, 0,1023, 1,55);
  Serial.write(sensorValue);

}
