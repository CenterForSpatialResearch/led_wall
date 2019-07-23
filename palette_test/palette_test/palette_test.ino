#include <Adafruit_NeoPixel.h>

const int DATA_PIN = 6;
const int PIXELS_PER_ROW = 15;
const int ROWS = 15;

const int PIXELS = PIXELS_PER_ROW * ROWS;
const int LEDS_PER_ROW = (((PIXELS_PER_ROW - 1) * 4) + 1);

Adafruit_NeoPixel strip(LEDS_PER_ROW * ROWS, DATA_PIN, NEO_GRB + NEO_KHZ800);

//

const uint32_t white = strip.Color(255, 255, 255);

const uint32_t red = strip.Color(255, 0, 0);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t green = strip.Color(0, 255, 0);

const uint32_t yellow = strip.Color(255, 255, 0);
const uint32_t cyan = strip.Color(0, 255, 255);
const uint32_t purple = strip.Color(255, 0, 255);

const uint32_t mod_blue = strip.Color(50, 100, 255);
const uint32_t mod_red = strip.Color(255, 50, 100);



const uint32_t COLORS[] = {   

//                              blue, cyan,
//
//                              red, mod_red,
//                              red, purple,
//                              red, yellow,                              
//                              red, mod_blue,
//                              yellow, white,                              
//                              yellow, cyan,
//                              blue, cyan,
//                              green, cyan

                                // all secondary
//                                white, cyan,
//                                white, purple,
//                                cyan, yellow,
//                                white, yellow,
//                                cyan, purple,
////                                yellow, purple

//                                // all whites
//                                white, blue,
//                                white, green,
//                                white, red,
//                                white, yellow,
//                               white, cyan,
//                                                                    

//                                // all yellows
//                                yellow, blue,
//                                yellow, green,
//                                yellow, red,
//                                white, yellow,
//                               yellow, cyan,

//                               // cyan
//                               purple, cyan,
//                                cyan, blue,
//                                cyan, green,
//                                cyan, red,
//                                white, cyan,
//                               yellow, cyan,



                                // combos
                                white, mod_blue,                                
                                mod_blue, cyan, 
                                cyan, red,
                                red, yellow,
                                yellow, white, 

                                                          
                              
                          };                         
const uint32_t OFF_COLOR = strip.Color(0, 0, 0);
uint32_t COLOR_LENGTH = sizeof(COLORS) / sizeof(COLORS[0]);

uint8_t sequence[PIXELS];

void setup() {
  Serial.begin(19200);
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

  for (uint8_t pixel=0; pixel<PIXELS; pixel++) {
      sequence[pixel] = pixel;
  }
  shuffleSequence(sequence);  
  
}

int cycle = 0;
int index = 0;

void loop() {
    if (index == PIXELS) {
      delay(5000);
      index = 0;
      cycle += 2;
      cycle %= COLOR_LENGTH;
      strip.clear();
    } else {
      uint8_t pixel = sequence[index++];

      int row = floor(pixel / PIXELS_PER_ROW);
      int column = pixel - (row * PIXELS_PER_ROW);     

      Serial.print(row);
      Serial.print(",");
      Serial.println(column);

      int rotation = 0;

//      if (row < 8) {
//        if (column < 8) {
//          rotation += 0;
//        } else {
//          rotation += 2;
//        }
//      } else {
//        if (column < 8) {
//          rotation += 4;
//        } else {
//          rotation += 6;
//        }
//      }
//      
      uint8_t choice = random(0, 2);      
      uint32_t c = COLORS[(cycle + rotation + choice) % COLOR_LENGTH];
      setColor(pixel, c);
    }
    delay(20);
    strip.show();
}


void setColor(uint8_t pixel, uint32_t color) { 
    int row = floor(pixel / PIXELS_PER_ROW);
    int column = pixel - (row * PIXELS_PER_ROW);     
    column = row % 2 > 0 ? (PIXELS_PER_ROW - 1) - column : column;    // flip odd rows
    pixel = (row * PIXELS_PER_ROW) + column;                          // adjusted pixel
    int led = (pixel * 4) - (floor(pixel / PIXELS_PER_ROW) * 3);  
    strip.setPixelColor(led, strip.gamma32(color));  
//    strip.setPixelColor(led, color);  
}


void shuffleSequence(uint8_t a[]) {
    uint8_t temp;
    uint8_t r;
    for (uint8_t i=0; i<PIXELS; i++) {
        temp = a[i];
        r = random(PIXELS);
        a[i] = a[r];
        a[r] = temp;
    }
}
