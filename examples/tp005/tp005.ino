#include <M304.h>
//EthernetClient client;

char *pgname = "M304jp TP005 Ver1.40";
char *cbf = "                    ";
//byte mac[6] ;
byte ip[4] = { 192,168,0,177 };

//extern stM304 st_m;
void setup(void) {
  char lbf[80];
  int  i;
  Serial.begin(115200);
  while(!Serial) {
    ;
  }
  m304Init();
  Serial.println("In setup()");
  for(i=0;i<6;i++) {
    Serial.print("i=");
    Serial.print(i);
    Serial.print(" 0x");
    Serial.println(st_m.mac[i],HEX);
  }
  
  sprintf(lbf,"MAC:%02X%02X.%02X%02X.%02X%02X",st_m.mac[0],st_m.mac[1],st_m.mac[2],st_m.mac[3],st_m.mac[4],st_m.mac[5]);
  lcd.setCursor(0,1);
  lcd.print("Connect Console USB");
  lcd.setCursor(0,2);
  lcd.print("Baud:115200");
  lcd.setCursor(0,3);
  Ethernet.begin(st_m.mac);
  if (Ethernet.begin(st_m.mac)==0) {
    Serial.println("Failed to configure Ethernet using DHCP.");
    lcd.print("FAIL DHCP PLS RESET");
  } else {
    lcd.setCursor(0,1);
    lcd.print(cbf);
    lcd.setCursor(0,1);
    lcd.print(lbf);
    //    Serial.println(lbf);
    //    Serial.print("IP:");
    //    Serial.println(Ethernet.localIP());
    //    Serial.print("GW:");
    //    Serial.println(Ethernet.gatewayIP());
    //    Serial.print("DNS:");
    //    Serial.println(Ethernet.dnsServerIP());
    lcd.setCursor(0,2);
    lcd.print("IP:");
    lcd.print(Ethernet.localIP());
    lcd.setCursor(0,3);
    lcd.print("GW:");
    lcd.print(Ethernet.gatewayIP());
    st_m.gw = Ethernet.gatewayIP();
    st_m.ip = Ethernet.localIP();
    st_m.dns = Ethernet.dnsServerIP();
    debug_print();
    for (byte tb=0; tb<4; tb++) {
      ip[tb] = Ethernet.localIP()[tb];
    }
  }
}

void loop(void) {
  
}
