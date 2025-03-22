#include <WiFi.h>
#include "DisplayUtils.h"

//Connects to the wifi network.
void initWifi()
{
  
  // If the Wifi Status is not connected or the ip address is 0.0.0.0, then connect to the Wifi.
  while ((WiFi.status() != WL_CONNECTED) || (String(WiFi.localIP().toString()) == "0.0.0.0"))
  {
    Serial.printf("Connecting to WIFI SSID %s \n", WIFI_SSID);
    
    
    Display.print(0, ("SSID:" + String(WIFI_SSID)).c_str());
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(1000);
  }
  
  Display.print(1, "IP:" + String(WiFi.localIP().toString()));
  
}