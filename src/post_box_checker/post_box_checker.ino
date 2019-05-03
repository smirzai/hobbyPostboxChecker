const int infraredLedPin = 16;
const int infraredLedFreq = 5000;
const int infraredLedChannel = 0;
const int infraredLedResolution = 8;
 
void setup() {

   // configure LED PWM functionalitites
  ledcSetup(infraredLedChannel, infraredLedFreq, infraredLedResolution);
   // attach the channel to the GPIO to be controlled
  ledcAttachPin(infraredLedPin, infraredLedChannel);

}

void loop() {
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(infraredLedChannel, dutyCycle);
    delay(15);
  }

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(infraredLedChannel, dutyCycle);   
    delay(15);
  }

}
