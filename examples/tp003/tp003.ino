#include <M304.h>

char *pgname = "M304jp TP003 Ver1.30";
void setup(void) {
  m304Init();
  delay(1000);
  digitalWrite(RLY8,LOW);
  digitalWrite(RLY1,LOW);
  delay(1000);
  digitalWrite(RLY8,HIGH);
  digitalWrite(RLY1,HIGH);
  lcd.setCursor(0,1);
  lcd.print("RLY MODE:");
  lcd.setCursor(12,2);
  lcd.print("87654321");
}

void loop(void) {
  int m,v;
  m = digitalRead(SW_RLY);
  lcd.setCursor(9,1);
  if ( m == LOW ) {
    lcd.print("MANUAL");
  } else {
    lcd.print("AUTO  ");
  }
  v = analogRead(SELECT_VR);
  v = map(v,0,1023,0,255);
  lcd.setCursor(0,2);
  lcd.print("VALUE:");
  lcd.print(v,16);
  for(int i=0;i<8;i++) {
    lcd.setCursor(19-i,3);
    if (bitRead(v,i)==0) {
      lcd.print("o");
      digitalWrite(22+i,HIGH);
    } else {
      lcd.print("C");
      digitalWrite(22+i,LOW);
    }
  }
}
