const int buttonPin = 2;
const int ledPin = 13;

int buttonState = 0;
int sensorValue = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // push to ligh
  buttonState = digitalRead(buttonPin);
  if(Serial.available()>0){
     if(buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  }
 Serial.write(buttonState);
 
 sensorValue = analogRead(A0);
  sensorValue = map(sensorValue, 0,1023, 1,55);
  Serial.write(sensorValue);

 
}
