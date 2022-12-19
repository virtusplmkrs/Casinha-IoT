#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

const char* ssid = "Planet Makers";
const char* password = "17227285";

ESP8266WebServer server(80);

uint8_t Relay1 = D8;
uint8_t Relay2 = D5;
uint8_t Relay3 = D2;
uint8_t Relay4 = D3;

int load1, load2, load3, load4;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  
  EEPROM.begin(512);
  
  load1 = EEPROM.read(1);
  load2 = EEPROM.read(2);
  load3 = EEPROM.read(3);
  load4 = EEPROM.read(4);

  Serial.print(load1); Serial.print(" ");
  Serial.print(load2); Serial.print(" ");
  Serial.print(load3); Serial.print(" ");
  Serial.println(load4);
  
  digitalWrite(Relay1, load1);
  digitalWrite(Relay2, load2);
  digitalWrite(Relay3, load3);
  digitalWrite(Relay4, load4);
  
  WiFi.softAP(ssid, password);
  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: \n");
  Serial.println(apip);
  
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/L1on",  handle_L1on);
  server.on("/L1off", handle_L1off);
  server.on("/L2on",  handle_L2on);
  server.on("/L2off", handle_L2off);
  server.on("/L3on",  handle_L3on);
  server.on("/L3off", handle_L3off);
  server.on("/L4on",  handle_L4on);
  server.on("/L4off", handle_L4off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.print("HTTP server started");
}

void loop() {
  server.handleClient();

  digitalWrite(Relay1, load1);
  digitalWrite(Relay2, load2);
  digitalWrite(Relay3, load3);
  digitalWrite(Relay4, load4);
}

void handle_OnConnect(){
  server.send(200, "text/html", SendHTML(load1, load2, load3, load4));
}

void handle_L1on()  {load1=0;data();}
void handle_L1off() {load1=1;data();}

void handle_L2on()  {load2=0;data();}
void handle_L2off() {load2=1;data();}

void handle_L3on()  {load3=0;data();}
void handle_L3off() {load3=1;data();}

void handle_L4on()  {load4=0;data();}
void handle_L4off() {load4=1;data();}

void data(){
EEPROM.write(1, load1);
EEPROM.write(2, load2);
EEPROM.write(3, load3);
EEPROM.write(4, load4);
EEPROM.commit();

server.send(200, "text/html", SendHTML(load1, load2, load3, load4));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t l1stat,uint8_t l2stat,uint8_t l3stat,uint8_t l4stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>P. Controle</title>\n";
  ptr +="<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: ;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-botton: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h2>Automacao e Controle</h2>\n";
   if(l1stat==0)
   {ptr +="<p>Ambiente1 Status: OFF<a class=\"button button-on\" href=\"/L1off\">ON</a></p>\n";}
   else
   {ptr +="<p>Ambiente1 Status: ON<a class=\"button button-off\" href=\"/L1on\">OFF</a></p>\n";}

   if(l2stat==0)
   {ptr +="<p>Ambiente2 Status: OFF<a class=\"button button-on\" href=\"/L2off\">ON</a></p>\n";}
   else
   {ptr +="<p>Ambiente2 Status: ON<a class=\"button button-off\" href=\"/L2on\">OFF</a></p>\n";}

   if(l3stat==0)
   {ptr +="<p>Ambiente3 Status: OFF<a class=\"button button-on\" href=\"/L3off\">ON</a></p>\n";}
   else
   {ptr +="<p>Ambiente3 Status: ON<a class=\"button button-off\" href=\"/L3on\">OFF</a></p>\n";}

   if(l4stat==0)
   {ptr +="<p>Ambiente4 Status: OFF<a class=\"button button-on\" href=\"/L4off\">ON</a></p>\n";}
   else
   {ptr +="<p>Ambiente4 Status: ON<a class=\"button button-off\" href=\"/L4on\">OFF</a></p>\n";}
    
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
