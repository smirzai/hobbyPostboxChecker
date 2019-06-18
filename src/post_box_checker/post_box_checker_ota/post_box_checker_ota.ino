  #include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#include "passwords.h"

const int infraredLedPin = 16;
const int infraredLedFreq = 38000;
const int infraredLedChannel = 0;
const int infraredLedResolution = 8;

long counter = 0;
long counter2 = 0;
boolean toggle = 1;
boolean toggle2 = 1;

#define LED_BUILTIN 5
#define SENSOR_IN_PIN 17


hw_timer_t * timer = NULL;

volatile long infrared_total_down_time;
volatile long infrared_down_timestamp;


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

boolean result = 0;

void IRAM_ATTR turnOffLeds() {
   ledcWrite(infraredLedChannel, 0);
   detachInterrupt(SENSOR_IN_PIN);
   digitalWrite(LED_BUILTIN, infrared_total_down_time > 10000);
   
   Serial.println(infrared_total_down_time);
 
}




void IRAM_ATTR infrared_change() {
  if (digitalRead(SENSOR_IN_PIN)) {
     infrared_down_timestamp = esp_timer_get_time();
    
  } else {
    infrared_total_down_time += esp_timer_get_time() - infrared_down_timestamp;
    
    
  }
  
  
  
}
 
 
void isOneLedBlocked() {
  
// initialize time 0, prescale 80, count up, each tick is 1 microsecond
   timer = timerBegin(0, 80, true);
   
   timerAttachInterrupt(timer, &turnOffLeds, true);

  

   // set timer for 5 ms, to turn off led signalling
   timerAlarmWrite(timer, 5000, false);
   
infrared_total_down_time += 10;
    // turn on led singaling
   ledcWrite(infraredLedChannel, 128);
   timerAlarmEnable(timer);
   infrared_total_down_time = 0;
   infrared_down_timestamp = 0;
   
   attachInterrupt(SENSOR_IN_PIN, infrared_change, CHANGE);


  
}


void IRAM_ATTR periodicEvent() {
  
   isOneLedBlocked();
 
}


void setup_timers() {
   // set inner led timer
   
   

   // set timer 2, cover timer
   hw_timer_t * timer2 = NULL;

   // initialize time 0, prescale 80, count up, each tick is 1 microsecond
   timer2 = timerBegin(1, 80, true);
   timerAttachInterrupt(timer2, &periodicEvent, true);


   // set timer for 5 ms, to turn off led signalling
   timerAlarmWrite(timer2, 100000, true);
   
   timerAlarmEnable(timer2);
  
}

void setup() {

  setup_ota();
  
   // configure LED PWM functionalitites
  ledcSetup(infraredLedChannel, infraredLedFreq, infraredLedResolution);

     // attach the channel to the GPIO to be controlled
  ledcAttachPin(infraredLedPin, infraredLedChannel);


  pinMode(SENSOR_IN_PIN, INPUT_PULLUP);

  
    
  ledcWrite(infraredLedChannel, 0);
  
 
  pinMode(LED_BUILTIN, OUTPUT);
 
 
 
  setup_timers();


}

 
void loop() {
  ArduinoOTA.handle();

   
  

   
}
