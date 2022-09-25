/*********************************************************************
This is a library for the Noika monochrome LCD based on the PCD8544 driver.

Implemented functions see below!

**********************************************************************/

#ifndef _LCD5110_H_
#define _LCD5110_H_

#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif

#include <stdlib.h>

#include "Arduino.h"
#include "Print.h"
#include "LCD5110_font.h"

// Display Modes
#define DISPLAYMODEBLANK      0b00001000
#define DISPLAYMODENORMAL     0b00001100
#define DISPLAYMODEALLSEGON   0b00001001
#define DISPLAYMODEINVERSE    0b00001101

// Instruction Set
#define INSTRUCTIONSET        0b00100000
#define SET_EXT               0b00000001
#define SET_BASIC             0b00000000
#define POWERDOWN             0b00000100
#define POWERUP               0b00000000
#define ADRESS_VERT           0b00000010
#define ADRESS_HORI           0b00000000

#define SET_TEMP              0b00000100
#define SET_BIAS              0b00010000
#define SET_CONTRAST          0b10000000
#define DEFAULT_TEMP          0b00000001
#define DEFAULT_BIAS          0b00000011
#define DEFAULT_CONTRAST      65

// Constants
#define DISPLAY_COL           84
#define DISPLAY_ROW           48
#define BUFFER_ROW            6
#define FONT_SMALL_X          5
#define FONT_MEDIUM_X         10
#define FONT_MEDIUM_Y         16
#define FONT_BIG_X            15
#define FONT_BIG_Y            24



class LCD5110 : public Print {

    public:
        enum font_t { small=0, medium=1, big=2 };
        enum displaymode_t { blank = DISPLAYMODEBLANK, normal = DISPLAYMODENORMAL, allon = DISPLAYMODEALLSEGON, invers = DISPLAYMODEINVERSE }; 

    private:
// Anschlußpinne für das Display
        uint8_t rst;
        uint8_t ce;
        uint8_t dc;
        uint8_t din;
        uint8_t clk;
// ENDE Anschlußpinne für das Display
// Der Framebuffer ist ein Zwischenspeicher für die Displaydaten.
// Grund: Sind die Daten einmal an das Display übergeben, können sie nicht mehr gelesen werden
//        Deshalb ist es direkt nicht möglich ein einzelnes Pixel zu setzen
		uint8_t   framebuffer[DISPLAY_COL][BUFFER_ROW];        
// Die Druckausgabe des Displays wird durch den Cusor gesteuert.
// Die obere, linke Ecke hat die Koordinaten 0,0        
        uint8_t cursor_x;
        uint8_t cursor_y;
// Eine Variable für den aktuell genutzten Font        
        font_t myfont;
// Methode zum Drucken eines Zeichens.
// Diese Methode wird zwingend benötigt damit die .print() und .println() Funktion genutzt werden kann        
	size_t write(uint8_t c);
// Führt einen HW-Reset durch        
        void doReset(void);
// Funktion um (Display-)Daten zu senden        
        void sendData(uint8_t data);
// Funktion um ein Kommando zu senden        
        void sendCommand(uint8_t command);
// Allgemeine Sendefunktion, wird nur indirekt verwendet
        void send(uint8_t data);
// Ermittelt die Y-Koordinaten innerhalb des Framebuffers
// cy übergibt den "cursor_y", 
// p_displayRow ist ein Pointer auf die Displayreihe ( kann leer übergeben werde)
// p_cy ist ein Pointer dessen Inhalt für einen Verschiebevorgang genutzt wird ( kann leer übergeben werde)
// Nutzung: 
//    uint8_t cy;
//    uint8_t displayRow;
//    getFBYcoord(y, &displayRow, &cy);
//    framebuffer[x][displayRow] |= (1 << cy);
//    ==> Setzt ein Pixel an der Position x,y        
        void getFBYcoord(uint8_t cy, uint8_t *p_displayRow, uint8_t *p_cy);
// Folgende Segmente werden für den "medium" und "big" Font genutzt. Hier wird ein 7 Segment Display als Vorlage genutzt
// Segmentbezeichnung ist wie folgt:
//
//    ==A
// F |  | B
//    ==G
// E |  | C
//    ==D
// P ist der Dezimalpunkt   
// 
// Die Abmessungen (x * y):
// big:     12 * 24 Pixel
// medium:   8 * 16 Pixel
// Segment Layout   MEDIUM                               BIG
//  0123456789012345678901234567890123456789012345678901234567890123456789
//
//0 AAAAAAAA  AAAAAAAAAAAAA       XXXXX   X XXXXXXX     X
//1 FAAAAAAB  FAAAAAAAAAAAB       X...X   X X.....X     X
//2 FF....BB  FFAAAAAAAAABB       X...X  X  X.....X    X
//3 FF....BB  FFF.......BBB       XXXXX  X  X.....X    X
//4 FF....BB  FFF.......BBB             X   X.....X   X 
//5 FF....BB  FFF.......BBB            X    XXXXXXX   X
//6 FGGGGGGB  FFF.......BBB           X              X
//7 EGGGGGGC  FFF.......BBB           X              X
//8 EE....CC  FFF.......BBB          X              X
//9 EE....CC  FFF.......BBB         X               X
//0 EE....CC  FGGGGGGGGGGGB        X  XXXXX        X
//1 EE....CC  GGGGGGGGGGGGG        X  X...X       X
//2 EDDDDDDC  EGGGGGGGGGGGC       X   X...X      X
//3 DDDDDDDD  EEE.......CCC       X   XXXXX     X
//4           EEE.......CCC                     X
//5           EEE.......CCC                    X
//6           EEE.......CCC                    X
//7           EEE.......CCC                   X    XXXXXXX
//8           EEE.......CCC                   X    X.....X
//9           EEE.......CCC                  X     X.....X
//0           EEDDDDDDDDDCC                  X     X.....X
//1           EDDDDDDDDDDDC                 X      X.....X
//2           DDDDDDDDDDDDD                 X      XXXXXXX 
//3

//
        void segment_a(uint8_t x, uint8_t y, font_t f=big);
        void segment_b(uint8_t x, uint8_t y, font_t f=big);
        void segment_c(uint8_t x, uint8_t y, font_t f=big);
        void segment_d(uint8_t x, uint8_t y, font_t f=big);
        void segment_e(uint8_t x, uint8_t y, font_t f=big);
        void segment_f(uint8_t x, uint8_t y, font_t f=big);
        void segment_g(uint8_t x, uint8_t y, font_t f=big);
        void segment_p(uint8_t x, uint8_t y, font_t f=big);

    public:
// Der Konstruktor    
        LCD5110(uint8_t _rst, uint8_t _ce, uint8_t _dc, uint8_t _din, uint8_t _clk);
// Funktion zum Setzen des Displaymodes.
// Gültige Modi sind die Elemente von displaymode_t
        void setDisplayMode(displaymode_t mode);
// Gültige Modi siehe Datenblatt
        void setBiasSystem(uint8_t rate);
// Setzt den Kontrast des Displays
        void setContrast(uint8_t value);
        void setFont(font_t _myfont);
// Löscht den FrameBuffer        
        void clear(void);
// Initialisierung des Displays        
        void begin(void);
// Überträgt den FrameBuffer an das Display        
        void draw(void);
// Schaltet das Display an        
        void on(void);
// Schaltet das Display aus / in den Sleep Mode        
        void off(void);
//Setzt des Cursor an eine Position        
        void setCursor(uint8_t x, uint8_t y);
//Setzt ein Pixel an einer Position        
        void setPixel(uint8_t x, uint8_t y);
//Löscht ein Pixel an einer Position        
        void clrPixel(uint8_t x, uint8_t y);
        //Zeichnet eine Linie von (x1,Y1) nach (x2/y2) ==> deaktiviert, wird nicht benötigt
        //void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
// Zeichnet ein Rechteck (generische Funktion mit folgenden Ausprägungen:
// 1. Ausprägung: Zeichne nur die Umrandung        
// (x1/y1)-------(x2/y1)  (x2/y1)-------(x2/y2)  (x2/y2)-------(x2/y1)  (x2/y1)-------(x1/y1)        
// Der Inhalt des Rechtecks bleibt dabei unverändert
// Aufruf: drawRect(x1, y1, x2, y2);
        void drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
// 2. Ausprägung: Zeichne die Umrandung und lösche den Inhalt.
// Im Inhalt werden alle Pixel gelöscht.
// Aufruf: drawRect(x1, y1, x2, y2, true, true, false);
// 3. Ausprägung: Zeichne ein ausgefülltes Rechteck (komplett schwarz)
// Aufruf: drawRect(x1, y1, x2, y2, true, true, true);
// 4. Ausprägung: Lösche einen Bereich im Display
// Aufruf: drawRect(x1, y1, x2, y2, false, true, false);
        void drawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool drawBorder, bool eraseInside, bool fillBlack);
};





#endif /* _LCD5110_H_ */
