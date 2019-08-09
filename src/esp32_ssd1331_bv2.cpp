/*
  esp32_ssd1331_bv2.cpp - for Arduino core for the ESP32 ( Use SPI library ).
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

#include "esp32_ssd1331_bv2.h"

Esp32Ssd1331Bv2::Esp32Ssd1331Bv2(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t dc, uint8_t rst)
  :mp_sck(sck)
  , mp_miso(miso)
  , mp_mosi(mosi)
  , mp_cs(cs)
  , mp_dc(dc)
  , mp_rst(rst)
{}

static SPIClass MySPI(HSPI);

//************ SSD1331 256色カラー初期化 ****************************************
void Esp32Ssd1331Bv2::init(){
  Esp32Ssd1331Bv2::init(true, 0, 0, 0, false);
}
//************ SSD1331 256色カラー初期化 ****************************************
void Esp32Ssd1331Bv2::init65kColor(){
  Esp32Ssd1331Bv2::init(true, 0, 0, 0, true);
}

//************ SSD1331 初期化 ****************************************
void Esp32Ssd1331Bv2::init(bool bl, uint8_t cs1, uint8_t cs2, uint8_t cs3, bool isColor_65k){
  pinMode(mp_rst, OUTPUT); //RES
  pinMode(mp_dc, OUTPUT); //DC

  digitalWrite(mp_rst, HIGH);
  digitalWrite(mp_rst, LOW);
  delay(1);
  digitalWrite(mp_rst, HIGH);

  digitalWrite(mp_dc, HIGH);

  if(bl == false){
    MySPI.begin(mp_sck, mp_miso, mp_mosi, -1);
  }else{
    MySPI.begin(mp_sck, mp_miso, mp_mosi, mp_cs);
  }
  
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  MySPI.setHwCs(bl);
  
  if(cs1 > 0){
    pinMode(cs1, OUTPUT);
    digitalWrite(cs1, HIGH);
    digitalWrite(cs1, LOW);
  }
  if(cs2 > 0){
    pinMode(cs2, OUTPUT);
    digitalWrite(cs2, HIGH);
    digitalWrite(cs2, LOW);
  }
  if(cs3 > 0){
    pinMode(cs3, OUTPUT);
    digitalWrite(cs3, HIGH);
    digitalWrite(cs3, LOW);
  }

  writeSpiCommand(0xAE); //Set Display Off
  writeSpiCommand(0xA0); //Remap & Color Depth setting
    if(isColor_65k){
      writeSpiCommand(0b01110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
    }else{
      writeSpiCommand(0b00110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
    }
  writeSpiCommand(0xA1); //Set Display Start Line
    writeSpiCommand(0);
  writeSpiCommand(0xA2); //Set Display Offset
    writeSpiCommand(0);
  writeSpiCommand(0xA4); //Set Display Mode (Normal)
  writeSpiCommand(0xA8); //Set Multiplex Ratio
    writeSpiCommand(0b00111111); //15-63
  writeSpiCommand(0xAD); //Set Master Configration
    writeSpiCommand(0b10001110); //a[0]=0 Select external Vcc supply, a[0]=1 Reserved(reset)
  writeSpiCommand(0xB0); //Power Save Mode
    writeSpiCommand(0b00000000); //0x1A Enable power save mode
  writeSpiCommand(0xB1); //Phase 1 and 2 period adjustment
    writeSpiCommand(0x74);
  writeSpiCommand(0xB3); //Display Clock DIV
    writeSpiCommand(0xF0);
  writeSpiCommand(0x8A); //Pre Charge A
    writeSpiCommand(0x81);
  writeSpiCommand(0x8B); //Pre Charge B
    writeSpiCommand(0x82);
  writeSpiCommand(0x8C); //Pre Charge C
    writeSpiCommand(0x83);
  writeSpiCommand(0xBB); //Set Pre-charge level
    writeSpiCommand(0x3A);
  writeSpiCommand(0xBE); //Set VcomH
    writeSpiCommand(0x3E);
  writeSpiCommand(0x87); //Set Master Current Control
    writeSpiCommand(0x06);
  writeSpiCommand(0x15); //Set Column Address
    writeSpiCommand(0);
    writeSpiCommand(95);
  writeSpiCommand(0x75); //Set Row Address
    writeSpiCommand(0);
    writeSpiCommand(63);
  writeSpiCommand(0x81); //Set Contrast for Color A
    writeSpiCommand(255);
  writeSpiCommand(0x82); //Set Contrast for Color B
    writeSpiCommand(255);
  writeSpiCommand(0x83); //Set Contrast for Color C
    writeSpiCommand(255);

  for(int j=0; j<64; j++){ //Display Black OUT
    for(int i=0; i<96; i++){
      writeSpiData(0x00);
      writeSpiData(0x00);
      writeSpiData(0x00);
    }
  }

  writeSpiCommand(0xAF); //Set Display On
  
  if(cs1 > 0){
    digitalWrite(cs1, HIGH);
  }
  if(cs2 > 0){
    digitalWrite(cs2, HIGH);
  }
  if(cs3 > 0){
    digitalWrite(cs3, HIGH);
  }
  delay(110); //ディスプレイONコマンドの後は最低100ms 必要
}
//*********** HSPI 通信送信 ***********************************
void Esp32Ssd1331Bv2::writeSpiCommand(uint8_t b){
  digitalWrite(mp_dc, LOW);//DC
  MySPI.write(b);
}

void Esp32Ssd1331Bv2::writeSpiData(uint8_t b){
  digitalWrite(mp_dc, HIGH);//DC
  MySPI.write(b);
}

void Esp32Ssd1331Bv2::writeSpiCommandBytes(uint8_t *b, uint16_t size){
  digitalWrite(mp_dc, LOW);//DC
  MySPI.writeBytes(b, size);

}

void Esp32Ssd1331Bv2::writeSpiDataBytes(uint8_t *b, uint16_t size){
  digitalWrite(mp_dc, HIGH);//DC
  MySPI.writeBytes(b, size);
}

//***********画面明るさ********************************
void Esp32Ssd1331Bv2::brightness(uint8_t brightness){
  writeSpiCommand(0x81); //Set Contrast for Color A
    writeSpiCommand(brightness);
  writeSpiCommand(0x82); //Set Contrast for Color B
    writeSpiCommand(brightness);
  writeSpiCommand(0x83); //Set Contrast for Color C
    writeSpiCommand(brightness);
}
//***********画面明るさフェードイン**********************
void Esp32Ssd1331Bv2::brightnessFadeIn(uint8_t interval){
  for(int brightness = 0; brightness < 256; brightness++){
    writeSpiCommand(0x81); //Set Contrast for Color A
      writeSpiCommand(brightness);
    writeSpiCommand(0x82); //Set Contrast for Color B
      writeSpiCommand(brightness);
    writeSpiCommand(0x83); //Set Contrast for Color C
      writeSpiCommand(brightness);
    delay(interval);
  }
}
//***********画面明るさフェードアウト********************
void Esp32Ssd1331Bv2::brightnessFadeOut(uint8_t interval){
  for(int brightness = 255; brightness >= 0; brightness--){
    writeSpiCommand(0x81); //Set Contrast for Color A
      writeSpiCommand(brightness);
    writeSpiCommand(0x82); //Set Contrast for Color B
      writeSpiCommand(brightness);
    writeSpiCommand(0x83); //Set Contrast for Color C
      writeSpiCommand(brightness);
    delay(interval);
  }
}
//***********ディスプレイ消去***************************
void Esp32Ssd1331Bv2::clearDisplay(){
  Esp32Ssd1331Bv2::clearDisplay(0, 0, 95, 63);
}
//***********ディスプレイ消去***************************
void Esp32Ssd1331Bv2::clearDisplay(uint8_t cs, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  //MySPI.setHwCs(bl);
  digitalWrite(cs, LOW);
  Esp32Ssd1331Bv2::clearDisplay(x0, y0, x1, y1);
  digitalWrite(cs, HIGH);
}

//***********ディスプレイ消去***************************
void Esp32Ssd1331Bv2::clearDisplay(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  //delayMicroseconds(500); //クリアーコマンドは400μs 以上の休止期間が必要かも
  delay(1);
  writeSpiCommand(0x25); //Clear Window
    writeSpiCommand(x0); //Column Address of Start
    writeSpiCommand(y0); //Row Address of Start
    writeSpiCommand(x1); //Column Address of End
    writeSpiCommand(y1); //Row Address of End
  //delayMicroseconds(800); //ここの間隔は各自調節してください。
  delay(1);
}
//********** 範囲コピー *********************************
void Esp32Ssd1331Bv2::copyDisplay(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t to_x, uint8_t to_y){
  uint8_t buf[7];
  buf[0] = 0x23; //Copy Command
  buf[1] = x0; buf[2] = y0; buf[3] = x1; buf[4] = y1; buf[5] = to_x; buf[6] = to_y; 

  //delayMicroseconds(500);
  delay(1);
  writeSpiCommandBytes(buf, sizeof(buf)); 
  //delayMicroseconds(500);
  delay(1);
}
//********* OLED 8x16フォント出力 ********************
void Esp32Ssd1331Bv2::displayFont8x16(uint8_t cs, uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]){
  digitalWrite(cs, LOW);
  Esp32Ssd1331Bv2::displayFont8x16(txt_size, x0, y0, red, green, blue, font_buf);
  digitalWrite(cs, HIGH);
}
//********* OLED 8x16フォント出力 ********************
void Esp32Ssd1331Bv2::displayFont8x16(uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]){
  uint8_t com[6];
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  //MySPI.setHwCs(true);
  
  if( txt_size > 12) txt_size = 12;
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0 + (8*txt_size) - 1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0 + 15;

  writeSpiCommandBytes(com, 6);

  int i, j, k;
  uint8_t bt = 0b10000000;

  uint8_t Dot = (red << 5) | (green << 2) | blue;

  uint8_t dummy[16*txt_size*8];
  uint16_t cnt = 0;

  for(i=0; i<16; i++){
    for(j=0; j<txt_size; j++){
      for(k=0; k<8; k++){
        if( font_buf[j][i] & (bt >> k) ){
          dummy[cnt] = Dot;
        }else{
          dummy[cnt] = 0;
        }
        cnt++;
      }
    }
  }

  writeSpiDataBytes(dummy, (16 * txt_size * 8));
}
//********* OLED 8x16フォント　サイズアップ　出力 ********************
void Esp32Ssd1331Bv2::displaySizeUpFont8x16(uint8_t size, uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]){
  Esp32Ssd1331Bv2::displayHVsizeUpFont8x16(size, size, txt_size, x0, y0, red, green, blue, font_buf);
}
//********* OLED 8x16フォント　サイズアップ　出力 ********************
void Esp32Ssd1331Bv2::displayHVsizeUpFont8x16(uint8_t h_size, uint8_t v_size, uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][16]){
  uint8_t com[6];
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  //MySPI.setHwCs(true);
  
  if( txt_size > 12) txt_size = 12;
  
  if(h_size == 4){
    txt_size = 3;
  }else if(h_size == 2){
    txt_size = 6;
  }
  uint8_t txtmax_sizeup = txt_size * (8 * h_size);
  
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    //com[2] = 95;
    com[2] = x0 + txtmax_sizeup - 1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0 + (16*v_size)-1;
    //com[5] = 63;

  writeSpiCommandBytes(com, 6);

  int i, j, k, ii;
  uint8_t bt = 0b10000000;

  uint8_t Dot = (red << 5) | (green << 2) | blue;
  
  uint8_t dummy[16*v_size][96] = {};
  
  uint16_t cnt = 0;

  uint8_t size_cnt=0;
  for(i=0; i<16; i++){
      for(j=0; j<txt_size; j++){
        for(k=0; k<8; k++){
          if( font_buf[j][i] & (bt >> k) ){
            for(ii=0; ii<h_size; ii++){
              dummy[i][cnt++] = Dot;
            }
          }else{
            cnt = cnt + h_size;
          }
        }
      }
      cnt = 0;
  }
  //digitalWrite(mp_dc, LOW);//DC

  for(i=0; i<16; i++){
    for(size_cnt=0; size_cnt<v_size; size_cnt++){
      writeSpiDataBytes(dummy[i], (txtmax_sizeup));
      //MySPI.writeBytes(dummy[i], (txtmax_sizeup));
    }
  }
}
//********* OLED 8x8フォント出力 ********************
void Esp32Ssd1331Bv2::displayFont8x8(uint8_t txt_size, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t font_buf[][8]){
  uint8_t com[6];
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  //MySPI.setHwCs(true);

  if( txt_size > 12) txt_size = 12;
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0 + (8*txt_size) - 1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0 + 7;

  writeSpiCommandBytes(com, sizeof(com));

  int i, j, k;
  uint8_t bt = 0b10000000;

  uint8_t Dot = (red << 5) | (green << 2) | blue;

  uint8_t dummy[8*txt_size*8];
  uint16_t cnt = 0;

  for(i=0; i<8; i++){
    for(j=0; j<txt_size; j++){
      for(k=0; k<8; k++){
        if( font_buf[j][i] & (bt >> k) ){
          dummy[cnt] = Dot;
        }else{
          dummy[cnt] = 0;
        }
        cnt++;
      }
    }
  }

  writeSpiDataBytes(dummy, (8 * txt_size * 8));
}
//*********** 時刻垂直スクロール ****************
void Esp32Ssd1331Bv2::VscrollFont16x16(uint8_t direction, uint8_t full_or_half, uint8_t buf[2][16], uint8_t *scroll_count, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t red, uint8_t green, uint8_t blue){
  Esp32Ssd1331Bv2::copyVscrollSizeUpFont(false, 1, direction, full_or_half, buf, *scroll_count, x0, y0, y1, red, green, blue);
  (*scroll_count)++;
}
//*********** 16x16フォント１文字水平スクロール **********************
void Esp32Ssd1331Bv2::HscrollFont16x16(uint8_t direction, uint8_t full_or_half, uint8_t buf[2][16], uint8_t *scroll_count, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t red, uint8_t green, uint8_t blue){
  uint8_t com[6];
  uint8_t Dot = (red << 5) | (green << 2) | blue;
  uint8_t i;
  uint8_t k = 0;
  uint8_t bbb = 0;

  switch( direction ){
    case 0:
      copyDisplay(x0+1, y0, x1, y1, x0, y0);

      com[0] = 0x15; //Set Column Address
        com[1] = x1; com[2] = x1;
      com[3] = 0x75; //Set Row Address
        com[4] = y0; com[5] = y1;

      if((*scroll_count) < 8){
        bbb = 0b10000000 >> (*scroll_count);
      }else if((*scroll_count) >= 8){
        bbb = 0b10000000 >> ((*scroll_count)-8);
      }

      if((*scroll_count) < 8){
        k = 0;
      }else{
        k = 1;
      }

      break;
    case 1:
      copyDisplay(x0, y0, x1-1, y1, x0+1, y0);

      com[0] = 0x15; //Set Column Address
        com[1] = x0; com[2] = x0;
      com[3] = 0x75; //Set Row Address
        com[4] = y0; com[5] = y1;

      if((*scroll_count) < 8){
        bbb = 0b00000001 << (*scroll_count);
      }else if((*scroll_count) >= 8){
        bbb = 0b00000001 << ((*scroll_count) - 8);
      }

      if(full_or_half == 2){
        if((*scroll_count) < 8){
          k = 1;
        }else{
          k = 0;
        }
      }else if(full_or_half ==1){
        k = 0;
      }

      break;
  }

  writeSpiCommandBytes(com, 6);  

  uint8_t color_2bytes[16];

  for(i=0; i<16; i++){
    if((buf[k][i] & bbb) > 0){
      color_2bytes[i] = Dot;
    }else{
      color_2bytes[i] = 0;
    }
  }

  writeSpiDataBytes(color_2bytes, 16);
  (*scroll_count)++;
}
//************ 256色カラー 1ピクセル 描画 *****************
void Esp32Ssd1331Bv2::drawPixel256color(uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue){
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  uint8_t com[6];
  //red (0-7), green (0-7), blue (0-3)
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0;

  writeSpiCommandBytes(com, 6);

  uint8_t Dot = (red << 5) | (green << 2) | blue;

  writeSpiData(Dot);
}
//************ 65000色カラー 1ピクセル 描画 *****************
void Esp32Ssd1331Bv2::drawPixel65kColor(uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue){
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  uint8_t com[6];
  uint8_t color_2bytes[2];

  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0;

  writeSpiCommandBytes(com, 6);

  color_2bytes[0] = (red << 3) | (green >> 3);
  color_2bytes[1] = (green << 5) | blue;

  writeSpiDataBytes(color_2bytes, 2);
}
//************ 65000色カラー 1ピクセル 描画 *****************
void Esp32Ssd1331Bv2::drawPixel65kColor2(uint8_t x0, uint8_t y0, uint8_t color_2bytes[2]){
  Esp32Ssd1331Bv2::drawPixel65kColorBytes(x0, y0, color_2bytes, 2);
}
//************ 65000色カラー 1ピクセル 描画 *****************
void Esp32Ssd1331Bv2::drawPixel65kColorBytes(uint8_t x0, uint8_t y0, uint8_t *color_2bytes, uint8_t size){
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  uint8_t com[6];

  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0 + size / 2;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0;

  writeSpiCommandBytes(com, 6);

  writeSpiDataBytes(color_2bytes, size);
}
//************ 65000色　線　描画******************************
void Esp32Ssd1331Bv2::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t line_red, uint8_t line_green, uint8_t line_blue){
  uint8_t com[8];
  //line_red (0-31), line_green (0-63), line_blue (0-31)
  uint8_t R, B;
  R = line_red <<1;
  B = line_blue <<1;

  com[0] = 0x21;
    com[1] = x0;
    com[2] = y0;
    com[3] = x1;
    com[4] = y1;
    com[5] = R;
    com[6] = line_green;
    com[7] = B;

  writeSpiCommandBytes(com, 8);
}
//************ 65000色　四角　線描画***************************
void Esp32Ssd1331Bv2::drawRectangleLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t line_red, uint8_t line_green, uint8_t line_blue){
  uint8_t com[13];
  //line_red (0-31), line_green (0-63), line_blue (0-31)
  uint8_t R, B;
  R = line_red <<1;
  B = line_blue <<1;

  com[0] = 0x26; //Fill Enable or Disable
    com[1] = 0b00000000; //A0=1 Fill Disable

  com[2] = 0x22; //Drawing Rectangle
    com[3] = x0; //Column Address of Start
    com[4] = y0; //Row Address of Start
    com[5] = x1; //Column Address of End
    com[6] = y1; //Row Address of End
    com[7] = R;
    com[8] = line_green;
    com[9] = B;
    com[10] = 0;
    com[11] = 0;
    com[12] = 0;

  writeSpiCommandBytes(com, 13);
}
//************ 65000色　四角　塗りつぶし描画***************************
void Esp32Ssd1331Bv2::drawRectangleFill(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t line_red, uint8_t line_green, uint8_t line_blue, uint8_t fill_red, uint8_t fill_green, uint8_t fill_blue){
  uint8_t com[13];
  //line_red (0-31), line_green (0-63), line_blue (0-31)
  //fill_red (0-31), fill_green (0-63), fill_blue (0-31)
  uint8_t lineR, lineB, fillR, fillB;
  lineR = line_red <<1;
  lineB = line_blue <<1;
  fillR = fill_red <<1;
  fillB = fill_blue <<1;

  com[0] = 0x26; //Fill Enable or Disable
    com[1] = 0b00000001; //A0=0 Fill Enable

  com[2] = 0x22; //Drawing Rectangle
    com[3] = x0; //Column Address of Start
    com[4] = y0; //Row Address of Start
    com[5] = x1; //Column Address of End
    com[6] = y1; //Row Address of End
    com[7] = lineR;
    com[8] = line_green;
    com[9] = lineB;
    com[10] = fillR;
    com[11] = fill_green;
    com[12] = fillB;

  writeSpiCommandBytes(com, 13);
}
//************* 256色カラー　円 線描画***********************
void Esp32Ssd1331Bv2::drawCircleLine256Color(uint8_t x0, uint8_t y0, uint16_t r, uint8_t line_red, uint8_t line_green, uint8_t line_blue){
  uint8_t com[6];
  //line_red (0-7), line_green (0-7), line_blue (0-3)
  uint8_t x1, y1;

  for(int i=0; i<360; i++){
    x1 = round((float)(x0 + (r * cos(radians(i)))));
    y1 = round((float)(y0 + (r * sin(radians(i)))));
    drawPixel256color(x1, y1, line_red, line_green, line_blue);
  }
  delay(1);
  //描画範囲を元に戻して置く
  com[0] = 0x15; //Set Column Address
    com[1] = 0;
    com[2] = 95;
  com[3] = 0x75; //Set Row Address
    com[4] = 0;
    com[5] = 63;

  writeSpiCommandBytes(com, 6);
}
//************* 65000色カラー　円 線描画***********************
void Esp32Ssd1331Bv2::drawCircleLine65kColor(uint8_t x0, uint8_t y0, uint16_t r, uint8_t line_red, uint8_t line_green, uint8_t line_blue){
  uint8_t com[8];
  uint8_t x1, y1;

  writeSpiCommand(0xA0); //Remap & Color Depth setting　
    writeSpiCommand(0b01110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format

  for(int i=0; i<360; i++){
    x1 = round((float)(x0 + (r * cos(radians(i)))));
    y1 = round((float)(y0 + (r * sin(radians(i)))));
    drawPixel65kColor(x1, y1, line_red, line_green, line_blue);
  }
  delay(1);
  //描画範囲を元に戻して置く
  com[0] = 0xA0; //Remap & Color Depth setting　
    com[1] = 0b00110010; //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format

  com[2] = 0x15; //Set Column Address
    com[3] = 0;
    com[4] = 95;
  com[5] = 0x75; //Set Row Address
    com[6] = 0;
    com[7] = 63;

  writeSpiCommandBytes(com, 8);
}
//************* 65000色カラー　円塗りつぶし描画*****************
void Esp32Ssd1331Bv2::drawCircleFill(uint8_t x0, uint8_t y0, uint16_t r, uint8_t line_red, uint8_t line_green, uint8_t line_blue){
  //line_red (0-31), line_green (0-63), line_blue (0-31)
  uint8_t R, B;
  R = line_red <<1;
  B = line_blue <<1;

  uint8_t x1, y1;

  for(int i=0; i<360; i++){
    x1 = round((float)(x0 + (r * cos(radians(i)))));
    y1 = round((float)(y0 + (r * sin(radians(i)))));
    drawLine(x0, y1, x1, y1, R, line_green, B);
  }
}
//*********電光掲示板風スクロール 8x16ドット********************
bool Esp32Ssd1331Bv2::scrollFont8x16RtoL(uint8_t y0, uint8_t full_or_half, uint8_t font_buf[16], uint8_t* scroll_count, uint8_t *full_or_half_count, uint8_t red, uint8_t green, uint8_t blue){

  copyScroll(y0, font_buf, *scroll_count, red, green, blue);

  (*scroll_count)++;

  if(*scroll_count == 8){
    *scroll_count = 0;
    if(full_or_half == 2){
      if(*full_or_half_count == 0){
        *full_or_half_count = 1;
        return false;
      }else{
        *full_or_half_count = 0;
        return true;
      }
    }else{
      *full_or_half_count = 0;
      return true;
    }
  }

  return false;
}
//********************************************************
bool Esp32Ssd1331Bv2::scrollFont8x16RtoL4Line(uint8_t cs_pin, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t *scroll_count, uint8_t *full_or_half_count, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  MySPI.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  MySPI.setDataMode(SPI_MODE3);
  //MySPI.setHwCs(bl);
  
  bool ret;

  digitalWrite(cs_pin, LOW);
  ret = Esp32Ssd1331Bv2::scrollFont8x16RtoL4Line(y0, num, full_or_half, scroll_count, full_or_half_count, font_buf, red, green, blue);
  digitalWrite(cs_pin, HIGH);

  return ret;
}

//********************************************************
bool Esp32Ssd1331Bv2::scrollFont8x16RtoL4Line(uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t *scroll_count, uint8_t *full_or_half_count, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  copyScroll(y0, font_buf[*full_or_half_count], *scroll_count, red, green, blue);

  *scroll_count = *scroll_count + 1;

  if(*scroll_count == 8){
    *scroll_count = 0;
    if(full_or_half == 2){
      if(*full_or_half_count == 0){
        *full_or_half_count = 1;
        return false;
      }else{
        *full_or_half_count = 0;
        return true;
      }
    }else{
      *full_or_half_count = 0;
      return true;
    }
  }

  return false;
}
//*********電光掲示板風スクロール 8x16ドット********************
bool Esp32Ssd1331Bv2::scrollFont8x16RtoL4Line(uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  return Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(false, 1, 1, 0, 95, y0, num, full_or_half, font_buf, red, green, blue);
}
//*********電光掲示板風スクロール 8x16ドット サイズアップ********************
bool Esp32Ssd1331Bv2::scrollSizeUpFont8x16RtoL(uint8_t size, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  return Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(false, size, size, 0, 95, y0, num, full_or_half, font_buf, red, green, blue);
}
//*********電光掲示板風スクロール 8x16ドット 全角サイズアップ********************
bool Esp32Ssd1331Bv2::scrollSizeUpFont8x16RtoL(uint8_t size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  return Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(false, size, size, x0, x1, y0, num, full_or_half, font_buf, red, green, blue);
}
//*********電光掲示板風スクロール 8x16ドット 縦、横サイズアップ********************
bool Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(uint8_t h_size, uint8_t v_size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  return Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(false, h_size, v_size, x0, x1, y0, num, full_or_half, font_buf, red, green, blue);
}
//*********電光掲示板風スクロール 8x16ドット 縦、横サイズアップ********************
bool Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(boolean *scrolle_reset, boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  if(*scrolle_reset){
    mp_scroll_count[num] = 0;
    mp_full_or_half_count[num] = 0;
    *scrolle_reset = false;
    return false;
  }
  return Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(reverse, h_size, v_size, x0, x1, y0, num, full_or_half, font_buf, red, green, blue);

}
//*********電光掲示板風スクロール 8x16ドット 縦、横サイズアップ********************
bool Esp32Ssd1331Bv2::scrollHVSizeUpFont8x16RtoL(boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  for(int i=0; i<h_size; i++){
    Esp32Ssd1331Bv2::copyScrollSizeUp(reverse, v_size, x0, x1, y0, font_buf[mp_full_or_half_count[num]], mp_scroll_count[num], red, green, blue);
  }
  mp_scroll_count[num]++;

  if(mp_scroll_count[num] == 8){
    mp_scroll_count[num] = 0;
    if(full_or_half == 2){
      if(mp_full_or_half_count[num] == 0){
        mp_full_or_half_count[num] = 1;
        return false;
      }else{
        mp_full_or_half_count[num] = 0;
        return true;
      }
    }else{
      mp_full_or_half_count[num] = 0;
      return true;
    }
  }

  return false;
}
//***************************
void Esp32Ssd1331Bv2::copyScroll(uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue){
  Esp32Ssd1331Bv2::copyScrollSizeUp(false, 1, 0, 95, y0, buf, scroll_count, red, green, blue);
}
//***************************
void Esp32Ssd1331Bv2::copyScrollSizeUp(uint8_t size, uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue){
  Esp32Ssd1331Bv2::copyScrollSizeUp(false, size, 0, 95, y0, buf, scroll_count, red, green, blue);
}

//***************************
void Esp32Ssd1331Bv2::copyScrollSizeUp(boolean reverse, uint8_t size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t buf[16], uint8_t scroll_count, uint8_t red, uint8_t green, uint8_t blue){
  if(size > 4) size = 4;
  uint8_t Dot = (red << 5) | (green << 2) | blue;

  uint8_t y1 = y0 + 16 * size - 1;
  copyDisplay(x0+1, y0, x1, y1, x0, y0);

  uint8_t com[6];
  com[0] = 0x15; //Set Column Address
    com[1] = x1;
    com[2] = x1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y1;

  writeSpiCommandBytes(com, 6);

  uint8_t bbb = 0b10000000 >> scroll_count;
  
  uint8_t max_size = 16 * size;

  uint8_t color_2bytes[max_size] = {};

  uint8_t Dot_write_cnt = 0;
  uint8_t font_read_cnt = 0;
  uint8_t i;
  uint8_t f_buf = 0;
  
  for(font_read_cnt=0; font_read_cnt<16; font_read_cnt++){
    if(reverse){
      f_buf = ~buf[font_read_cnt];
    }else{
      f_buf = buf[font_read_cnt];
    }
    if( f_buf & bbb ){
      for(i=0; i<size; i++){
        color_2bytes[Dot_write_cnt++] = Dot;
      }
    }else{
      for(i=0; i<size; i++){
        color_2bytes[Dot_write_cnt++] = 0;
      }
    }
  }

  writeSpiDataBytes(color_2bytes, max_size);
}

//********* 16x16ドット スクロール（全角限定）縦、横サイズアップ********************
bool Esp32Ssd1331Bv2::VscrollHVSizeUpFont16x16(uint8_t h_size, uint8_t v_size, uint8_t direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  return Esp32Ssd1331Bv2::VscrollHVSizeUpFont16x16(false, h_size, v_size, direction, x0, y0, y1, num, full_or_half, font_buf, red, green, blue);
}
//********* 16x16ドット スクロール（全角限定）縦、横サイズアップ********************
bool Esp32Ssd1331Bv2::VscrollHVSizeUpFont16x16(boolean *scrolle_reset, boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  if(*scrolle_reset){
    mp_scroll_count[num] = 0;
    *scrolle_reset = false;
    return false;
  }
  return Esp32Ssd1331Bv2::VscrollHVSizeUpFont16x16(reverse, h_size, v_size, direction, x0, y0, y1, num, full_or_half, font_buf, red, green, blue);
}
//*********　16x16ドット スクロール（全角限定）縦、横サイズアップ、縦方向スクロール********************
bool Esp32Ssd1331Bv2::VscrollHVSizeUpFont16x16(boolean reverse, uint8_t h_size, uint8_t v_size, uint8_t direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t full_or_half, uint8_t font_buf[][16], uint8_t red, uint8_t green, uint8_t blue){
  for(int i=0; i<v_size; i++){
    Esp32Ssd1331Bv2::copyVscrollSizeUpFont(reverse, h_size, direction, full_or_half, font_buf, mp_scroll_count[num], x0, y0, y1, red, green, blue);
  }
  mp_scroll_count[num]++;

  if(mp_scroll_count[num] == 16){
    mp_scroll_count[num] = 0;
    return true;
  }

  return false;
}
//*********** 垂直スクロール ****************
void Esp32Ssd1331Bv2::copyVscrollSizeUpFont(boolean reverse, uint8_t h_size, uint8_t direction, uint8_t full_or_half, uint8_t buf[][16], uint8_t scroll_count, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t red, uint8_t green, uint8_t blue){
  uint8_t Dot = (red << 5) | (green << 2) | blue;
  uint8_t i, j, k;
  uint8_t bbb = 0b10000000;
  uint8_t color_2bytes[8 * full_or_half * h_size] = {};
  uint8_t com[6];
  uint8_t x1 = x0 + 8 * full_or_half * h_size - 1;

  switch( direction ){
    case 0:
      copyDisplay(x0, y0+1, x1, y1, x0, y0);

      com[0] = 0x15; //Set Column Address
        com[1] = x0; com[2] = x1;
      com[3] = 0x75; //Set Row Address
        com[4] = y1; com[5] = y1;
      break;
    case 1:
      copyDisplay(x0, y0, x1, y1-1, x0, y0+1);

      com[0] = 0x15; //Set Column Address
        com[1] = x0; com[2] = x1;
      com[3] = 0x75; //Set Row Address
        com[4] = y0; com[5] = y0;
      break;
  }  

  writeSpiCommandBytes(com, 6);
  
  uint8_t Dot_write_cnt = 0;
  uint8_t f_buf = 0;

  switch( direction ){
    case 0:
      for(k=0; k<full_or_half; k++){
        if(reverse){
          f_buf = ~buf[k][scroll_count];
        }else{
          f_buf = buf[k][scroll_count];
        }
        for(i=0; i<8; i++){
          if( f_buf & (bbb >> i) ){
            for(j=0; j<h_size; j++){
              color_2bytes[Dot_write_cnt++] = Dot;
            }
          }else{
            for(j=0; j<h_size; j++){
              color_2bytes[Dot_write_cnt++] = 0;
            }
          }
        }
      }
      break;
    case 1:
      for(k=0; k<full_or_half; k++){
        if(reverse){
          f_buf = ~buf[k][15-scroll_count];
        }else{
          f_buf = buf[k][15-scroll_count];
        }
        for(i=0; i<8; i++){
          if( f_buf & (bbb >> i) ){
            for(j=0; j<h_size; j++){
              color_2bytes[Dot_write_cnt++] = Dot;
            }
          }else{
            for(j=0; j<h_size; j++){
              color_2bytes[Dot_write_cnt++] = 0;
            }
          }
        }
      }
      break;
  }

  writeSpiDataBytes(color_2bytes, sizeof(color_2bytes));
}