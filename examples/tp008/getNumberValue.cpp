#include <M304.h>

bool getNumberValue(int x0,int y0,uint8_t base,long min,long max,uint8_t cast,uint8_t fkey,uint8_t akey,long *lgval) {
  char lcdtxt[21],*ptr_lcdtxt,fmt[10];
  int  ia;
  long lv;
  struct KYBDMEM *ptr_ck;
  extern struct KYBDMEM *getCrossKey(void);

  ptr_lcdtxt = &lcdtxt[0];
  switch(base) {
  case 16:
    sprintf(fmt,"%%%dX",cast);
    break;
  default:
    sprintf(fmt,"%%%dd",cast);
    break;
  }

  do {
    ptr_ck = getCrossKey();
    lv = map((long)(ptr_ck->selpos),0,1023,0,255);
    lcd.setCursor(x0,y0);
    sprintf(lcdtxt,fmt,lv);
    lcd.print(ptr_lcdtxt);
  } while(ptr_ck->kpos==0);
  if ((ptr_ck->kpos&K_ENT)==K_ENT) {
    *lgval = (long)lv;
    return(TRUE);
  } else {
    return(FALSE);
  }
}

