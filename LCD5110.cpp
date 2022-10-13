#include "LCD5110.h"

LCD5110::LCD5110(uint8_t _rst, uint8_t _ce, uint8_t _dc, uint8_t _din, uint8_t _clk) {
    rst = _rst;
    ce  = _ce;
    dc  = _dc;
    din = _din;
    clk = _clk;

    pinMode(rst, OUTPUT);
    pinMode(ce,  OUTPUT);
    pinMode(dc,  OUTPUT);
    pinMode(din, OUTPUT);
    pinMode(clk, OUTPUT);

 }

void LCD5110::begin(){
   //_cursor = Cursor();

    doReset();
    sendCommand(INSTRUCTIONSET | SET_EXT);
    // set temp control
    sendCommand(SET_TEMP | DEFAULT_TEMP);
    // set bias
    sendCommand(SET_BIAS | DEFAULT_BIAS);
    // set contrast
    sendCommand(SET_CONTRAST | DEFAULT_CONTRAST); 
    sendCommand(INSTRUCTIONSET | SET_BASIC );
    // Displaymode normal
    sendCommand(DISPLAYMODENORMAL); 
    
    on();
    clear();
    draw();
    cursor_x = 0;
    cursor_y = 0; 
}

void LCD5110::doReset(){
    digitalWrite(rst, LOW);
    delay(500);
    digitalWrite(rst, HIGH);
}

void LCD5110::sendCommand(uint8_t command){
    digitalWrite(dc, LOW);
    send(command);
}

void LCD5110::sendData(uint8_t data){
    digitalWrite(dc, HIGH);
    send(data);
}

void LCD5110::send(uint8_t data){
    digitalWrite(ce, LOW);
    shiftOut(din, clk, MSBFIRST, data);
    digitalWrite(ce, HIGH);
    
}

void LCD5110::clear(){
    for (uint8_t y=0; y < BUFFER_ROW; y++) for (uint8_t x=0; x < DISPLAY_COL; x++) framebuffer[x][y] = 0;
    cursor_x = 0;
    cursor_y = 0;
}

void LCD5110::on(){
    sendCommand(INSTRUCTIONSET | POWERUP);
}

void LCD5110::off(){
    sendCommand(INSTRUCTIONSET | POWERDOWN);
}

void LCD5110::setDisplayMode(displaymode_t mode){
    sendCommand(INSTRUCTIONSET | SET_BASIC);
    sendCommand(mode);
}


void LCD5110::setBiasSystem(uint8_t rate){
    sendCommand(INSTRUCTIONSET | SET_EXT);
    sendCommand( SET_BIAS | (rate & 0b00000111) );
    sendCommand(INSTRUCTIONSET | SET_BASIC);
}

void LCD5110::setContrast(uint8_t value){
    sendCommand(INSTRUCTIONSET | SET_EXT);
    sendCommand(SET_CONTRAST | value);
    sendCommand(INSTRUCTIONSET | SET_BASIC);
}

void LCD5110::setFont(font_t _myfont){
    myfont = _myfont;
}

void LCD5110::setCursor(uint8_t x, uint8_t y){
    cursor_x = x;
    cursor_y = y;
}

void LCD5110::getFBYcoord(uint8_t cy, uint8_t *p_displayRow, uint8_t *p_cy){
    uint8_t displayRow=0;
    while ( cy >= 8 ) {
        cy -= 8;
        displayRow++;
    }
    *p_displayRow = displayRow;
    *p_cy = cy;
}

size_t LCD5110::write(uint8_t c){ 
    size_t retval=0;
//    uint8_t cx;
    uint8_t cy;
    uint8_t displayRow;
    uint8_t displayData;
    getFBYcoord(cursor_y, &displayRow, &cy);
	// CF und LF sorgen für Zeilenumbruch
	if (c == 10 || c == 13) {
        cursor_x=0;	
        switch (myfont) {
            case small:  
                cursor_y+=8;
            break;
            case medium:  
                cursor_y+=FONT_MEDIUM_Y;
            break;
            case big:  
                cursor_y+=FONT_BIG_Y;
            break;
        }
    }
	if (myfont == small && c > 31 && c < 127) {
		retval = 1;
        for(byte i = 0; i < 7; i++) {
            if (cursor_x + i < DISPLAY_COL) {
                if ( i<5 ) {
                    displayData=pgm_read_byte(&myFont8x5[c - 32][i]);
                    framebuffer[cursor_x+i][displayRow] &= ~(0xFF << cy);
                    framebuffer[cursor_x+i][displayRow] |= (displayData << cy);
                    if (displayRow < 6) {
                        framebuffer[cursor_x+i][displayRow+1] &= ~(0xFF >> (8 - cy));
                        framebuffer[cursor_x+i][displayRow+1] |= (displayData >> (8 - cy));
                    }
                } else {
                    framebuffer[cursor_x+i][displayRow] &= ~(0xFF << cy);
                    if (displayRow < 6) {
                        framebuffer[cursor_x+i][displayRow+1] &= ~(0xFF >> (8 - cy));
                    }
                }
            }
        }
		cursor_x += 7;
	}
	if ((myfont == big || myfont == medium) && ((c > 47 && c < 58) || c == 44 || c == 45 || c == 46 || c == 32 || c == 42 || c == 37)) {
		retval = 1;
        if ((c > 47 && c < 58) || c == 37) {
            if (myfont == big) {
                drawRect(cursor_x, cursor_y, FONT_BIG_X, FONT_BIG_Y,false,true,false);
            } else {
                drawRect(cursor_x, cursor_y, FONT_MEDIUM_X, FONT_MEDIUM_Y,false,true,false);
            }
        }
        switch (c) {
            case 37:  // %
                if (myfont == big) {
                    for (uint8_t x=0; x<7; x++) setPixel(cursor_x+x,cursor_y+0);
                    setPixel(cursor_x+0,cursor_y+1);
                    setPixel(cursor_x+0,cursor_y+2);
                    setPixel(cursor_x+0,cursor_y+3);
                    setPixel(cursor_x+0,cursor_y+4);
                    setPixel(cursor_x+6,cursor_y+1);
                    setPixel(cursor_x+6,cursor_y+2);
                    setPixel(cursor_x+6,cursor_y+3);
                    setPixel(cursor_x+6,cursor_y+4);
                    for (uint8_t x=0; x<7; x++) setPixel(cursor_x+x,cursor_y+5);
                    for (uint8_t x=7; x<13; x++) setPixel(cursor_x+x,cursor_y+17);
                    setPixel(cursor_x+7,cursor_y+18);
                    setPixel(cursor_x+7,cursor_y+19);
                    setPixel(cursor_x+7,cursor_y+20);
                    setPixel(cursor_x+7,cursor_y+21);
                    setPixel(cursor_x+12,cursor_y+18);
                    setPixel(cursor_x+12,cursor_y+19);
                    setPixel(cursor_x+12,cursor_y+20);
                    setPixel(cursor_x+12,cursor_y+21);
                    for (uint8_t x=7; x<13; x++) setPixel(cursor_x+x,cursor_y+22);
                    setPixel(cursor_x+12,cursor_y+0);
                    setPixel(cursor_x+12,cursor_y+1);
                    setPixel(cursor_x+11,cursor_y+2);
                    setPixel(cursor_x+11,cursor_y+3);
                    setPixel(cursor_x+10,cursor_y+4);
                    setPixel(cursor_x+10,cursor_y+5);
                    setPixel(cursor_x+9,cursor_y+6);
                    setPixel(cursor_x+9,cursor_y+7);
                    setPixel(cursor_x+8,cursor_y+8);
                    setPixel(cursor_x+8,cursor_y+9);
                    setPixel(cursor_x+7,cursor_y+10);
                    setPixel(cursor_x+6,cursor_y+11);
                    setPixel(cursor_x+5,cursor_y+12);
                    setPixel(cursor_x+4,cursor_y+13);
                    setPixel(cursor_x+4,cursor_y+14);
                    setPixel(cursor_x+3,cursor_y+15);
                    setPixel(cursor_x+3,cursor_y+16);
                    setPixel(cursor_x+2,cursor_y+17);
                    setPixel(cursor_x+2,cursor_y+18);
                    setPixel(cursor_x+1,cursor_y+19);
                    setPixel(cursor_x+1,cursor_y+20);
                    setPixel(cursor_x+0,cursor_y+21);
                    setPixel(cursor_x+0,cursor_y+22);
                    cursor_x += FONT_BIG_X; 
                } else {
                    for (uint8_t x=0; x<5; x++) setPixel(cursor_x+x,cursor_y+0);
                    setPixel(cursor_x+0,cursor_y+1);
                    setPixel(cursor_x+0,cursor_y+2);
                    setPixel(cursor_x+4,cursor_y+1);
                    setPixel(cursor_x+4,cursor_y+2);
                    for (uint8_t x=0; x<5; x++) setPixel(cursor_x+x,cursor_y+3);
                    for (uint8_t x=4; x<9; x++) setPixel(cursor_x+x,cursor_y+10);
                    setPixel(cursor_x+4,cursor_y+11);
                    setPixel(cursor_x+4,cursor_y+12);
                    setPixel(cursor_x+8,cursor_y+11);
                    setPixel(cursor_x+8,cursor_y+12);
                    for (uint8_t x=4; x<9; x++) setPixel(cursor_x+x,cursor_y+13);
                    setPixel(cursor_x+8,cursor_y+0);
                    setPixel(cursor_x+8,cursor_y+1);
                    setPixel(cursor_x+7,cursor_y+2);
                    setPixel(cursor_x+7,cursor_y+3);
                    setPixel(cursor_x+6,cursor_y+4);
                    setPixel(cursor_x+5,cursor_y+5);
                    setPixel(cursor_x+4,cursor_y+6);
                    setPixel(cursor_x+4,cursor_y+7);
                    setPixel(cursor_x+3,cursor_y+8);
                    setPixel(cursor_x+2,cursor_y+9);
                    setPixel(cursor_x+1,cursor_y+10);
                    setPixel(cursor_x+1,cursor_y+11);
                    setPixel(cursor_x+0,cursor_y+12);
                    setPixel(cursor_x+0,cursor_y+13);
                    cursor_x += FONT_MEDIUM_X;
                }
                break;
           case 48:  // 0
               segment_a(cursor_x, cursor_y, myfont);
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               segment_e(cursor_x, cursor_y, myfont);
               segment_f(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 49:  // 1
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 50:  // 2
               segment_a(cursor_x, cursor_y, myfont);
               segment_b(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               segment_e(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 51:  // 3
               segment_a(cursor_x, cursor_y, myfont);
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 52:  // 4
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               segment_f(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 53:  // 5
               segment_a(cursor_x, cursor_y, myfont);
               segment_f(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 54:  // 6
               segment_a(cursor_x, cursor_y, myfont);
               segment_f(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               segment_e(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 55:  // 7
               segment_a(cursor_x, cursor_y, myfont);
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 56:  // 8
               segment_a(cursor_x, cursor_y, myfont);
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               segment_e(cursor_x, cursor_y, myfont);
               segment_f(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 57:  // 9
               segment_a(cursor_x, cursor_y, myfont);
               segment_b(cursor_x, cursor_y, myfont);
               segment_c(cursor_x, cursor_y, myfont);
               segment_d(cursor_x, cursor_y, myfont);
               segment_f(cursor_x, cursor_y, myfont);
               segment_g(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 32:
               if (myfont == big) cursor_x += FONT_BIG_X; else cursor_x += FONT_MEDIUM_X;
               break;
           case 44:  // , (Komma)
           case 46:  // . (Punkt)
               segment_p(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += 5; else cursor_x += 4;
               break;
           case 45:  // -
               segment_g(cursor_x, cursor_y, myfont);
               if (myfont == big) cursor_x += 12; else cursor_x += 8;
               break;   
           case 42: // * (genutzt als °)
               if (myfont == big) {
                  drawRect(cursor_x,cursor_y+1,3,4,true,true,false);
                  cursor_x += 5;
               } else {
                  drawRect(cursor_x,cursor_y+1,2,3,true,true,false);
                  cursor_x += 4;
               }
               break;
       }
    }
#ifdef ESP8266
    delay(0);
#endif    
	return retval; 
}

void LCD5110::segment_a(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t x=0; x<13; x++) setPixel(_x+x, _y);
    for (uint8_t x=1; x<12; x++) setPixel(_x+x, _y+1);
    for (uint8_t x=2; x<11; x++) setPixel(_x+x, _y+2);
  }
  if (f == medium) {
    for (uint8_t x=0; x<8; x++) setPixel(_x+x, _y);
    for (uint8_t x=1; x<7; x++) setPixel(_x+x, _y+1);
  }
}

void LCD5110::segment_b(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t y=1; y<11; y++) setPixel(_x+12, _y+y);
    for (uint8_t y=2; y<10; y++) setPixel(_x+11, _y+y);
    for (uint8_t y=3; y<9;  y++) setPixel(_x+10, _y+y);
  }
  if (f == medium) {
    for (uint8_t y=1; y<7; y++) setPixel(_x+7, _y+y);
    for (uint8_t y=2; y<7; y++) setPixel(_x+6, _y+y);
  }
}

void LCD5110::segment_c(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t y=12; y<22; y++) setPixel(_x+12, _y+y);
    for (uint8_t y=13; y<21; y++) setPixel(_x+11, _y+y);
    for (uint8_t y=14; y<20; y++) setPixel(_x+10, _y+y);
  }
  if (f == medium) {
    for (uint8_t y=7; y<13; y++) setPixel(_x+7, _y+y);
    for (uint8_t y=8; y<12; y++) setPixel(_x+6, _y+y);
  }
}

void LCD5110::segment_d(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t x=2; x<11; x++) setPixel(_x+x, _y+20);
    for (uint8_t x=1; x<12; x++) setPixel(_x+x, _y+21);
    for (uint8_t x=0; x<13; x++) setPixel(_x+x, _y+22);
  }
  if (f == medium) {
    for (uint8_t x=1; x<7; x++) setPixel(_x+x, _y+12);
    for (uint8_t x=0; x<8; x++) setPixel(_x+x, _y+13);
  }
}

void LCD5110::segment_e(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t y=12; y<22; y++) setPixel(_x,   _y+y);
    for (uint8_t y=13; y<21; y++) setPixel(_x+1, _y+y);
    for (uint8_t y=14; y<20; y++) setPixel(_x+2, _y+y);
  }
  if (f == medium) {
    for (uint8_t y=7; y<13; y++) setPixel(_x,   _y+y);
    for (uint8_t y=8; y<12; y++) setPixel(_x+1, _y+y);
  }
}

void LCD5110::segment_f(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t y=1; y<11; y++) setPixel(_x,   _y+y);
    for (uint8_t y=2; y<10; y++) setPixel(_x+1, _y+y);
    for (uint8_t y=3; y<10; y++) setPixel(_x+2, _y+y);
  }
  if (f == medium) {
    for (uint8_t y=1; y<7; y++) setPixel(_x,   _y+y);
    for (uint8_t y=2; y<6; y++) setPixel(_x+1, _y+y);
  }
}

void LCD5110::segment_g(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for (uint8_t x=1; x<12; x++) setPixel(_x+x, _y+10);
    for (uint8_t x=0; x<13; x++) setPixel(_x+x, _y+11);
    for (uint8_t x=1; x<12; x++) setPixel(_x+x, _y+12);
  }
  if (f == medium) {
    for (uint8_t x=1; x<7; x++) setPixel(_x+x, _y+6);
    for (uint8_t x=1; x<7; x++) setPixel(_x+x, _y+7);
  }
}

void LCD5110::segment_p(uint8_t _x, uint8_t _y, font_t f){
  if (f == big) {
    for(uint8_t x=0; x<3; x++) for(uint8_t y=20; y<23; y++) setPixel(x+_x, y+_y);
  }
  if (f == medium) {  
    for(uint8_t x=0; x<2; x++) for(uint8_t y=12; y<14; y++) setPixel(x+_x, y+_y);
  }
}

void LCD5110::setPixel(uint8_t x, uint8_t y){
    uint8_t cy;
    uint8_t displayRow;
    if ( x < DISPLAY_COL && y < DISPLAY_ROW ) {
        getFBYcoord(y, &displayRow, &cy);
        framebuffer[x][displayRow] |= (1 << cy);
    }
}

void LCD5110::clrPixel(uint8_t x, uint8_t y){
    uint8_t cy;
    uint8_t displayRow;
    if ( x < DISPLAY_COL && y < DISPLAY_ROW ) {
        getFBYcoord(y, &displayRow, &cy);
        framebuffer[x][displayRow] &= ~(1 << cy);
    }
}

/*
 * Linie wird nicht benötigt, kann bei Bedarf aktiviert werden !!
 * 
void LCD5110::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
    uint8_t cy;
    uint8_t displayRow;
    // Linie eher vertikal
    if ( x2-x1 > y2-y1 ) {
        uint8_t dx=0;
        float dy=(y2-y1)/(float)(x2-x1);
        for(uint8_t x=x1; x<=x2; x++) {
            setPixel(x,(uint8_t)(y1+(dx*dy)));
            dx++;
        }
    } else {
    // Linie eher vertikal
        uint8_t dy=0;
        float dx=(x2-x1)/(float)(y2-y1);
        for(uint8_t y=y1; y<=y2; y++) {
            setPixel((uint8_t)(x1+(dy*dx)),y);
            dy++;
        }
    }        
}
*/

void LCD5110::drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    drawRect(x1,y1,x2,y2,true,false,false);
}

void LCD5110::drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool drawBorder, bool eraseInside, bool fillBlack) {
    if (eraseInside) {
        for(uint8_t x=0; x<=x2; x++) 
            for(uint8_t y=0; y<=y2; y++)
                if (fillBlack) setPixel(x1+x,y1+y); else clrPixel(x1+x,y1+y);
    }
    if (drawBorder) {
        for (uint8_t x=0; x<=x2; x++) setPixel(x+x1,y1);     // Obere Linie
        for (uint8_t x=0; x<=x2; x++) setPixel(x+x1,y1+y2);  // Untere Linie
        for (uint8_t y=0; y<=y2; y++) setPixel(x1,y+y1);     // Linke Linie
        for (uint8_t y=0; y<=y2; y++) setPixel(x1+x2,y+y1);  // Rechte Linie
    }
}

void LCD5110::draw(void){
    sendCommand(0b01000000); // First line 
    sendCommand(0b10000000); // First Column
    for (uint8_t y=0; y < BUFFER_ROW; y++) for (uint8_t x=0; x < DISPLAY_COL; x++) sendData(framebuffer[x][y]);

    
}
