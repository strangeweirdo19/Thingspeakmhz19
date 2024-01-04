#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "MHZ19.h"


MHZ19 myMHZ19;
WiFiClient  client;

char ssid[] = "Airtel-MyWiFi-AMF-311WW-EB1E";   // your network SSID (name)
char pass[] = "2163f324";   // your network password
int keyIndex = 0; // your network key Index number WiFiClient client;
unsigned long myChannelNumber = 1925600;
const char * myWriteAPIKey = "CF6OI6X26XZWICIK";

void setup() {
  Serial.begin(9600);  // Initialize serial
  myMHZ19.begin(Serial);  // *Serial(Stream) refence must be passed to library begin().
  myMHZ19.setAutoCalibration(false);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) { // Connect or reconnect to WiFi
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network.
      Serial.print(".");
      delay(500);
    }
    Serial.println("\nConnected.");
  }
  int CO2 = myMHZ19.getPpmPwm();
  ThingSpeak.setField(1, CO2);

  if (ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey) == 200)
    Serial.println("Channel update successful.");
  else
    Serial.println("Problem updating channel. HTTP error code ");
  delay(100); // Wait 60 seconds to update the channel again
}
