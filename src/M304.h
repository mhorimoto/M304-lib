#ifndef _M304_H_
#define _M304_H_
#define _M304_H_V  132

#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
#include <EEPROM.h>
#include <AT24CX.h>
#include <Wire.h>
#include <DS1307RTC.h>

/*** COMMON ***/

#define TRUE      1
#define FALSE     0
#define ERROR     -1
#define NO        0
#define UNMATCH   0
#define IGNORE    1
#define CHANGE_MAKE  2
#define CHANGE_BREAK 3


/*** CMODE ***/

#define RUN       0
#define CMND      1

#define NETCMND  10
#define RTCCMND  11
#define SCHCMND  12
#define EEPROMCMND 13

/*** CMENU ***/

#define NETCONFIG 0
#define RTCCONFIG 1
#define SCHCONFIG 2
#define EEPROMOPE 3

/*** Define for LCD ***/
#define RS        37
#define RW        38
#define ENA       39
#define DB0       40
#define DB1       41
#define DB2       42
#define DB3       43
#define DB4       44
#define DB5       45
#define DB6       46
#define DB7       47
#define PAGECNT    2

/*** Define for Arrow Key ***/
#define SW_SAFE    3
#define SW_RLY    31
#define SW_ENTER  32
#define SW_UP     33
#define SW_DOWN   34
#define SW_LEFT   35
#define SW_RIGHT  36
#define SELECT_VR A15

/*** Define Relay Ports ***/
#define  RLY1    22
#define  RLY2    23
#define  RLY3    24
#define  RLY4    25
#define  RLY5    26
#define  RLY6    27
#define  RLY7    28
#define  RLY8    29

/*** Ethernet2 Ports ***/
#define NICSS    53

/*** I2C Address ***/
#define AT24LC254_ADDR   0x50
#define AT24LC254_INDEX  0

/*** UECS TEXT ***/
#define CCMFMT "<?xml version=\"1.0\"?><UECS ver=\"1.00-E10\"><DATA type=\"%s\" room=\"%d\" region=\"%d\" order=\"%d\" priority=\"%d\">%s</DATA><IP>%s</IP></UECS>"

/*** EEPROM LOWCORE ASSIGN ***/
#define LC_UECS_ID        0x00
#define LC_MAC            0x06
#define FIX_DHCP_FLAG     0x0c
#define FIXED_IPADDRESS   0x10
#define FIXED_NETMASK     0x14
#define FIXED_DEFGW       0x18
#define FIXED_DNS         0x1c
#define VENDER_NAME       0x40
#define NODE_NAME         0x50
#define LC_DBGMSG         0x60  /* bit pos 0x80: Serial out, 0x40: LCD out */
#define   SO_MSG    0x80
#define   LCD_MSG   0x40
#define LC_START          0x80
#define LC_SCH_START 0x1000
#define   STHR       0x0
#define   STMN       0x1
#define   EDHR       0x2
#define   EDMN       0x3
#define   INMN       0x4
#define   DUMN       0x5
#define   RLY_L      0xe
#define   RLY_H      0xf
#define LC_END      0x7fff

typedef struct stM304 {
  byte mac[6];
  boolean dhcpflag=true;
  byte set_ip[4];
  IPAddress ip;
  IPAddress gw;
  IPAddress dns;
  IPAddress subnet;
  int cidr;
};

/*** EEPROM 0x0100..0x02ff ***/
typedef struct uecsM304 {
  bool enable;
  byte room;
  byte region;
  int  order;
  byte priority;
  char ccm_type[20];
};  //  32bytes/1unit

/*  Cross Key Switch */

#define K_ENT    1
#define K_UP     2
#define K_DOWN   4
#define K_LEFT   8
#define K_RIGHT 16

#define K_DIGIT  32
#define K_XDIGIT 33
#define K_PRINT  34
#define K_ALPHA  35
#define K_ALNUM  36

#define PUSH_SHORT 1000
#define PUSH_LONG  300000

#ifndef _KYBDMEM_
#define _KYBDMEM_
struct KYBDMEM {
  uint8_t   kpos;
  uint16_t  selpos;
  bool      longf;
};
#endif
#ifndef _M304_COMMON_KYBD
#endif

#ifndef _M304_COMMON_DISPLAY
#define _M304_COMMON_DISPLAY 1

class LCDd : public LiquidCrystal {
 private:
  char tarea[PAGECNT][20][4];
 public:
 LCDd():LiquidCrystal(RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7) {
  LiquidCrystal:init(0, RS,RW,ENA,DB0,DB1,DB2,DB3,DB4,DB5,DB6,DB7);
};
 LCDd(uint8_t rs, uint8_t enable,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
      uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):
  LiquidCrystal(rs,enable,d0,d1,d2,d3,d4,d5,d6,d7){
  LiquidCrystal:init(0, rs,255,enable,d0,d1,d2,d3,d4,d5,d6,d7);
};
  LCDd(uint8_t rs, uint8_t rw, uint8_t enable,
       uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
       uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7):
  LiquidCrystal(rs,rw,enable,d0,d1,d2,d3,d4,d5,d6,d7){};
  LCDd(uint8_t rs, uint8_t rw, uint8_t enable,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
  LCDd(uint8_t rs, uint8_t enable,
      uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);
  void initWriteArea(int);
  int  setWriteChar(int,int,int,char),getDataInt(int,int,int,int),setLine(int,int,char *);
  int  IntRead(int,int,int,int);
  char CharRead(int,int,int);
  void LineWrite(int,int),PageWrite(int),CharWrite(int,int,int,char),IntWrite(int,int,int,int,bool,int);
  void TextWrite(int,int,int,char[]),IPWrite(int,int,int,IPAddress),IPWriteWithCidr(int,int,int,IPAddress,int);
};

#endif

#ifndef _M304_CPP_
  extern void m304Init(void);
  extern void debug_print(void);
#endif
  extern stM304 st_m;
  extern LiquidCrystal lcd;
  extern AT24C256      atmem;
  extern EthernetClient client;
#endif
