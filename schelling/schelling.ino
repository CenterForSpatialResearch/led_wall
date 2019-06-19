#include <Adafruit_NeoPixel.h>

// settable params
const uint8_t PIXELS_PER_ROW = 15;
const uint8_t ROWS = 15;
const uint8_t TYPES = 2;

const uint8_t DATA_PIN = 6;
const int DELAY = 50;
const int MAX_STEPS = 1000;
const uint8_t COUNTDOWN = 10;

// dependent constants
const uint8_t PIXELS = PIXELS_PER_ROW * ROWS;
const int LEDS_PER_ROW = (((PIXELS_PER_ROW - 1) * 4) + 1);
const uint8_t POPULATION = floor(PIXELS / 2);
const uint8_t NONE = 255; // max value for uint8_t, restricts this to the 15x15 grid

Adafruit_NeoPixel strip(LEDS_PER_ROW * ROWS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// colors
const uint32_t COLOR_1 = strip.Color(255, 10, 25);
const uint32_t COLOR_2 = strip.Color(0, 100, 255);
const uint32_t COLORS[] = {COLOR_1, COLOR_2};
const uint32_t OFF_COLOR = strip.Color(0, 0, 0);

// states
const uint8_t STARTUP = 0;
const uint8_t INTRO = 1;
const uint8_t PLAY = 2;
const uint8_t HOLD = 3;
const uint8_t CODA = 4;
const uint8_t STOP = 5;

// declare persistant variables
uint8_t state;
uint8_t index;
int steps;
uint8_t countdown;

// allocate arrays
uint8_t NEIGHBORS[PIXELS][8];
uint32_t pixel_colors[PIXELS];
uint32_t previous_pixel_colors[PIXELS];
uint8_t sequence[PIXELS];
uint8_t transitions[PIXELS];

// p5 specific initialization
void setup() {
    Serial.begin(19200);
    Serial.println("setup()");
    randomSeed(analogRead(0));
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
    strip.setBrightness(255);
    initNeighbors();
    reset();
    state = STARTUP;
    Serial.println("STARTUP");
}

// reset process
void reset() {
    Serial.println("reset()");
    index = 0;
    steps = 0;   
    countdown = COUNTDOWN;
    resetColors();
    for (uint8_t t=0; t<PIXELS; t++) {
        transitions[t] = NONE;
    }    
    for (uint8_t s=0; s<PIXELS; s++) {
        sequence[s] = s;
    }
    shuffleSequence(sequence);
}

void loop() {

    if (state == STOP) {
      
    } else if (state == STARTUP) {
        uint8_t pixel = index++;
        setColor(pixel, COLORS[pixel % 2]);
        if (index > PIXELS) { // == will skip last pixel w/ reset
            reset();
            state = INTRO;
            Serial.println("INTRO");      
        }
    }  

    else if (state == INTRO) {
        uint8_t pixel = sequence[index++];
        setColor(pixel, COLORS[index % TYPES]);
        if (index == POPULATION) {
            state = STOP;
            state = PLAY;    
            Serial.println("PLAY");
        }
    } 

    else if (state == PLAY) {
        while (true) {
            if (steps == MAX_STEPS) {
                // console.log("hit max");
                Serial.print("free memory: ");
                Serial.println(freeMemory());
                state = HOLD;
                Serial.println("HOLD");
                break;
            }
            uint8_t pixel = sequence[index++];
            index %= PIXELS;
            if (getColor(pixel) == OFF_COLOR) {
                continue;
            }
            uint8_t happiness = calcHappiness(pixel, NEIGHBORS[pixel]);
            uint8_t happiest_neighbor = NONE;
            uint8_t max_happiness = 0;
            uint8_t offset = random(0, 8);            
            for (uint8_t j=0; j<8; j++) {
                uint8_t n = (j + offset) % 8;
                if (NEIGHBORS[pixel][n] == NONE) {
                    continue;
                }
                if (getColor(NEIGHBORS[pixel][n]) == OFF_COLOR) {
                    uint8_t n_happiness = calcHappiness(pixel, NEIGHBORS[NEIGHBORS[pixel][n]]);
                    if (n_happiness >= max_happiness) {
                        max_happiness = n_happiness;
                        happiest_neighbor = NEIGHBORS[pixel][n];
                    }
                }
            }
            if (happiest_neighbor != NONE) {
                if (max_happiness >= happiness) {
                    moveAgent(pixel, happiest_neighbor);
                    steps++;
                    if (steps % 100 == 0) {
                        Serial.println(steps);
                    }
                    break;
               }
            }
            
        } 
    }

    else if (state == HOLD) {
        countdown--;
        if (countdown == 0) {
            state = CODA;
            Serial.println("CODA");            
        }
    }

    else if (state == CODA) {
        uint8_t checked = 0;
        while (true) {
            if (checked == PIXELS) {
                reset();              
                state = INTRO;
                Serial.println("INTRO");
                break;
            }                    
            uint8_t pixel = sequence[index++];
            index %= PIXELS;
            checked++;
            if (getColor(pixel) != OFF_COLOR) {
                setColor(pixel, OFF_COLOR);
                break;
            }
        }
    }

    updateTransitions();
    strip.show();
    delay(DELAY);

}

void initNeighbors() {
    Serial.println("initNeighbors()");
    for (uint8_t pixel=0; pixel<PIXELS; pixel++) {
        NEIGHBORS[pixel][0] = pixel - 1;
        NEIGHBORS[pixel][1] = pixel + 1;
        NEIGHBORS[pixel][2] = pixel + PIXELS_PER_ROW - 1;
        NEIGHBORS[pixel][3] = pixel + PIXELS_PER_ROW;
        NEIGHBORS[pixel][4] = pixel + PIXELS_PER_ROW + 1;
        NEIGHBORS[pixel][5] = pixel - PIXELS_PER_ROW - 1;
        NEIGHBORS[pixel][6] = pixel - PIXELS_PER_ROW;
        NEIGHBORS[pixel][7] = pixel - PIXELS_PER_ROW + 1;
        if (pixel < PIXELS_PER_ROW) {
            NEIGHBORS[pixel][5] = NONE;
            NEIGHBORS[pixel][6] = NONE;
            NEIGHBORS[pixel][7] = NONE;
        }
        if (pixel >= PIXELS - PIXELS_PER_ROW) {
            NEIGHBORS[pixel][2] = NONE;
            NEIGHBORS[pixel][3] = NONE;
            NEIGHBORS[pixel][4] = NONE;
        }
        if (pixel % PIXELS_PER_ROW == 0) {
            NEIGHBORS[pixel][0] = NONE;
            NEIGHBORS[pixel][2] = NONE;
            NEIGHBORS[pixel][5] = NONE;
        }
        if (pixel % PIXELS_PER_ROW == PIXELS_PER_ROW - 1) {
            NEIGHBORS[pixel][1] = NONE;
            NEIGHBORS[pixel][4] = NONE;
            NEIGHBORS[pixel][7] = NONE;
        }
    }
}

uint8_t calcHappiness(uint8_t pixel, uint8_t neighbors[]) {
    uint8_t same = 0;
    for (uint8_t n=0; n<8; n++) {
        if (getColor(neighbors[n]) == getColor(pixel)) {            
            same++;
        }
    }    
    return same;
}

void moveAgent(uint8_t current_pixel, uint8_t new_pixel) {
    setColor(new_pixel, getColor(current_pixel));
    setColor(current_pixel, OFF_COLOR);
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

uint32_t getColor(uint8_t pixel) {
    return pixel_colors[pixel];
}

void resetColors() {
    for (uint8_t pixel=0; pixel<PIXELS; pixel++) {
        paintColor(pixel, OFF_COLOR);
        pixel_colors[pixel] = OFF_COLOR;
        previous_pixel_colors[pixel] = OFF_COLOR;        
    }
}

void setColor(uint8_t pixel, uint32_t color) {
    if (color != pixel_colors[pixel]) {
        previous_pixel_colors[pixel] = pixel_colors[pixel];
        pixel_colors[pixel] = color;
        transitions[pixel] = 2;
    }
}

void updateTransitions() {   
    for (uint8_t pixel=0; pixel<PIXELS; pixel++) {
        if (transitions[pixel] != NONE) {
            uint32_t c1 = previous_pixel_colors[pixel];
            uint32_t c2 = pixel_colors[pixel];
            float pos = (3 - transitions[pixel]) / 3;            
            uint32_t c = lerpColor(c1, c2, pos);           
            paintColor(pixel, c);
            transitions[pixel]--;          // -1 will wrap to 255 which is NONE             
        }
    }
}


uint32_t lerpColor(uint32_t current, uint32_t target, float pos) {
    pos = (pos * .5) + .5;
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


void paintColor(uint8_t pixel, uint32_t color) { 
    int row = floor(pixel / PIXELS_PER_ROW);
    int column = pixel - (row * PIXELS_PER_ROW);     
    column = row % 2 > 0 ? (PIXELS_PER_ROW - 1) - column : column;    // flip odd rows
    pixel = (row * PIXELS_PER_ROW) + column;                          // adjusted pixel
    int led = (pixel * 4) - (floor(pixel / PIXELS_PER_ROW) * 3);  
    strip.setPixelColor(led, color);  
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
