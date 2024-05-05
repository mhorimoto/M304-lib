#define _M304_CPP_
#define _M304_CPP_V  1315
#include <M304.h>

extern char *pgname;
LiquidCrystal lcd(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);
AT24C256      atmem(0);
EthernetClient client;

stM304 st_m;

void m304Init(void) {
  int i;
  Ethernet.init(53);
  pinMode(SW_ENTER,INPUT_PULLUP);
  pinMode(SW_UP,INPUT_PULLUP);
  pinMode(SW_DOWN,INPUT_PULLUP);
  pinMode(SW_LEFT,INPUT_PULLUP);
  pinMode(SW_RIGHT,INPUT_PULLUP);
  pinMode(SW_SAFE,INPUT_PULLUP);
  pinMode(SW_RLY,INPUT);
  pinMode(SELECT_VR,INPUT);
  pinMode(RLY1,OUTPUT);
  digitalWrite(RLY1,HIGH);
  pinMode(RLY2,OUTPUT);
  digitalWrite(RLY2,HIGH);
  pinMode(RLY3,OUTPUT);
  digitalWrite(RLY3,HIGH);
  pinMode(RLY4,OUTPUT);
  digitalWrite(RLY4,HIGH);
  pinMode(RLY5,OUTPUT);
  digitalWrite(RLY5,HIGH);
  pinMode(RLY6,OUTPUT);
  digitalWrite(RLY6,HIGH);
  pinMode(RLY7,OUTPUT);
  digitalWrite(RLY7,HIGH);
  pinMode(RLY8,OUTPUT);
  digitalWrite(RLY8,HIGH);
  lcd.begin(20,4);
  lcd.print(pgname);
  for ( i=0;i<6;i++ ) {
    st_m.mac[i] = atmem.read(LC_MAC+i);
  }
}

/*********************************/
/* Cross Key Switch sub-routine  */
/*********************************/
struct KYBDMEM crosskey,*ptr_crosskey;
struct KYBDMEM *getCrossKey(void) {
  int CancelChattering(int);
  int e,u,d,l,r,a;
  e = CancelChattering(SW_ENTER);
  u = CancelChattering(SW_UP);
  d = CancelChattering(SW_DOWN);
  l = CancelChattering(SW_LEFT);
  r = CancelChattering(SW_RIGHT);
  a = analogRead(SELECT_VR);
  // crosskey.kpos = 0;
  if (e==LOW) crosskey.kpos |= K_ENT;
  if (u==LOW) crosskey.kpos |= K_UP;
  if (d==LOW) crosskey.kpos |= K_DOWN;
  if (l==LOW) crosskey.kpos |= K_LEFT;
  if (r==LOW) crosskey.kpos |= K_RIGHT;
  crosskey.selpos = a;
  ptr_crosskey = &crosskey;
  return(ptr_crosskey);
}

int CancelChattering(int s) {
  unsigned long gauge=0;
  bool debugMsgFlag(int);
  gauge=0;
  while(!digitalRead(s)) {
    crosskey.longf = false;
    gauge++;
  }
  if ( gauge > PUSH_LONG ) {
    crosskey.longf = true;
    if (debugMsgFlag(SO_MSG)) {
      Serial.print("LONG=");
      Serial.println(gauge);
    }
    return(LOW);
  }
  if ( gauge > PUSH_SHORT ) {
    crosskey.longf = false;
    if (debugMsgFlag(SO_MSG)) {
      Serial.print("SHORT=");
      Serial.println(gauge);
    }
    return(LOW);
  }
  return(HIGH);
}

void debug_print(void) {
  char lbf[80];
  int i;
  sprintf(lbf,"MAC:%02X%02X.%02X%02X.%02X%02X",st_m.mac[0],st_m.mac[1],st_m.mac[2],st_m.mac[3],st_m.mac[4],st_m.mac[5]);
  Serial.println(lbf);
  Serial.print("IP:");
  Serial.println(st_m.ip);
  Serial.print("GW:");
  Serial.println(st_m.gw);
  Serial.print("DNS:");
  Serial.println(st_m.dns);
}

bool debugMsgFlag(int a) {
  byte d;
  d = atmem.read(LC_DBGMSG);
  if ((d&a)==a) return true;
  return false;
}

void LCDd::initWriteArea(int p) {
  int x,y;
  if (p >= PAGECNT) return;
  for(y=0;y<4;y++) {
    for(x=0;x<20;x++) {
      LCDd::tarea[p][x][y] = ' ';
    }
  }
}

void LCDd::LineWrite(int p,int y) {
  int x;
  if (y>3) return; 
  if (p >= PAGECNT) return;
  LCDd::setCursor(0,y);
  for(x=0;x<20;x++) {
    LCDd::print(LCDd::tarea[p][x][y]);
  }
}

void LCDd::PageWrite(int p) {
  int y;
  if (p >= PAGECNT) return;
  for (y=0;y<4;y++) {
    LCDd::LineWrite(p,y);
  }
}

void LCDd::CharWrite(int p,int x,int y,char a) {
  if (p >= PAGECNT) return;
  if (LCDd::setWriteChar(p,x,y,a)) {
    LCDd::setCursor(x,y);
    LCDd::print(a);
  }
}
  
int LCDd::setWriteChar(int p,int x,int y,char a) {
  if (p >= PAGECNT) return;
  if ((x<20)&&(y<4)) {
    LCDd::tarea[p][x][y] = a;
    return 1;
  }
  return 0;
}


int LCDd::setLine(int p,int y,char *a) {
  int i;
  if (p>=PAGECNT) return(-1);
  if ((y<0)||(y>3)) return(-2);
  i=0;
  while(*a>0x1f) {
    LCDd::tarea[p][i][y] = *a;
    i++;
    a++;
    if (i>=20) break;
  }
  return(0);
}

int LCDd::IntRead(int p,int x,int y,int w) {
  char c[6];
  int i;
  if (p>=PAGECNT) return(-1);
  if ((y<0)||(y>3)) return(-2);
  if ((w<0)||(w>5)) return(-3);
  for(i=0;i<w;i++) {
    c[i] = LCDd::tarea[p][x+i][y];
  }
  c[i]=(char)NULL;
  i = atoi(c);
  return(i);
}

void LCDd::TextWrite(int p,int x,int y,char a[]) {
  int i,j;
  char sv[6],fmt[5];
  if (p >= PAGECNT) return;
  i = strlen(a);
  if ((i+x)>20) i=20-x;
  if (debugMsgFlag(SO_MSG)) {
    Serial.print("TextWrite x=");
    Serial.print(x);
    Serial.print(",y=");
    Serial.print(y);
    Serial.print(",a=");
    Serial.print(a);
    Serial.print(",i=");
    Serial.println(i);
  }
  for(j=0;j<i;j++) {
    LCDd::setWriteChar(p,x+j,y,a[j]);
  }
  LCDd::setCursor(x,y);
  LCDd::print(a);
  LCDd::setCursor(x,y);
}


void LCDd::IntWrite(int p,int x,int y,int w,bool zp,int a) {
  int i;
  char sv[6],fmt[5];
  if ((w < 1) || (w > 5)) {
    //    w out of range
    return;
  }
  if (p >= PAGECNT) return;
  if (zp) {
    snprintf(fmt,5,"%%0%dd",w);
  } else {
    snprintf(fmt,4,"%%%dd",w);
  }
  snprintf(sv,w+1,fmt,a);

  for(int i=0;i<w;i++) {
    LCDd::setWriteChar(p,x+i,y,sv[i]);
  }
  LCDd::setCursor(x,y);
  LCDd::print(sv);
  LCDd::setCursor(x,y);
}

char LCDd::CharRead(int p,int x,int y) {
  char c;
  if (p >= PAGECNT) return(-1);
  c = LCDd::tarea[p][x][y];
  return c;  
}

int LCDd::getDataInt(int p,int x,int y,int w) {
  int i,v;
  char s[6];
  if ((w>5)||(w<1))  return 0;
  for(i=0;i<w;i++) {
    s[i] = LCDd::tarea[p][x+i][y];
  }
  s[i]=(char)0;
  v = int(s);
}


void LCDd::IPWrite(int p,int x,int y,IPAddress ipa) {
  int w;
  int ip[4];

  x--;
  for (byte tb=0; tb<4; tb++) {
    ip[tb] = (int)((ipa>>(8*tb)) & 0xff);
    if (ip[tb]<10) {
      w = 1;
    } else if (ip[tb]<100) {
      w = 2;
    } else {
      w = 3;
    }
    LCDd::IntWrite(p,x+1,y,w,false,ip[tb]);
    x = x + w + 1;
    if (tb<3) {
      LCDd::CharWrite(p,x,y,'.');
    }
  }
}

void LCDd::IPWriteWithCidr(int p,int x,int y,IPAddress ipa,int cidr) {
  int w;
  int ip[4];

  x--;
  for (byte tb=0; tb<4; tb++) {
    ip[tb] = (int)((ipa>>(8*tb)) & 0xff);
    if (ip[tb]<10) {
      w = 1;
    } else if (ip[tb]<100) {
      w = 2;
    } else {
      w = 3;
    }
    LCDd::IntWrite(p,x+1,y,w,false,ip[tb]);
    x = x + w + 1;
    if (tb<3) {
      LCDd::CharWrite(p,x,y,'.');
    }
  }
  LCDd::CharWrite(p,x,y,'/');
  if (cidr<10) {
    w = 1;
  } else {
    w = 2;
  }
  LCDd::IntWrite(p,x+1,y,w,false,cidr);
}

int mask2cidr(IPAddress nma) {
  int ci,tb;
  ci=0;
  for (tb=0;tb<4;tb++) {
    //    Serial.begin(115200);
    //    Serial.print("mask2cidr=");
    //    Serial.print(nma[tb],DEC);
    //    Serial.print("  ci=");
    //    Serial.println(ci);
    //    Serial.end();
    switch((nma[tb]&0xff)) {
    case 128:
      ci+=1;
      break;
    case 192:
      ci+=2;
      break;
    case 224:
      ci+=3;
      break;
    case 240:
      ci+=4;
      break;
    case 248:
      ci+=5;
      break;
    case 252:
      ci+=6;
      break;
    case 254:
      ci+=7;
      break;
    case 255:
      ci+=8;
      break;
    case 0:
      break;
    default:
      return(-2);
    }
  }
  //  Serial.begin(115200);
  //  Serial.print("return mask2cidr=");
  //  Serial.println(ci,DEC);
  //  Serial.end();
  return(ci);
}


#undef _M304_CPP_
