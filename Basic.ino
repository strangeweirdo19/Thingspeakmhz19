#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

#include "ThingSpeak.h"
WiFiClient client;

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "REM7WWSLLVHPU0H9";
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

void setup() {
  // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP", "password");  // password protected ap

  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    ThingSpeak.begin(client);  // Initialize ThingSpeak
  }
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if (Serial.available()) {
      int aqi =  Serial.parseInt();
      // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
      // pieces of information in a channel.  Here, we write to field 1.
      int x = ThingSpeak.writeField(myChannelNumber, 2, aqi, myWriteAPIKey);
      //uncomment if you want to get temperature in Fahrenheit
      //int x = ThingSpeak.writeField(myChannelNumber, 1, temperatureF, myWriteAPIKey);
      if (x == 200) {
        Serial.println("Channel update successful.");
      } else {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
      lastTime = millis();
    }
  }
}
