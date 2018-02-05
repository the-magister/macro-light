#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <Streaming.h>
#include <Metro.h>
#include <EEPROM.h>

#define PIN 6
#define NUM_LEDS 60
#define BRIGHTNESS 50

typedef struct {
  uint8_t red,green,blue,white;
  uint8_t bright;
} EEPROMstruct;

EEPROMstruct settings;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

void getEEPROM() {
  EEPROM.get(0, settings);
  showSettings();
}
void setEEPROM() {
  EEPROM.put(0, settings);
  showSettings();
}

void showSettings() {
  
  strip.setBrightness(
    neopix_gamma[settings.bright]
  );
  
  showStrip(
    strip.Color(
      neopix_gamma[settings.red], 
      neopix_gamma[settings.green], 
      neopix_gamma[settings.blue],
      neopix_gamma[settings.white]
    )
  ); 
  
  Serial << "r:" << settings.red;
  Serial << "g:" << settings.green;
  Serial << "b:" << settings.blue;
  Serial << "w:" << settings.white;
  Serial << "B:" << settings.bright;
  Serial << endl;
}

void setup() {
  Serial.begin(115200);
  
  strip.begin();
  getEEPROM();
}

void loop() {

  if( Serial.available() ) {
    delay(20); // wait for the rest
    
    // have some commands
    char key = Serial.read(); // must be 'r','g','b','w','b'
    byte val = Serial.parseInt();
    switch( key ) {
      case 'r': settings.red = val; showSettings(); break;
      case 'g': settings.green = val; showSettings(); break;
      case 'b': settings.blue = val; showSettings(); break;
      case 'w': settings.white = val; showSettings(); break;
      case 'B': settings.bright = val; showSettings(); break;
      default:
        Serial << "Unknown command.  Try r,g,b,w,B followed by a byte.  E.g: r128" << endl;
    }
    
  }
  strip.show();
}

// Fill the dots one after the other with a color
void showStrip(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
}

