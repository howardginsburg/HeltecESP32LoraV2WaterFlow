#include "Arduino.h"
#include "heltec.h"
#include "DisplayUtils.h"
#include "WifiUtils.h"
#include "MQTTClient.h"
#include <NewPing.h>

// Pin definitions for the ultrasonic sensor
#define TRIGGER_PIN  17  // Arduino pin tied to the trigger pin on the ultrasonic sensor.
#define ECHO_PIN     2   // Arduino pin tied to the echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance to measure (in centimeters). Sensor max range is 400-500cm.

// Create a NewPing instance for the ultrasonic sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Variables to track water flow state
bool previousState = false;      // Tracks the previous state of water flow
bool initialMessageSent = false; // Ensures the initial MQTT message is sent

/**
 * @brief Arduino setup function.
 * 
 * This function initializes the serial communication, sets up the built-in LED pin,
 * and initializes the display.
 */
void setup() {
  Serial.begin(115200);         // Start serial communication at 115200 baud
  pinMode(LED_BUILTIN, OUTPUT); // Set the built-in LED pin as an output
  Display.init();               // Initialize the display
}

/**
 * @brief Arduino loop function.
 * 
 * This function continuously measures the distance using the ultrasonic sensor,
 * determines if water flow is detected, updates the display, and sends MQTT messages
 * when the state changes or on the first run.
 */
void loop() {
  // Initialize WiFi connection
  initWifi();
  
  // Measure the distance using the ultrasonic sensor
  unsigned int uS = sonar.ping(); // Get ping time in microseconds
  unsigned int distance = uS / US_ROUNDTRIP_CM; // Convert ping time to distance in cm

  // Print the measured distance to the serial monitor and display
  Serial.println("Measurement: " + String(distance) + "cm");
  Display.print(3, "Measurement: " + String(distance));

  // Determine if water flow is detected (distance > 0 indicates water flow)
  bool currentState = distance > 0;
  
  // Update the display with the water flow status
  if (currentState) {
    Display.print(4, "Water Flow Detected!");
  } else {
    Display.print(4, "");
  }

  // Send an MQTT message if the state has changed or if the initial message has not been sent
  if ((currentState != previousState) || !initialMessageSent) {
    initialMessageSent = true; // Mark the initial message as sent
    // Create a JSON message with the water flow status
    String jsonMessage = "{\"WaterFlow\": " + String(currentState ? "1" : "0") + "}";
    Serial.println("Sending message to MQTT: " + jsonMessage);
    sendMessageToMQTT(MQTT_TOPIC, jsonMessage.c_str()); // Send the message to the MQTT broker
  }

  // Update the previous state to the current state
  previousState = currentState;

  // Wait for a specified interval before the next measurement
  delay(SLEEP_INTERVAL);
}