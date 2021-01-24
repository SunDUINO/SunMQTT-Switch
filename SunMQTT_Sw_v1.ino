/*
 Name:		SunMQTT_Sw_v1.ino
 Created:	21.01.2021 19:54:30
 Author:	sunriver
 *****************************************************************************************************************
 ** ESP - 03  module
 ** ESP8266Ex
 ** Chip ID-- 0x1340C8  GD25Q40BTIG 4M-- 512KB
 *****************************************************************************************************************
 *  PK1 connect GPIO13
 *  PK2 connect GPIO12
 *  ---   Aktive  HIGH Signal
 *****************************************************************************************************************
*/

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "Wifi pasword"

char mqttServer[] = "IP or url"; 
int mqttPort = 1883; 
char deviceId[] = "SunMQTT_Sw"; 
char topic1[] = "subTopic"; 
char topic2[] = "pubTopic";
char mqttUsr[] = "mqtt_user_name";
char mqttPwd[] = "mqtt_broker_pwd";
char sts1[] = "ON";
char sts2[] = "OFF";
WiFiClient wifiClient;

PubSubClient client(wifiClient);

int status = WL_IDLE_STATUS;
 

void setup() 
{
    Serial.begin(115200);
    delay(10);
    pinMode(13, OUTPUT);     
    pinMode(12, OUTPUT);
    
}

void loop() 
{
    if (!client.connected()) 
    { 
       connect();
    }
    client.loop();
}

unsigned long lstOps; //last MQTT Message
const long MIN_OPERATION_INTERVAL = 2000L;

const String onString = "LightON";
const String offString = "LightOFF";

void callback(char* topic1, byte* payload, unsigned int length) {
    signed long now = millis();
    long deltaTime = now - lstOs;
    if (deltaTime > MIN_OPERATION_INTERVAL) {
        String message = "";
        for (int i = 0; i < length; i++) {
            message = message + (char)payload[i];
        }
        if (message == onString) 
        {
            digitalWrite(12, HIGH);
            client.publish(topic2, sts1);
            lstOps = now;
        }
        else if (message == offString) 
        {
            digitalWrite(12, LOW);
            client.publish(topic2, sts2);
            lstOps = now;
        }
    }
}

void connect() 
{
    while (!client.connected()) 
    {
        status = WiFi.status();
        if (status != WL_CONNECTED) {
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            while (WiFi.status() != WL_CONNECTED) 
            {
                delay(500);
                // serial debug 
                Serial.print(".");
            }
            Serial.print("Connected to ");
            Serial.println(WIFI_SSID);
        }
        client.setServer(mqttServer, mqttPort);
        client.setCallback(callback);
        if (client.connect(deviceId, mqttUsr, mqttPwd)) {
            client.subscribe(topic1); 
            Serial.println("Connected to MQTT");
        }
        else {
            Serial.print("[FAILED] [ rc = ");
            Serial.print(client.state());
            Serial.println(" : retrying wait 5 sec]");
            delay(5000);
        }
    }
}
