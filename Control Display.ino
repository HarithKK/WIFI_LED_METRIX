#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <DMD2.h>
#include <SystemFont5x7.h>
#include <Arial_Black_16.h>

#define W 3
#define H 1

SoftDMD dmd1(W,H);  // DMD controls the entire display
SoftDMD dmd2(W,H);

#define BRIGHT 150

int width_changer=8;


SoftwareSerial wifi(2,3);
String message;

unsigned long last=0;
void setup() {
  // put your setup code here, to run once:
  wifi.begin(2400);
  delay(3000);
  Serial.begin(9600);

  message = ReadWifi();
  
  dmd1.setBrightness(BRIGHT);
  dmd1.selectFont(Arial_Black_16);
  dmd1.begin();

  dmd2.setBrightness(BRIGHT);
  dmd2.selectFont(SystemFont5x7);
  dmd2.begin();
  dmd2.drawString(0,0,F("KW Setup"));

  dmd2.fillScreen(true);
  delay(10);
  dmd2.fillScreen(false);
  delay(10);
  

}
int count =0;

void loop() {

  if(count > 30){
    asm volatile ("  jmp 0");
    
  }
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);
  drawTemperature(root[F("Ext_temp")].as<String>());
  drawHumidity(root[F("Humidity")].as<String>());

  delay(2000);
  for(int i=0;i<16;i++)
    dmd1.scrollY(-1);
  drawWDS(getVane(root[F("Win_dir")].as<String>()),root[F("Win_speed")].as<String>(),0);
  
  delay(2000);
  for(int i=0;i<16;i++)
    dmd2.scrollY(-1);
  
  drawRGP(root[F("Rain_gauge")].as<String>(),root[F("Pressure")].as<String>(),0);
  delay(2000);

  for(int i=0;i<16;i++)
    dmd2.scrollY(-1);
    
  drawSML(root[F("Soil_Moisture")].as<String>(),root[F("Intensity")].as<String>(),0);
  delay(2000);
  
  dmd2.fillScreen(false);
  count++;
}

String getVane(String s){
    return s;
}


String ReadWifi(){
  Serial.println("RE");
  wifi.println("R");
  String rec=String("");
  unsigned long st = millis();
  uint8_t et =0;
  while(((millis() - st) < 10000UL) && et == 0) {
    if(wifi.available()){
      char c= (char)wifi.read();
      rec += c;
      Serial.write(c);
      if(c == '}'){
        et = 1;
      }
    }    
  }
  wifi.flush();
  return rec;
}


void drawWDS(String WD,String WS,int h){
    String s = F("W. Vane : ");
    s.concat(WD);
    dmd2.drawString(0,0+h,s);

    s = F("W. Speed : ");
    s.concat(WS);
    dmd2.drawString(0,9+h,s);
}

void drawRGP(String RG,String P,int h){
    String s = F("Rain Fall : ");
    s.concat(RG);
    dmd2.drawString(0,0+h,s);

    s = F("Pressure : ");
    s.concat(P);
    dmd2.drawString(0,9+h,s);
}

void drawSML(String SM,String L,int h){
    String s = F("Soil M. : ");
    s.concat(SM);
    dmd2.drawString(0,0+h,s);

    s = F("Solar In : ");
    s.concat(L);
    dmd2.drawString(0,9+h,s);
}

void drawTemperature(String temp){
  dmd1.drawString(1+ width_changer,0,temp);
  dmd1.drawCircle(21 + width_changer,1,1);
  dmd1.drawString(24 + width_changer,0,"C");

}

void drawHumidity(String h){
  h.concat('%');
  dmd1.drawString(50+width_changer,0,h);
  
}

