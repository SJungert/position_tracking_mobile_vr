#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
extern "C" {
#include<user_interface.h>
}
const char* ssid = "loading...";
const char* password = "password";
//xString incomingData;
// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
int messageSize;

void setup() {
    Serial.begin(256000);
    delay(10);
    messageSize = 32;
    // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Udp.beginPacket("192.168.0.0", 1024);
}

void loop(){
    String incomingData = Serial.readStringUntil('~');
    //if (incomingData.length() != messageSize)
    //return;
    Serial.println(incomingData);
    //Udp.beginPacket("192.168.0.0", 1024);
    Serial.println("sending Packet");
    //Udp.print(incomingData);
    //Udp.endPacket();
    Udp.beginPacket("192.168.0.0", 1024);
    Udp.print(incomingData);
    Udp.endPacket();
    Serial.println("Packet Sent");
}