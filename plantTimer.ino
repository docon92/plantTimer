#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

bool LED_ON = false;

const long utcOffsetInSeconds = -18000;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
    pinMode(14, OUTPUT);     //  pin 14 is D5 on WeMos D1 mini
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(14, HIGH);
  
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    res = wm.autoConnect("PlantTimer", "PlantTimer"); // anonymous ap
    // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected to network...)");
        digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (Note that LOW is the voltage level
        digitalWrite(14, LOW);
    }

  timeClient.begin();
}

// the loop function runs over and over again forever
void loop() {

  timeClient.update();

//  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  int hourNow = timeClient.getHours();
  if(hourNow<20 && hourNow>=6){
    LED_ON = true;
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(14, HIGH);
    Serial.println("Turning on Lights!");
  }
  else{
    LED_ON = false;
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(14, LOW);// Turn the LED off by making the voltage HIGH
    Serial.println("Turning off Lights!");
  }
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
