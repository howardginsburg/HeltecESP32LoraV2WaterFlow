#include "MQTTClient.h"
#include <WiFi.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void initializeMQTT() {
    
    client.setServer(MQTT_SERVER, MQTT_PORT);

    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect(MQTT_CLIENT, MQTT_USER, MQTT_PASSWORD)) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void sendMessageToMQTT(const char* topic, const char* message) {
    if (!client.connected()) {
        initializeMQTT();
    }
    client.loop();
    client.publish(topic, message);
    //Flash the LED to indicate a message was sent
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}