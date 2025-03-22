#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <PubSubClient.h>

void sendMessageToMQTT(const char* topic, const char* message);

#endif // MQTTCLIENT_H