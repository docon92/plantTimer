    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.aut

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "MTL_Wifi";
const char* password = "porticoculture";

bool LED_ON = false;

const long utcOffsetInSeconds = -18000;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    Serial.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
//  while (WiFi.status() != WL_CONNECTED){
//    delay (500);
//    Serial.print(".");
//  }

  timeClient.begin();
  
  
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(14, OUTPUT);
  
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
  if(!LED_ON && hourNow<20 && hourNow>6){
    LED_ON = true;
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    digitalWrite(14, HIGH);
    Serial.println("Turning on Lights!");
  } else if (LED_ON && (hourNow>20 || hourNow<6) ){
    LED_ON = false;
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(14, LOW);// Turn the LED off by making the voltage HIGH
    Serial.println("Turning off Lights!");
  }
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
