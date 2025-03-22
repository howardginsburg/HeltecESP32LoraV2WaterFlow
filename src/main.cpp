#include "Arduino.h"
#include "heltec.h"
#include "DisplayUtils.h"
#include "WifiUtils.h"
#include "MQTTClient.h"
#include <NewPing.h>

#define TRIGGER_PIN  17  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
bool previousState = false;
bool initialMessageSent = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Display.init();
}


void loop() {
  initWifi();
  
  // Send ping, get ping time in microseconds (uS).
  unsigned int uS = sonar.ping();
  // Convert ping time to distance (0 = outside set distance range, no ping echo)
  unsigned int distance = uS / US_ROUNDTRIP_CM;

  Serial.println("Measurement: " + String(distance) + "cm");
  Display.print(3, "Measurement: " + String(distance));

  // Water is flowing if we get a response > 0.  Water will carry
  // the frequency generator signal back to the receiver.
  bool currentState = distance > 0;
  
  // Print the state to the display
  if (currentState) {
    Display.print(4, "Water Flow Detected!");
  } else {
    Display.print(4, "");
  }

  // Send a message to the server if the state has changed or if the initial message has not been sent
  if ((currentState != previousState) || !initialMessageSent) {
    initialMessageSent = true;
    //send message to server
    String jsonMessage = "{\"WaterFlow\": " + String(currentState ? "true" : "false") + "}";
    Serial.println("Sending message to MQTT: " + jsonMessage);
    sendMessageToMQTT(MQTT_TOPIC, jsonMessage.c_str());
  }

  // Update the previous state
  previousState = currentState;

  // Wait 2 seconds
  delay(SLEEP_INTERVAL);
  
}
