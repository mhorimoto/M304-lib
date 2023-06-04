#include <M304.h>

int tpn=0 ;
char *pgname = "M304jp TP001 Ver1.10";

void setup(void) {
  m304Init();
}

void loop(void) {
  int t ;
  switch(tpn) {
  case 0:
    lcd.setCursor(0, 1);
    t = millis() / 1000;
    lcd.print(t);
    if (t>10) tpn++;
    break;
  case 1:
    lcd.setCursor(0,1);
    lcd.print("AutoScroll test");
    lcd.setCursor(0,2);
    for (int thisChar = 0; thisChar < 10; thisChar++) {
      lcd.print(thisChar);
      delay(500);
    }
    lcd.setCursor(20,3);
    lcd.autoscroll();
    for (int thisChar = 0; thisChar < 10; thisChar++) {
      lcd.print(thisChar);
      delay(500);
    }
    lcd.noAutoscroll();
    lcd.clear();
  }
}
