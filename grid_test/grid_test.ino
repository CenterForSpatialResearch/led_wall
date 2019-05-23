#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 6;
const int PIXELS_PER_ROW = 5;
const int ROWS = 5;

const int PIXELS = PIXELS_PER_ROW * ROWS;
const int LEDS_PER_ROW = (((PIXELS_PER_ROW - 1) * 4) + 1) * ROWS;

Adafruit_NeoPixel strip(LEDS_PER_ROW * ROWS, DATA_PIN, NEO_GRB + NEO_KHZ800);

uint32_t color_1 = strip.Color(255, 0, 100);
uint32_t color_2 = strip.Color(0, 100, 255);
uint32_t off_color = strip.Color(0, 0, 0);

int pixel;
int led;
int r;
uint32_t current_color = color_1;

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

//// sanity test
//void loop() {
//  for(int i=0; i<PIXELS; i++) {
//    int pixel = i;//random(0, PIXELS);
//    setPixel(pixel, current_color);
//    strip.show();
//    delay(300);
//  }
//  current_color = current_color == color_1 ? color_2 : color_1;
//}

void loop() {
  pixel = random(0, PIXELS);
  r = random(0, 4);
  if (r == 0) {
    setPixel(pixel, color_1);
  } else if (r == 1) {
    setPixel(pixel, color_2);  
  } else if (r == 2) {
    setPixel(pixel, off_color);
  } else {
    
  }
  strip.show();
  delay(random(10, 100));
}

void setPixel(int pixel, uint32_t color) {
  led = (pixel * 4) - (floor(pixel / PIXELS_PER_ROW) * 3);
  strip.setPixelColor(led, color);
}

