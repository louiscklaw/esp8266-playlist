/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include "Arduino.h"
#include <WS2812FX.h>

#define LED_COUNT 8
#define LED_PIN 15

#define TIMER_MS 1000

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

unsigned long last_change = 0;
unsigned long now = 0;

void setup()
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  ws2812fx.init();
  ws2812fx.setBrightness(4);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(0x007BFF);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();
}

void loop()
{
  now = millis();

  ws2812fx.service();

  if(now - last_change > TIMER_MS) {
    ws2812fx.setMode((ws2812fx.getMode() + 1) % ws2812fx.getModeCount());
    last_change = now;
  }
}
