#include "Arduino.h"

#include "FastLED.h"
FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN 3
#define CLK_PIN 4
#define LED_TYPE WS2801
#define COLOR_ORDER RGB
#define NUM_LEDS 50
CRGB leds[NUM_LEDS];

#define EYE_SET_COUNT 10
long eyeDelays[EYE_SET_COUNT];
uint8_t eyeColor[EYE_SET_COUNT];

#define BRIGHTNESS 200

unsigned long now;

void setup() {
  delay(3000); // 3 second delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(BRIGHTNESS);
  
  fill_solid(leds, NUM_LEDS, 0x000000);
  FastLED.show();
  delay(3000);

  now = millis();
  for(uint8_t i=0; i<EYE_SET_COUNT; i++) {
      eyeColor[i] = 1;
      eyeDelays[i] = now + 1000;
    }
  FastLED.show();
}


void loop() {
  now = millis();
  for(uint8_t i=0; i<EYE_SET_COUNT; i++) {
    if(eyeDelays[i] < now) {
      CRGB c = 0xFF0000;
      if(eyeColor[i] == 1) {
        eyeColor[i] = 0;
        eyeDelays[i] = now + random16(2000,6000);
        c = 0x000000;
      }
      else {
        eyeColor[i] = 1;
        eyeDelays[i] = now + random16(200,3000);
      }
      leds[i*5] = c;
      leds[i*5+1] = c;
    }
  }
  FastLED.show();
}

