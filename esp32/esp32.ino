/*
For "DOIT ESP32 DEVKIT V1" board.
Before flashing from the Arduino IDE, do this sequence on the board buttons:
- Press and hold BOOT…
- Press and release EN…
- Release BOOT
*/

// Runs once at power on
void setup() {
  // Serial communication speed
  Serial.begin(115200);
  
  // TODO
  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // TODO
  // read the state of the pushbutton value
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH
  if (buttonState == HIGH) {
    // turn LED on
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off
    digitalWrite(ledPin, LOW);
  }
}
