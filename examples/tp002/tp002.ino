#include <M304.h>

char *pgname = "M304jp TP002 Ver2.40";

void setup(void) {
  m304Init();
  lcd.setCursor(8,1);
  lcd.print("SAFE:");
  lcd.setCursor(8,2);
  lcd.print(" RLY:");
  lcd.setCursor(8,3);
  lcd.print("SELE:");
}

void loop(void) {
  int e,u,d,l,r,s,o,a;
  e = digitalRead(SW_ENTER);
  u = digitalRead(SW_UP);
  d = digitalRead(SW_DOWN);
  l = digitalRead(SW_LEFT);
  r = digitalRead(SW_RIGHT);
  s = digitalRead(SW_SAFE);
  o = digitalRead(SW_RLY);
  a = analogRead(SELECT_VR);
  
  lcd.setCursor(3,2);
  if ( e == LOW ) {
    lcd.print("*");
  } else {
    lcd.print(" ");
  }
  lcd.setCursor(3,1);
  if ( u == LOW ) {
    lcd.print("^");
  } else {
    lcd.print(" ");
  }
  lcd.setCursor(3,3);
  if ( d == LOW ) {
    lcd.print("v");
  } else {
    lcd.print(" ");
  }
  lcd.setCursor(2,2);
  if ( l == LOW ) {
    lcd.print("<");
  } else {
    lcd.print(" ");
  }
  lcd.setCursor(4,2);
  if ( r == LOW ) {
    lcd.print(">");
  } else {
    lcd.print(" ");
  }
  lcd.setCursor(13,1);
  if ( s == LOW ) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  lcd.setCursor(13,2);
  if ( o == LOW ) {
    lcd.print("MANUAL");
  } else {
    lcd.print("AUTO  ");
  }
  lcd.setCursor(13,3);
  lcd.print(a);
  if (a<10) {
    lcd.setCursor(14,3);
    lcd.print("   ");
  } else if (a<100) {
    lcd.setCursor(15,3);
    lcd.print("  ");
  } else if (a<1000) {
    lcd.setCursor(16,3);
    lcd.print(" ");
  }
}
