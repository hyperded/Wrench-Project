
#include <FastLED.h>

#define COLS 52
#define ROWS 18
#define NUM_LEDS (COLS * ROWS)

#define DATA_PIN 2
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 100

CRGB leds[NUM_LEDS];
presbyte and chatgpt (mostly chatgpt lol)
// ===== SERPENTINE MATRIX MAPPING =====
uint16_t XY(int x, int y) {
    if (y < 0 || y >= ROWS || x < 0 || x >= COLS) {
        return 0;
    }

    if (y % 2 == 0) {
        return y * COLS + x;
    }

    return y * COLS + (COLS - 1 - x);
}

// ===== DRAWING FUNCTIONS =====

void setPixel(int x, int y, CRGB color) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        leds[XY(x, y)] = color;
    }
}

void drawGlyphHD() {
    // Your glyph drawing code here
}

void drawXHD() {
    // Your X drawing code here
}

void drawFrame() {
    FastLED.clear();

    drawGlyphHD();
    drawXHD();
}

// ===== INITIALIZATION =====

void setup() {
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(
        leds, NUM_LEDS
    );

    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear(true);
}

// ===== MAIN LOOP =====

void loop() {
// Receive commands from Python
    while (Serial.available()) {
        char incoming = Serial.read();

        if (incoming == 'A' || incoming == 'B' || incoming == 'C') {
            handleAction(incoming);
        }
    }

    // Select drawing based on current action
    FastLED.clear();

    switch (currentAction) {
        case 'A':
            drawGlyphHD();
            break;

        case 'B':
            drawXHD();
            break;

        case 'C':
            drawIdle();
            break;
    }
    FastLED.show();
    delay(16); // Approximately 60 FPS
}
