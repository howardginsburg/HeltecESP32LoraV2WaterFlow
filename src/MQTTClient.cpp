#include "MQTTClient.h"
#include <WiFi.h>

// Create a WiFi client instance
WiFiClient wifiClient;

// Create an MQTT client instance using the WiFi client
PubSubClient client(wifiClient);

/**
 * @brief Initializes the MQTT connection.
 * 
 * This function sets up the MQTT server and attempts to connect to it.
 * It retries up to a maximum of 5 times if the connection fails.
 * If the connection is not established after 5 attempts, it logs an error message.
 */
void initializeMQTT() {
    // Set the MQTT server and port
    client.setServer(MQTT_SERVER, MQTT_PORT);

    int retryCount = 0;          // Counter for connection attempts
    const int maxRetries = 5;   // Maximum number of retries

    // Attempt to connect to the MQTT broker
    while (!client.connected() && retryCount < maxRetries) {
        Serial.print("Connecting to MQTT...");
        if (client.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("connected"); // Connection successful
        } else {
            // Log the failure and retry
            Serial.print("failed with state ");
            Serial.print(client.state());
            Serial.println(". Retrying...");
            retryCount++;
            delay(2000); // Wait 2 seconds before retrying
        }
    }

    // Log an error if the connection could not be established
    if (!client.connected()) {
        Serial.println("Failed to connect to MQTT broker after 5 attempts.");
    }
}

/**
 * @brief Sends a message to a specified MQTT topic.
 * 
 * This function ensures the MQTT client is connected before attempting to send a message.
 * If the client is not connected, it attempts to reconnect using `initializeMQTT`.
 * If the connection is still not established, it logs an error and exits.
 * 
 * @param topic The MQTT topic to publish the message to.
 * @param message The message to publish.
 */
void sendMessageToMQTT(const char* topic, const char* message) {
    // Check if the MQTT client is connected
    if (!client.connected()) {
        initializeMQTT(); // Attempt to reconnect
    }

    // Ensure the client is connected before publishing
    if (!client.connected()) {
        Serial.println("MQTT client not connected. Cannot send message.");
        return; // Exit if the client is not connected
    }

    // Process incoming messages and maintain the connection
    client.loop();

    // Publish the message to the specified topic setting retain to true.
    client.publish(topic, message, true);

    // Flash the built-in LED to indicate a message was sent
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}