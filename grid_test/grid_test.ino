#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 6;
const int PIXELS_PER_ROW = 15;
const int ROWS = 8;

const int PIXELS = PIXELS_PER_ROW * ROWS;
const int LEDS_PER_ROW = (((PIXELS_PER_ROW - 1) * 4) + 1);

Adafruit_NeoPixel strip(LEDS_PER_ROW * ROWS, DATA_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t COLOR_1 = strip.Color(255, 0, 50);
const uint32_t COLOR_2 = strip.Color(0, 100, 255);
const uint32_t COLORS[] = {COLOR_1, COLOR_2};
const uint32_t OFF_COLOR = strip.Color(0, 0, 0);

int pixel = 0;

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
void loop() {
  if (pixel < PIXELS) {
    setColor(pixel, COLORS[pixel % 2]);
    pixel++;
    if (pixel == PIXELS) {
      Serial.println("DONE");
    }
  }
  strip.show();
  delay(50);
}

void setColor(int pixel, uint32_t color) {
  Serial.print(pixel);
  Serial.print(": ");

  int row = floor(pixel / PIXELS_PER_ROW);
  int column = pixel - (row * PIXELS_PER_ROW);
  Serial.print(row);
  Serial.print("x");
  Serial.print(column);
  Serial.print("->");
  column = row % 2 > 0 ? (PIXELS_PER_ROW - 1) - column : column;
  Serial.print(column);
  
  pixel = (row * PIXELS_PER_ROW) + column;
  int led = (pixel * 4) - (floor(pixel / PIXELS_PER_ROW) * 3);
  Serial.print(" (");
  Serial.print(led);
  Serial.println(")");
  strip.setPixelColor(led, color);
}
