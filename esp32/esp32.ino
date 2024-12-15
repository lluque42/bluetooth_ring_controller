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

const int serial_speed = 115200;
const int capacitive_touch_input_number = 8;
\
// The delay (ms) in the loop() function that determines
// the sample frequency of the capacitive touch inputs.
const int sampling_period = 200;

// Number of samples needed to calculate the baseline
// for the capacitive touch inputs
const int baseline_samples = 1000;

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

// Averages the values for the capacitive touch GPIOs 
// to obtain a baseline
void  calc_baseline(long *baseline)
{
  int           i;
  long          samples;

  samples = 0;
  while (samples < baseline_samples)
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
  while (i < capacitive_touch_input_number)
  {
    baseline[i] /= samples;
    i++;
  }
  Serial.println(samples);
}

// Initializes an array from the input pin values
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

// Signal conditioning from the analog values to 0 or 1 to
// represent the touch / no-touch condition of each of the
// capacitive touch GPIO circuits.
void  get_touch_states(int *values, int *states)
{
  int i;

  i = 0;
  while (i < capacitive_touch_input_number)
  {
    if (values[i] < abs(baseline[i] - values[i]))
      states[i] = 1;
    else
      states[i] = 0;
    i++;
  }
}

// Prints an integer array with a label
void  print_array(char *label, int *array, int number_of_elements)
{
  int i;

  i = 0;
  while (i < number_of_elements)
  {
    Serial.print(label);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(array[i]);
    //Serial.print(" (baseline = ");
    //Serial.println(baseline[i]);
    i++;
  }
}

void  getZeros(int *states, int *upperZeros, int *lowerZeros) {
  int uZeros = 0;
  int lZeros = 0;
  int groupFlag = 0;

  int i = 0;
  while (i < 8) {
    if (!groupFlag && states[i] == 0)
      uZeros++;
    else if (groupFlag && states[i] == 0)
      lZeros++;
    else if (groupFlag == 0 && states[i] == 1)
      groupFlag = 1;
    i++;
  }
}

/**
* Function that checks if there is a unique isolated group of true states
* @return  It returns false if there is not an unique group of true states
*          It return true otherwise
*/
int isolatedTrueGroup(int *states) {
  int isolatedFlag = 0;
  int prevState = 0;

  int i = 0;
  while (i < 8) {
    if (states[i] == 1 && isolatedFlag == 0)
      isolatedFlag = 1;
    else if (prevState == 0 && states[i] == 1 && isolatedFlag == 1)
      return 0;
    prevState = states[i];
    i++;
  }
  return 1;
}

void  checkGesture(int prevUpperZeros, int prevLowerZeros, int upperZeros, int lowerZeros)
{
  
}

int falseStates(int *states)
{
  int i = 0;

    while (i < 8)
    {
      if (states[i] == 0)
        return 0;
      i++;
    }
    return 1;
}

void  gestureInterpreter(int *states) {
  static int  *prePrevStates = 0;
  static int  *prevStates = 0;
  static int  prevUpperZeros = 0;
  static int  prevLowerZeros = 0;
  int         upperZeros = 0;
  int         lowerZeros = 0;

  if (!isolatedTrueGroup(states)) {
    upperZeros = 0;
    lowerZeros = 0;
    prevUpperZeros = 0;
    prevLowerZeros = 0;
    return ;
  }
  if (upperZeros == 0 && lowerZeros == 0) {
    getZeros(states, &upperZeros, &lowerZeros);
  }
  if (falseStates(prePrevStates) && falseStates(states) && isolatedTrueGroup(prevStates))
  {
    Serial.println("Click!");
  }
  //checkGesture(prevUpperZeros, prevLowerZeros, upperZeros, lowerZeros);
  prevUpperZeros = upperZeros;
  prevLowerZeros = lowerZeros;
  prePrevStates = prevStates;
  prevStates = states;
}

// The code that will be run once at power on
void setup()
{
  // Serial communication speed
  Serial.begin(serial_speed);
  calc_baseline(baseline);
  Serial.println("Ring on line!!!");
}

// The code that will be run repeatedly
void loop()
{
  int i;
  int states[capacitive_touch_input_number];
  int values[capacitive_touch_input_number];

  get_touch_analog_values(values);
  //print_array("value_", values, capacitive_touch_input_number);
  get_touch_states(values, states);
 // gestureInterpreter(states);
  print_array("state_", states, capacitive_touch_input_number);
  delay(sampling_period);
}
