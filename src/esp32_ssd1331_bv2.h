/*
  esp32_ssd1331_bv2.h - for Arduino core for the ESP32 ( Use SPI library ).
  Beta version 2.0.0
  
  Modified ESP32_SSD1331 beta ver 1.71.
  
The MIT License (MIT)

Copyright (c) 2017 Mgo-tec. All rights reserved.
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

SPI.h (SPI library for ESP32)
Copyright (c) 2015 Hristo Gochkov. All rights reserved.
License : LGPL 2.1
*/

#ifndef ESP32_SSD1331_BV2_H_INCLUDED
#define ESP32_SSD1331_BV2_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>

class Esp32Ssd1331Bv2
{
private:
  uint8_t mp_sck;
  uint8_t mp_miso;
  uint8_t mp_mosi;
  uint8_t mp_cs;
  uint8_t mp_dc;
  uint8_t mp_rst;
  uint8_t mp_scroll_count[6] = {};
  uint8_t mp_full_or_half_count[6] = {};

public:
  Esp32Ssd1331Bv2(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t dc, uint8_t rst);

  void init();
  void init65kColor();
  void init(bool bl, uint8_t cs1, uint8_t cs2, uint8_t cs3,  bool isColor_65k = false);
  void writeSpiCommand(uint8_t b);
  void writeSpiData(uint8_t b);
  void writeSpiCommandBytes(uint8_t *b, uint16_t size);
  void writeSpiDataBytes(uint8_t *b, uint16_t size);
  void brightness(uint8_t brightness);
  void brightnessFadeIn(uint8_t interval);
  void brightnessFadeOut(uint8_t interval);
  void clearDisplay();
  void clearDisplay(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void clearDisplay(uint8_t cs, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void copyDisplay(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t to_x, uint8_t to_y);
  void displayFont8x16(uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]);
  void displayFont8x16(uint8_t cs, uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]);

  void displaySizeUpFont8x16(uint8_t size, uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]);
  void displayHVsizeUpFont8x16(uint8_t h_size, uint8_t v_size, uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]);

  void displayFont8x8(uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][8]);
  void VscrollFont16x16(uint8_t direction, uint8_t full_or_half, uint8_t buf[2][16], uint8_t *scroll_count, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t red, uint8_t green, uint8_t blue);
  void HscrollFont16x16(uint8_t direction, uint8_t full_or_half, uint8_t buf[2][16], uint8_t *scroll_count, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t red, uint8_t green, uint8_t blue);
  void drawPixel256color(uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue);
  void drawPixel65kColor(uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue);
  void drawPixel65kColor2(uint8_t x0, uint8_t y0, uint8_t color_2bytes[2]);
  void drawPixel65kColorBytes(uint8_t x0, uint8_t y0, uint8_t *color_2bytes, uint8_t size);
  void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t line_red, uint8_t line_green, uint8_t line_blue);
  void drawRectangleLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t line_red, uint8_t line_green, uint8_t line_blue);
  void drawRectangleFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t line_red, uint8_t line_green, uint8_t line_blue, uint8_t fill_red, uint8_t fill_green, uint8_t fill_blue);
  void drawCircleLine256Color(uint8_t x0, uint8_t y0, uint16_t r, uint8_t line_red, uint8_t line_green, uint8_t line_blue);
  void drawCircleLine65kColor(uint8_t x0, uint8_t y0, uint16_t r, uint8_t line_red, uint8_t line_green, uint8_t line_blue);
  void drawCircleFill(uint8_t x0, uint8_t y0, uint16_t r, uint8_t line_red, uint8_t line_green, uint8_t line_blue);

  bool scrollFont8x16RtoL(uint8_t y0, uint8_t full_or_half, uint8_t font_buf[16], uint8_t *scroll_count, uint8_t *full_or_half_count, uint8_t red, uint8_t green, uint8_t blue);
  bool scrollFont8x16RtoL4Line(uint8_t cs_pin, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t *scroll_count, uint8_t *full_or_half_count, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);
  bool scrollFont8x16RtoL4Line(uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t *scroll_count, uint8_t *full_or_half_count, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);
  bool scrollFont8x16RtoL4Line(uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);

  bool scrollSizeUpFont8x16RtoL(uint8_t size, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);
  bool scrollSizeUpFont8x16RtoL(uint8_t size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);
  bool scrollHVSizeUpFont8x16RtoL(uint8_t h_size, uint8_t v_size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);

  bool scrollHVSizeUpFont8x16RtoL(boolean *scrolle_reset, boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);

  bool scrollHVSizeUpFont8x16RtoL(boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);
  void copyScrollSizeUp(boolean reverse, uint8_t size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue);

  void copyScroll(uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue);
  void copyScrollSizeUp(uint8_t size, uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue);
  void copyScrollSizeUp(uint8_t size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue);

  bool VscrollHVSizeUpFont16x16(uint8_t h_size, uint8_t v_size, uint8_t direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);

  bool VscrollHVSizeUpFont16x16(boolean *scrolle_reset, boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);

  bool VscrollHVSizeUpFont16x16(boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue);
  void copyVscrollSizeUpFont(boolean reverse, uint8_t h_size, uint8_t direction, uint8_t full_or_half, uint8_t buf[][16], uint8_t scroll_count, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t red, uint8_t green, uint8_t blue);

};

#endif
