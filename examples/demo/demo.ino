#include "LCD5110.h"

#define RST 2
#define CE 4
#define DC 5
#define DIN 6
#define CLK 7
#define LIGHT 3

LCD5110 lcd = LCD5110(RST, CE, DC, DIN, CLK);

void setup() {
  lcd.begin();
  
  pinMode(LIGHT, OUTPUT);
  digitalWrite( LIGHT , LOW);
  delay(1000);
  lcd.setCursor(0,10);
  lcd.println("   LCD");
  lcd.println("   5110");
  lcd.println("Displaytest");
  lcd.draw();
  delay(5000); 
  digitalWrite( LIGHT , HIGH);
  delay(200); 
  digitalWrite( LIGHT , LOW);
  lcd.clear();
  lcd.setCursor(0,20);
  lcd.println("Display");
  lcd.println("   blank ");
  lcd.draw();
  delay(200); 
  lcd.setDisplayMode(LCD5110::blank);
  delay(1000); 
  lcd.setCursor(0,20);
  lcd.println("Display");
  lcd.println("   invers");
  lcd.draw();
  lcd.setDisplayMode(LCD5110::invers);
  delay(1000); 
  lcd.setCursor(0,20);
  lcd.println("Display");
  lcd.println("   allon ");
  lcd.draw();
  delay(200); 
  lcd.setDisplayMode(LCD5110::allon);
  delay(1000); 
  lcd.setCursor(0,20);
  lcd.println("Display");
  lcd.println("   normal");
  lcd.draw();
  lcd.setDisplayMode(LCD5110::normal);
  delay(1000); 
  lcd.clear();
  lcd.setCursor(0,20);
  lcd.println("Display");
  lcd.println("goto sleep");
  lcd.draw();
  delay(200); 
  lcd.off();
  delay(2000); 
  lcd.on();
  lcd.clear();
  lcd.setCursor(0,20);
  lcd.println("Display");
  lcd.println("on again");
  lcd.draw();
  delay(1000); 
  lcd.clear();
  lcd.setCursor(0,10);
  lcd.println("Contrast");
  lcd.println("from");
  lcd.println("40 to 80");
  lcd.draw();
  delay(1000); 
  lcd.clear();
  for (uint8_t i=40; i<81; i++) { 
    lcd.setContrast(i);
    lcd.setCursor(0,20);
    lcd.println("Contrast:");
    lcd.print("   ");
    lcd.println(i); 
    lcd.draw();
    delay(200); 
  }
  lcd.setContrast(65);
  delay(1000); 
  
  digitalWrite( LIGHT , HIGH);
  delay(200); 
  digitalWrite( LIGHT , LOW);
  lcd.clear();    
  lcd.println("Ein beliebiger Text");
  lcd.println("println() ");
  lcd.println("erzeugt ");
  lcd.println("Zeilenumbruch");
  lcd.print("eins ");
  lcd.println("zwei ");
  lcd.draw();
  delay(3000);
  lcd.clear();    
  float f=123.456;
  lcd.println(f,1);
  lcd.println("eine");
  lcd.println("Nachkommastelle");
  lcd.println(f,3);
  lcd.println("drei");
  lcd.println("Nachkommastellen");
  lcd.draw();
  delay(3000);
  lcd.clear();    
  lcd.drawRect(10,10,50,10);
  lcd.setCursor(12,12);
  lcd.print("Rahmen");
  lcd.draw();
  delay(1000);
  lcd.clear();    
  lcd.drawRect(0,0,70,30,false,true,true);
  lcd.setCursor(0,35);
  lcd.print("gefuellt");
  lcd.draw();
  delay(1000);
  lcd.drawRect(20,15,20,10,false,true,false);
  lcd.drawRect(5,5,10,5,false,true,false);
  lcd.drawRect(50,10,10,10,false,true,false);
  lcd.setCursor(0,35);
  lcd.print("geloescht");
  lcd.draw();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
