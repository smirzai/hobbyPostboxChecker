  #include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "passwords.h"


const int infraredLedFreq = 38000;
const int infraredLedChannel = 0;
const int infraredLedResolution = 8;


#define LED_BUILTIN 5
#define SENSOR_IN_PIN 17
#define INFRARED_LED_PIN  16


void setup_ota() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void setup() {
  setup_ota();
  
   // configure LED PWM functionalitites
 // ledcSetup(infraredLedChannel, infraredLedFreq, infraredLedResolution);
 //  pinMode(SENSOR_IN_PIN, INPUT_PULLUP);

  
    
   // attach the channel to the GPIO to be controlled
 // ledcAttachPin(infraredLedPin, infraredLedChannel);
 // ledcWrite(infraredLedChannel, 0);
  
 
 // pinMode(LED_BUILTIN, OUTPUT);
  
  // hw_timer_t * timer = NULL;
  //timer = timerBegin(0, 80, true);

 
 // checkMail();

}

volatile boolean result = 0;

int toggle = 0;
void IRAM_ATTR onTimer() {
  toggle = ! toggle;
  digitalWrite(LED_BUILTIN, toggle);
  ledcWrite(infraredLedChannel, 128 * toggle) ;
 
}

// this function checks if any of the infrared ways are blocked
// returns 1 if there is mail, 0 otherwise
int checkMail() {
  hw_timer_t * timer = NULL;
 // timer = timerBegin(0, 80, true);
  //timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, 100000, true);
  //timerAlarmEnable(timer);
  
}


void loop() {
  ArduinoOTA.handle();
 
 
  
}
