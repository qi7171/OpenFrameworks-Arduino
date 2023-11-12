// 定义常量
const int buttonPin = 2;
const int ledPin = 13;

int buttonState = 0;
int sensorValue = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 按住才会亮
  buttonState = digitalRead(buttonPin);
  if(Serial.available()>0){
     if(buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  }
 Serial.write(buttonState);

 // put your main code here, to run repeatedly:
  sensorValue = analogRead(A0);
  sensorValue = map(sensorValue, 0,1023, 1,55);
  Serial.write(sensorValue);
}
