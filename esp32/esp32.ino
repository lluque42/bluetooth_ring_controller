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
/*
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
BLECharacteristic *pCharacteristic;

bool deviceConnected = false;
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
*/

#include "BluetoothSerial.h"

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

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

// The delay (ms) in the loop() function that determines
// the sample frequency of the capacitive touch inputs.
const int sampling_period = 200;

// Number of samples needed to calculate the baseline
// for the capacitive touch inputs
const int baseline_samples = 1000;

long    baseline[] = {0,0,0,0,0,0,0,0};

//
const int buttonPin = 18;
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

// The code that will be run once at power on
void setup()
{
  pinMode(buttonPin, INPUT);
  /*
  BLEDevice::init("ESP32");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                     CHARACTERISTIC_UUID,
                     BLECharacteristic::PROPERTY_READ |
                     BLECharacteristic::PROPERTY_WRITE |
                     BLECharacteristic::PROPERTY_NOTIFY
                   );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();
  
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
  //BT end
  */

  // Serial communication speed
  Serial.begin(serial_speed);
  delay(1000);
  // Initialize bluetooth connection using this device name
  init_bt("Filigring");
  calc_baseline(baseline);
  Serial.println("Ring on line!!!");
}

// The code that will be run repeatedly
void loop()
{
  int i;
  int states[capacitive_touch_input_number];
  int values[capacitive_touch_input_number];
  int buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);
  //return;

  get_touch_analog_values(values);
  //print_array("value_", values, capacitive_touch_input_number);
  get_touch_states(values, states);


  //Serial.println("ERROR! Bluetooth unavailable");
  /*
  send_char_bt('t', 5000);
  send_char_bt('v', 5000);
  ...
  */
  //send_test_gestures_bt(5000);

  
  // To enable/disable the sending of gestures
  if (buttonState == HIGH)
  {
    print_array("state_", states, capacitive_touch_input_number);
    
  }
  else
  {


    // The other end must send something after stablishing the connection with the Filigring
    // for the SerialBT.available() function to start returning true!!!! 
    while (!SerialBT.available())
    {
      Serial.println("Waiting for a Bluetooth connection... The other end must send something after succesfully pairing and connecting with the Filigring");
      delay(3000);
    }

    i = 0;
    int pressed = 0;
    int pressed_count = 0;

    while (i < 8)
    {
      if (pressed == 0 && states[i] == 1)
      {
        pressed = 1;
        send_custom_gestures_bt("tvvv^^vvvccdc^^vv", 500);
        break ;
      }
      i++;
    }
    if (pressed == 1)
    {
      pressed_count = 0;
      int j = 0;
      while (j < 8)
      {
        pressed_count += states[i];
        j++;
      }
      if (pressed_count == 0)
      {
        pressed = 0;
      }
    }
    

  /*
    // BT code begins
    if (deviceConnected) {
      // Cuando quieras enviar una señal de captura
      uint8_t value = 1;
      pCharacteristic->setValue(&value, 1);
      pCharacteristic->notify();
      delay(1000); // Ajusta según necesites
    }
    // BT code ends
  */
  // gestureInterpreter(states);
    
  }
  delay(sampling_period);
}

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



// Initializes bluetooth connection using the argument as the device name
void  init_bt(char *device_name)
{
  // Uncomment this to delete paired devices; Must be called after begin
  SerialBT.deleteAllBondedDevices();
  SerialBT.begin(device_name);
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name);
}

int send_test_gestures_bt(int delay_ms)
{
  delay(delay_ms);
  send_char_bt('t', 500);
  send_char_bt('v', 200);
  send_char_bt('v', 200);
  send_char_bt('v', 200);
  send_char_bt('c', 200);
  return (1);
}

int send_custom_gestures_bt(std::string gestures, int delay_ms)
{
  int i;
  int length;
  /*
  const char *gestures_cstr;

  gestures_cstr = gestures.c_str();
  */
  length = gestures.size();
  i = 0;
  while (i < length)
  {
    send_char_bt(gestures[i], delay_ms);
    //Serial.printf("i = %d, char sent = '%c'\n", i, gestures[i]);
    i++;
  }
  
  return (1);
  /*
  {
    send_char_bt(gestures_cstr[i], delay_ms);
    Serial.printf("i = %d\n", i);
    i++;
  }
  */
}

int send_char_bt(char c, int delay_ms)
{
  if (!SerialBT.available())
  {
    Serial.println("ERROR! Bluetooth unavailable");
    return (0);
  }
  //Serial.printf("Received '%s'\n", SerialBT.read());
  SerialBT.write(c);
  //Serial.printf("Char '%c' sent, now waiting 20ms or %dms\n", c, delay_ms);
  if (delay_ms < 20)
    delay(20);
  else
    delay(delay_ms);
  return (1);
}

