/* Wi-Fi STA Connect and Disconnect Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

*/
#include <WiFi.h>

// Wifi connection parameters
const char *wifi_ssid = "42malaga_student";
const char *wifi_password = "42St4dentM2laga";

int init_error_status = 0;


// For trying to stablish the wifi connection the connection
// will try for about 10 seconds (20x 500ms)
int wifi_tryDelay = 500;
int wifi_numberOfTries = 20;

//int btnGPIO = 0;
//int btnState = false;

void setup() {
  Serial.begin(115200);
  delay(10);

  if (!init_wifi(wifi_ssid, wifi_password))
  {
    Serial.printf("Error connecting to '%s' wifi!!!\n", wifi_ssid);
    init_error_status = 1;
  }
}

void loop() {
  if (!init_error_status)
    Serial.println(WiFi.localIP());
  // Read the button state
  //btnState = digitalRead(btnGPIO);
/*
  if (btnState == LOW) {
    // Disconnect from WiFi
    Serial.println("[WiFi] Disconnecting from WiFi!");
    // This function will disconnect and turn off the WiFi (NVS WiFi data is kept)
    if (WiFi.disconnect(true, false)) {
      Serial.println("[WiFi] Disconnected from WiFi!");
    }
    delay(1000);
  }
  */
}

int init_wifi(const char *ssid, const char *password)
{
  // Set GPIO0 Boot button as input
  //pinMode(btnGPIO, INPUT);

  // We start by connecting to a WiFi network
  // To debug, please enable Core Debug Level to Verbose
  if (!wifi_connect(ssid, password))
  {
    Serial.println("ERROR while initializing wifi!");
    return (0);
  }

  // TODO Now open an udp socket or something
  //...
  return (1);
}

int wifi_connect(const char *ssid, const char *password)
{
  Serial.println();
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  // Auto reconnect is set true as default
  // To set auto connect off, use the following function
  //    WiFi.setAutoReconnect(false);



  // Wait for the WiFi event
  while (true) {

    switch (WiFi.status()) {
      case WL_NO_SSID_AVAIL: Serial.println("[WiFi] SSID not found"); break;
      case WL_CONNECT_FAILED:
        Serial.print("[WiFi] Failed - WiFi not connected! Reason: ");
        return (0);
        break;
      case WL_CONNECTION_LOST: Serial.println("[WiFi] Connection was lost"); break;
      case WL_SCAN_COMPLETED:  Serial.println("[WiFi] Scan is completed"); break;
      case WL_DISCONNECTED:    Serial.println("[WiFi] WiFi is disconnected"); break;
      case WL_CONNECTED:
        Serial.println("[WiFi] WiFi is connected!");
        Serial.print("[WiFi] IP address: ");
        Serial.println(WiFi.localIP());
        return (1);
        break;
      default:
        Serial.print("[WiFi] WiFi Status: ");
        Serial.println(WiFi.status());
        break;
    }
    delay(wifi_tryDelay);

    if (wifi_numberOfTries <= 0) {
      Serial.print("[WiFi] Failed to connect to WiFi!");
      // Use disconnect function to force stop trying to connect
      WiFi.disconnect();
      return (0);
    } else {
      wifi_numberOfTries--;
    }
  }
}