#include <WiFi.h>
#include "DisplayUtils.h"

// Connects to the wifi network.
void initWifi()
{
  // If the Wifi Status is not connected or the IP address is 0.0.0.0, then connect to the Wifi.
  while ((WiFi.status() != WL_CONNECTED) || (String(WiFi.localIP().toString()) == "0.0.0.0"))
  {
    Serial.printf("Connecting to WIFI SSID %s \n", WIFI_SSID);
    Display.print(0, ("SSID:" + String(WIFI_SSID)).c_str());
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(4000);

    // Check for connection errors
    if (WiFi.status() != WL_CONNECTED)
    {
      switch (WiFi.status())
      {
      case WL_NO_SSID_AVAIL:
        Serial.println("Error: SSID not available.");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("Error: Connection failed. Check password.");
        break;
      case WL_DISCONNECTED:
        Serial.println("Error: Disconnected from WiFi.");
        break;
      default:
        Serial.println("Error: Unknown WiFi connection issue.");
        break;
      }
    }
  }

  Display.print(1, "IP:" + String(WiFi.localIP().toString()));
}