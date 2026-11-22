void setPixel(int x, int y, CRGB color) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        leds[XY(x, y)] = color;
    }
}

// Draws a 14x10 bitmap centered at (cx, cy) for 52x18 resolution
void drawGlyphHD(int cx, int cy, const uint16_t rows[10], CRGB color) {
    for (int r = 0; r < 10; r++) {
        for (int c = 0; c < 14; c++) {
            if (rows[r] & (1 << (13 - c))) {
                setPixel(cx - 7 + c, cy - 5 + r, color);
            }
        }
    }
}

// Draws a 2x-thick X for 52x18 resolution
void drawXHD(int cx, int cy, int halfW, int halfH, CRGB color) {
    for (int i = -halfH; i <= halfH; i++) {
        int dx = (i * halfW) / halfH;
        for (int tx = -1; tx <= 1; tx++) {
            setPixel(cx + dx + tx, cy + i, color);
            setPixel(cx - dx + tx, cy + i, color);
        }
    }
}

void drawFrame() {
    uint32_t t = millis();

    fill_solid(leds, COLS * ROWS, CRGB::Black);

    // High-res 14x10 bitmap for "<3" (chunky 2px strokes)
    const uint16_t HEART_SIDE_HD[10] = {
        0b00011000110110,
        0b00110001111111,
        0b01100011111111,
        0b11000011111111,
        0b11000011111111,
        0b11000001111110,
        0b01100000111100,
        0b00110000011000,
        0b00011000000000,
        0b00000000000000
    };

    // High-res 14x10 bitmap for "@" (chunky 2px spiral)
    const uint16_t AT_SIGN_HD[10] = {
        0b00111111111000,
        0b01111111111100,
        0b11000000000110,
        0b11000111110110,
        0b11001100110110,
        0b11001100110110,
        0b11001111111100,
        0b11000111101100,
        0b01110000000000,
        0b00111111111000
    };

    // Cycle through 3 expressions every 3 seconds: 0 = X X, 1 = <3 <3, 2 = @ @
    int mode = (t / 3000) % 3;

    // Blink right before switching expressions
    uint32_t sub = t % 3000;
    bool blinking = (sub > 2750 && sub < 2920);

    // Subtle 2-pixel vertical visor bounce & pulse
    int bobY = (sin8(t / 14) > 160) ? 1 : ((sin8(t / 14) < 95) ? -1 : 0);
    uint8_t pulse = 220 + (sin8(t / 8) >> 3);

    // Tuned eye centers for 52x18 wide visor layout
    int leftX = 13;
    int rightX = 38;
    int centerY = 9 + bobY;

    if (blinking) {
        CRGB blinkColor = CRGB(240, 240, 255);
        for (int i = -6; i <= 6; i++) {
            setPixel(leftX + i, centerY, blinkColor);
            setPixel(leftX + i, centerY + 1, blinkColor);
            setPixel(rightX + i, centerY, blinkColor);
            setPixel(rightX + i, centerY + 1, blinkColor);
        }
    } else if (mode == 0) {
        // X X (Classic Wrench white/cool LED glow)
        CRGB xColor = CRGB(pulse, pulse, 255);
        drawXHD(leftX, centerY, 5, 5, xColor);
        drawXHD(rightX, centerY, 5, 5, xColor);
    } else if (mode == 1) {
        // <3 <3 (Pink/Red LED glow)
        CRGB heartColor = CRGB(255, 40, 110);
        drawGlyphHD(leftX, centerY, HEART_SIDE_HD, heartColor);
        drawGlyphHD(rightX, centerY, HEART_SIDE_HD, heartColor);
    } else {
        // @ @ (Cyan/White dizzy LED glow)
        CRGB dizzyColor = CRGB(120, pulse, 255);
        drawGlyphHD(leftX, centerY, AT_SIGN_HD, dizzyColor);
        drawGlyphHD(rightX, centerY, AT_SIGN_HD, dizzyColor);
    }
}
