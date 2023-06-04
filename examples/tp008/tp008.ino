#include <M304.h>
//#include "crosskey.h"

//struct KYBDMEM {
//  uint8_t   kpos;
//  uint16_t  selpos;
//} ;

char *pgname = "M304jp TP008 Ver2.03";
uint8_t mode;

void setup(void) {
  m304Init();
  mode = 0;
}

void loop(void) {
  struct KYBDMEM *ptr_ck;
  long *ptr_va,va;
  char lcdtxt[21],*ptr_lcdtxt;
  extern bool getNumberValue(int,int,uint8_t,long,long,uint8_t,uint8_t,uint8_t,long *);
  extern struct KYBDMEM *getCrossKey(void);
  ptr_va = &va;
  switch(mode) {
  case K_RIGHT:
    if (getNumberValue(0,1,10,0,255,3,K_ENT,K_RIGHT,ptr_va)==TRUE) {
      mode = 0;
    }
    break;
  default:
    ptr_lcdtxt = &lcdtxt[0];
    ptr_ck = getCrossKey();
    sprintf(lcdtxt,"KEY: %02X  ",ptr_ck->kpos);
    lcd.setCursor(0,1);
    lcd.print(ptr_lcdtxt);
    sprintf(lcdtxt,"VOL: %03X  ",ptr_ck->selpos);
    lcd.setCursor(0,2);
    lcd.print(ptr_lcdtxt);
    mode = ptr_ck->kpos;
    break;
  }
}

