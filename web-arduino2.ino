#include <EtherCard.h>

#define WEB_IP "192.168.0.200"

int a0Value = 0; 
int a1Value = 0; 
int a2Value = 0; 
int a3Value = 0; 
int a4Value = 0; 
int a5Value = 0; 

byte d7Status = 0; 



static byte myip[] = { 192,168,0,200 };
static byte gwip[] = { 192,168,0,1 };
static byte mymac[] = { 0x34,0x30,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500];    
  
BufferFiller bfill;              

void setup () {

 pinMode(7, OUTPUT);
  
 if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
   Serial.println( "Failed to access Ethernet controller");
 ether.staticSetup(myip);
}

static word homePage() {

 a0Value = analogRead(0);
 a1Value = analogRead(1);
 a2Value = analogRead(2);
 a3Value = analogRead(3);
 a4Value = analogRead(4);
 a5Value = analogRead(5);
 
 d7Status = digitalRead(7);
 
 bfill = ether.tcpOffset();                        
 bfill.emit_p(PSTR(                                
   "HTTP/1.0 200 OK\r\n"
   "Content-Type: text/html\r\n"
   "Pragma: no-cache\r\n"
   "\r\n"
   "<meta http-equiv='refresh' content='1'/>"
   "<title>RBBB server</title>"
   "<h1>analog port value</h1>"
   "A0 $D <br />"
   "A1 $D <br />"
   "A2 $D <br />"
   "A3 $D <br />"
   "A4 $D <br />"
   "A5 $D <br />"

   "<h1>digital port status</h1>"
   "D7 $D   "
   "<a href=\"http://"
   WEB_IP
   "/?D7=on\" rel=\"nofollow\"> ON </a>"
   " | "
   "<a href=\"http://"
   WEB_IP
   "/?D7=off\" rel=\"nofollow\"> OFF </a></br>"
   
   ),
   a0Value, a1Value, a2Value, a3Value, a4Value, a5Value, d7Status);               
return bfill.position();                                          
}

void loop () {

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
 
 if (pos) {
   if (strstr((char *)Ethernet::buffer + pos, "GET /?D7=on") != 0) {
    actionD7(true);
   }
   else if(strstr((char *)Ethernet::buffer + pos, "GET /?D7=off") != 0) {
    actionD7(false);
   }
   ether.httpServerReply(homePage());
 }
 
}

void actionD7(boolean pinStatus) {
  digitalWrite(7, pinStatus);
}








