/*
Written for "DOIT ESP32 DEVKIT V1" board.

Before flashing from the Arduino IDE, do this sequence on the board buttons:
- Press and hold BOOT…
- Press and release EN…
- Release BOOT

USB port permissions (Ubuntu)
$ # (One time for the user)
$ sudo adduser lluque dialout
$ # (and every time it's unplugged/plugged)
$ sudo chmod a+rw /dev/ttyUSB0
*/

// set pin numbers
const int touch_0 = 4;
//const int touch_1 = 0;  //Not accessible as a pin
//const int touch_2 = 2;  // Not working properly (check protoboard)
const int touch_3 = 15;
const int touch_4 = 13;
const int touch_5 = 12;
const int touch_6 = 14;
const int touch_7 = 27;
const int touch_8 = 33;
const int touch_9 = 32;

long    baseline[] = {0,0,0,0,0,0,0,0};
/*
From the Lafvin pdf:
  T0 (GPIO 4)
  T1 (GPIO 0)
  T2 (GPIO 2)
  T3 (GPIO 15)
  T4 (GPIO 13)
  T5 (GPIO 12)
  T6 (GPIO 14)
  T7 (GPIO 27)
  T8 (GPIO 33)
  T9 (GPIO 32)
*/

void  calc_baseline(long *baseline)
{
  int           i;
  long          samples;

  samples = 0;
  while (samples < 1000)
  {
    baseline[0] += touchRead(touch_0);
    //baseline[1] += touchRead(touch_1);
    //baseline[2] += touchRead(touch_2);
    baseline[1] += touchRead(touch_3);
    baseline[2] += touchRead(touch_4);
    baseline[3] += touchRead(touch_5);
    baseline[4] += touchRead(touch_6);
    baseline[5] += touchRead(touch_7);
    baseline[6] += touchRead(touch_8);
    baseline[7] += touchRead(touch_9);
    samples++;
  }
  i = 0;
  while (i < 8)
  {
    baseline[i] /= samples;
    i++;
  }
  Serial.println(samples);
}

// Runs once at power on
void setup() {
  // Serial communication speed
  Serial.begin(115200);

  //print_touch_read();  
  // A delay for the serial monitor for debugging purposes
  //delay(5000);
  //calc_baseline(baseline);

  calc_baseline(baseline);
  Serial.println("Ring on line!!!");

  // initialize the pushbutton pin as an input
  //pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  //pinMode(ledPin, OUTPUT);
}
/*
void  print_touch_read(void)
{
  Serial.print("Touch_0: ");
  Serial.println(touchRead(touch_0));
  Serial.print("Touch_1: ");
  Serial.println(touchRead(touch_1));
  Serial.print("Touch_2: ");
  Serial.println(touchRead(touch_2));
  Serial.print("Touch_3: ");
  Serial.println(touchRead(touch_3));
  Serial.print("Touch_4: ");
  Serial.println(touchRead(touch_4));
  Serial.print("Touch_5: ");
  Serial.println(touchRead(touch_5));
  Serial.print("Touch_6: ");
  Serial.println(touchRead(touch_6));
  Serial.print("Touch_7: ");
  Serial.println(touchRead(touch_7));
  Serial.print("Touch_8: ");
  Serial.println(touchRead(touch_8));
  Serial.print("Touch_9: ");
  Serial.println(touchRead(touch_9));
}
*/
void  get_touch_analog_values(int *values)
{
  values[0] = touchRead(touch_0);
  //values[1] = touchRead(touch_1);
  //values[2] = touchRead(touch_2);
  values[1] = touchRead(touch_3);
  values[2] = touchRead(touch_4);
  values[3] = touchRead(touch_5);
  values[4] = touchRead(touch_6);
  values[5] = touchRead(touch_7);
  values[6] = touchRead(touch_8);
  values[7] = touchRead(touch_9);
}

void  get_touch_states(int *values, int *states)
{
  int i;

  i = 0;
  while (i < 8)
  {
    if (values[i] < abs(baseline[i] - values[i]))
      states[i] = 1;
    else
      states[i] = 0;
    i++;
  }
}

void  print_array(char *label, int *array)
{
  int i;

  i = 0;
  while (i < 8)
  {
    Serial.print(label);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(array[i]);
    Serial.print(" (baseline = ");
    Serial.println(baseline[i]);
    i++;
  }
}

void loop() {
  int i;
  int states[8];
  int values[8];

  //calc_baseline(baseline);
  get_touch_analog_values(values);
  print_array("value_", values);
  get_touch_states(values, states);
  print_array("state_", states);
  /*
  i = 0;
  while (i < 10)
  {
    Serial.print("Baseline_");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(baseline[i]);
    i++;
  }
  */
  
  
  // put your main code here, to run repeatedly:

  delay(200);

  return;

  /*
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
  */
}
