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
#define NUM_LEDS 100
#define SKIP_DISTANCE 8
#define BRIGHTNESS 254
#define FADE_OFF_STEP 5
#define FADE_ON_STEP 5
  

#define EYE_SET_COUNT 17
long eyeDelays[EYE_SET_COUNT];
uint8_t eyeState[EYE_SET_COUNT];
CRGB eyeSetColor[EYE_SET_COUNT];

CRGB leds[NUM_LEDS];

void handleEyes();

unsigned long now;

class Eyes {
  public:
    uint8_t state;
    uint8_t hue;
    uint8_t index;
    int brightness;
    long durrationInState;
};

Eyes eyes[EYE_SET_COUNT];

//Cylon - LED sweeps back and forth, with the color, delay and number of cycles of your choice 
void cylon(CRGB color, uint16_t wait, uint8_t number_of_cycles)
{
	FastLED.setBrightness(255);
	for (uint8_t times = 0; times<=number_of_cycles; times++)
	{
		// Make it look like one LED is moving in one direction
		for(int led_number = 0; led_number < NUM_LEDS; led_number++)
		{
			//Apply the color that was passed into the function
			leds[led_number] = color;
			//Actually turn on the LED we just set
			FastLED.show();
			// Turn it back off
			leds[led_number] = CRGB::Black;
			// Pause before "going" to next LED
			delay(wait);
		}

		// Now "move" the LED the other direction
		for(int led_number = NUM_LEDS-1; led_number >= 0; led_number--)
		{
			//Apply the color that was passed into the function
			leds[led_number] = color;
			//Actually turn on the LED we just set
			FastLED.show();
			// Turn it back off
			leds[led_number] = CRGB::Black;
			// Pause before "going" to next LED
			delay(wait);
		}
	}
	return;
}

void setEyes(Eyes e);

void setup() {
  delay(3000); // 3 second delay for recovery
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
  
  cylon(CRGB::Red,25, 2);
  // fill_rainbow(leds, NUM_LEDS, 0);
  // FastLED.show();
  // delay(1000); // 1 second strand test...

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(1000);
  
  for (uint8_t i=0; i < EYE_SET_COUNT; i++) {
    eyes[i].state = 0; // on
    eyes[i].hue = 16; // orange
    eyes[i].index = i * SKIP_DISTANCE; // first led
    eyes[i].durrationInState = millis() + 1000; // 10 seconds
  }
}


void loop() {
  handleEyes();
  FastLED.show();
  // delay(1);
}

void handleEyes() {
  for (uint8_t i=0; i < EYE_SET_COUNT; i++) {
    now = millis();
    if (now > eyes[i].durrationInState) {
      switch(eyes[i].state){
        case 0 : // fade on > on
          eyes[i].durrationInState = random8(8) == 1 ? now + 200 : now + random16(200, 3000);
          eyes[i].state = 1;
          break;
        case 1 : // on > fade off
          if (eyes[i].brightness - FADE_OFF_STEP < 0) {
            eyes[i].state = 2;
            eyes[i].brightness = 0;
          }
          else {
            eyes[i].brightness -= FADE_OFF_STEP;
          }
          break;        
        case 2 : // fade off > off
          eyes[i].durrationInState = random(8) == 1 ? now + 15000 : now + random16(2000, 6000);
          eyes[i].state = 3;
          break;
        case 3 : // off > fade on
          if (eyes[i].brightness + FADE_ON_STEP > 254) {
            eyes[i].state = 0;
            eyes[i].brightness = 254;
          }
          else {
            eyes[i].brightness += FADE_ON_STEP;
          }
          break;
        
      }
      setEyes(eyes[i]);
    }
  }
}

void setEyes(Eyes e) {
  leds[e.index] = CHSV(e.hue, 255, e.brightness);
  leds[e.index + 1] = CHSV(e.hue, 255, e.brightness);
}

