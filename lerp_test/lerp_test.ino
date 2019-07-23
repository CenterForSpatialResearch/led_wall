#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 6;
const int PIXELS_PER_ROW = 15;
const int ROWS = 15;

const int PIXELS = PIXELS_PER_ROW * ROWS;
const int LEDS_PER_ROW = (((PIXELS_PER_ROW - 1) * 4) + 1);

Adafruit_NeoPixel strip(LEDS_PER_ROW * ROWS, DATA_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t COLOR_1 = strip.Color(255, 0, 50);
const uint32_t COLOR_2 = strip.Color(0, 100, 255);
const uint32_t COLORS[] = {COLOR_1, COLOR_2};
const uint32_t OFF_COLOR = strip.Color(0, 0, 0);

void setup() {
  Serial.begin(19200);
//  Serial.print("DATA_PIN ");  
//  Serial.println(DATA_PIN);  
  Serial.print("PIXELS_PER_ROW ");  
  Serial.println(PIXELS_PER_ROW);  
  Serial.print("ROWS ");  
  Serial.println(ROWS);  
  Serial.print("PIXELS ");  
  Serial.println(PIXELS); 
  Serial.print("LEDS_PER_ROW ");  
  Serial.println(LEDS_PER_ROW);    
  Serial.print("TOTAL LEDS ");
  Serial.println(strip.numPixels());
    
  strip.begin();
  strip.show();
  strip.setBrightness(255);
}

// sanity test
int index = 0;

void loop() {
  float pos = index % 100 / 100.0;
  Serial.println(pos);
  for (int pixel=0; pixel<PIXELS; pixel++) {
    uint32_t c = lerpColor(COLORS[0], OFF_COLOR, pos);   
    setColor(pixel, c);
  }
  index++;
  strip.show();
  delay(50);
}


uint32_t lerpColor(uint32_t current, uint32_t target, float pos) {
    // yes, linear RGB fade isnt correct, but it's used as a very brief effect
    float current_red = (uint8_t)(current >> 16);
    float target_red = (uint8_t)(target >> 16);
    float current_green = (uint8_t)(current >> 8);
    float target_green = (uint8_t)(target >> 8);
    float current_blue = (uint8_t)(current);
    float target_blue = (uint8_t)(target);
    float result_red = ((current_red * (1 - pos)) + (target_red * pos));
    float result_green = ((current_green * (1 - pos)) + (target_green * pos));
    float result_blue = ((current_blue * (1 - pos)) + (target_blue * pos));
    return strip.Color(result_red, result_green, result_blue);
}

void setColor(uint8_t pixel, uint32_t color) { 
    int row = floor(pixel / PIXELS_PER_ROW);
    int column = pixel - (row * PIXELS_PER_ROW);     
    column = row % 2 > 0 ? (PIXELS_PER_ROW - 1) - column : column;    // flip odd rows
    pixel = (row * PIXELS_PER_ROW) + column;                          // adjusted pixel
    int led = (pixel * 4) - (floor(pixel / PIXELS_PER_ROW) * 3);  
    strip.setPixelColor(led, strip.gamma32(color));  
}
