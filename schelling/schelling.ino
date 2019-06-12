#include <Adafruit_NeoPixel.h>

// settable params
const uint8_t DATA_PIN = 6;
const uint8_t PIXELS_PER_ROW = 15;
const uint8_t ROWS = 15;
const uint8_t TYPES = 2;
const int MAX_STEPS = 2500;
const uint8_t COUNTDOWN = 100;

// dependent constants
const uint8_t PIXELS = PIXELS_PER_ROW * ROWS;
const int LEDS_PER_ROW = (((PIXELS_PER_ROW - 1) * 4) + 1) * ROWS;
const uint8_t POPULATION = floor(PIXELS / 2);

Adafruit_NeoPixel strip(LEDS_PER_ROW * ROWS, DATA_PIN, NEO_GRB + NEO_KHZ800);

// colors
const uint32_t COLOR_1 = strip.Color(255, 0, 100);
const uint32_t COLOR_2 = strip.Color(0, 100, 255);
const uint32_t COLORS[] = {COLOR_1, COLOR_2};
const uint32_t OFF_COLOR = strip.Color(0, 0, 0);

// states
const uint8_t STARTUP = 0;
const uint8_t PLAY = 1;
const uint8_t HOLD = 2;
const uint8_t SHUTDOWN = 3;

// declare persistant variables
uint8_t state;
uint8_t index;
uint8_t steps;
uint8_t countdown;

// allocate arrays
uint8_t NEIGHBORS[PIXELS][8];
uint32_t pixel_colors[PIXELS];
uint32_t previous_pixel_colors[PIXELS];
uint8_t sequence[PIXELS];
uint8_t transitions[PIXELS];

// p5 specific initialization
void setup() {
    randomSeed(analogRead(0));
    strip.begin();
    strip.show();
    strip.setBrightness(255);
    initNeighbors();
    reset();
}

// reset process
void reset() {
    state = STARTUP;
    index = 0;
    steps = 0;   
    countdown = COUNTDOWN;
    resetColors();
    for (uint8_t t=0; t<PIXELS; t++) {
        transitions[t] = -1;
    }    
    for (uint8_t s=0; s<PIXELS; s++) {
        sequence[s] = s;
    }
    shuffleSequence(sequence);
}

void loop() {

    if (state == STARTUP) {
        uint8_t pixel = sequence[index++];
        setColor(pixel, COLORS[index % TYPES]);
        updateTransitions();
        if (index == POPULATION) {
            state = PLAY;    
            // console.log("play")
        }
    } 

    else if (state == PLAY) {
        while (true) {
            if (steps == MAX_STEPS) {
                // console.log("hit max");
                state = SHUTDOWN;
                return;
            }
            uint8_t pixel = sequence[index++];
            index %= PIXELS;
            if (getColor(pixel) == OFF_COLOR) {
                continue;
            }
            uint8_t happiness = calcHappiness(pixel, NEIGHBORS[pixel]);
            uint8_t happiest_neighbor = -1;
            uint8_t max_happiness = 0;
            for (uint8_t j=7; j>=0; j--) {
                uint8_t offset = random(0, 8);
                uint8_t n = (j + offset) % 8;
                if (NEIGHBORS[pixel][n] == -1) {
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
            if (happiest_neighbor >=0 && max_happiness >= happiness) {
                moveAgent(pixel, happiest_neighbor);
                steps++;
                updateTransitions();
                return;
            }
            
        } 
    }

    else if (state == HOLD) {
        countdown--;
        if (countdown == 0) {
            state = SHUTDOWN;
        }
    }

    else if (state == SHUTDOWN) {
        uint8_t checked = 0;
        while (true) {
            if (checked == PIXELS) {
                reset();
                return;
            }                    
            uint8_t pixel = sequence[index++];
            index %= PIXELS;
            checked++;
            if (getColor(pixel) != OFF_COLOR) {
                setColor(pixel, OFF_COLOR);
                updateTransitions();
                return;
            }
        }
    }

}

void initNeighbors() {
    for (uint8_t pixel=0; pixel<PIXELS; pixel++) {
        NEIGHBORS[0][0] = pixel - 1;
        NEIGHBORS[0][1] = pixel + 1;
        NEIGHBORS[0][2] = pixel + PIXELS_PER_ROW - 1;
        NEIGHBORS[0][3] = pixel + PIXELS_PER_ROW;
        NEIGHBORS[0][4] = pixel + PIXELS_PER_ROW + 1;
        NEIGHBORS[0][5] = pixel - PIXELS_PER_ROW - 1;
        NEIGHBORS[0][6] = pixel - PIXELS_PER_ROW;
        NEIGHBORS[0][7] = pixel - PIXELS_PER_ROW + 1;
        if (pixel < PIXELS_PER_ROW) {
            NEIGHBORS[0][5] = -1;
            NEIGHBORS[0][6] = -1;
            NEIGHBORS[0][7] = -1;
        }
        if (pixel >= PIXELS - PIXELS_PER_ROW) {
            NEIGHBORS[0][2] = -1;
            NEIGHBORS[0][3] = -1;
            NEIGHBORS[0][4] = -1;
        }
        if (pixel % PIXELS_PER_ROW == 0) {
            NEIGHBORS[0][0] = -1;
            NEIGHBORS[0][2] = -1;
            NEIGHBORS[0][5] = -1;
        }
        if (pixel % PIXELS_PER_ROW == PIXELS_PER_ROW - 1) {
            NEIGHBORS[0][1] = -1;
            NEIGHBORS[0][4] = -1;
            NEIGHBORS[0][7] = -1;
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
        if (transitions[pixel] >= 0) {
            uint32_t c1 = previous_pixel_colors[pixel];
            uint32_t c2 = pixel_colors[pixel];
            float pos = (3 - transitions[pixel]) / 3;            
            uint32_t c = lerpColor(c1, c2, pos);           
            paintColor(pixel, c);
            transitions[pixel]--;                       
        }
    }
}

uint32_t lerpColor(uint32_t c1, uint32_t c2, float pos) {
    // figure out
    return c2;
}

void paintColor(uint8_t pixel, uint32_t color) {    
    int led = (pixel * 4) - (floor(pixel / PIXELS_PER_ROW) * 3);  
    strip.setPixelColor(led, color);  
}
