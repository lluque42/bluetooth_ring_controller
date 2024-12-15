// Note: Pairing is authenticated automatically by this device

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

void setup() {
  Serial.begin(115200);

  // Initialize bluetooth connection using this device name
  init_bt("Filigring");
}

void loop()
{
  // The other end must send something after stablishing the connection with the Filigring
  // for the SerialBT.available() function to start returning true!!!! 
  while (!SerialBT.available())
  {
    Serial.println("Waiting for a Bluetooth connection... The other end must send something after succesfully pairing and connecting with the Filigring");
    delay(3000);
  }
  //Serial.println("ERROR! Bluetooth unavailable");
  /*
  send_char_bt('t', 5000);
  send_char_bt('v', 5000);
  ...
  */
  //send_test_gestures_bt(5000);
  send_custom_gestures_bt("tvvv^^vvvccdc^^vv", 500);
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
    Serial.printf("i = %d, char sent = '%c'\n", i, gestures[i]);
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
  Serial.printf("Char '%c' sent, now waiting 20ms or %dms\n", c, delay_ms);
  if (delay_ms < 20)
    delay(20);
  else
    delay(delay_ms);
  return (1);
}
