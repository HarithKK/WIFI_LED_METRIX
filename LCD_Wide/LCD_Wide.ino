//#include <SoftwareSerial.h>
//#include <ArduinoJson.h>
#include <SPI.h>
#include <DMD2.h>
#include <SystemFont5x7.h>
#include <Arial_Black_16.h>

#define W 4
#define H 2

SoftDMD dmd1(W,H);  // DMD controls the entire display
//oftDMD dmd2(W,H);

#define BRIGHT 150

int width_changer=18;


//SoftwareSerial wifi(2,3);
String message;

unsigned long last=0;
void setup() {
  // put your setup code here, to run once:
  delay(3000);
  Serial.begin(2400);

  dmd1.setBrightness(BRIGHT);
  dmd1.begin();
  
  dmd1.fillScreen(true);
  delay(100);
  dmd1.fillScreen(false);
  delay(100);

  dmd1.selectFont(Arial_Black_16);
  dmd1.drawString(5,2,"KW Creations ");
  
  message = ReadWifi(); //F("{\"Record_time\":\"2018-04-24 12:19:03\",\"Humidity\":\"73\",\"Ext_temp\":\"29\",\"Int_temp\":\"29\",\"Intensity\":\"33\",\"Win_dir\":\"28.71\",\"Win_speed\":\"0\",\"Rain_gauge\":\"0\",\"Pressure\":\"100861\",\"Soil_Moisture\":\"0\"}");// ReadWifi();
  


  dmd1.fillScreen(true);
  delay(100);
  dmd1.fillScreen(false);
  delay(100);

  
}
int count =1;

void loop() {

  if(count==5){
    String s =ReadWifiGraph('T');
    drawGraph("Temp",getValue(10),s);
    delay(6000);
    dmd1.fillScreen(false);

    s =ReadWifiGraph('H');
    drawGraph("Humd",getValue(6),s);
    delay(6000);
    dmd1.fillScreen(false);

    s =ReadWifiGraph('G');
    drawGraph("Rain",getValue(6),s);
    delay(6000);
    dmd1.fillScreen(false);

    s =ReadWifiGraph('W');
    drawGraph("W.Spd",getValue(6),s);
    delay(6000);
    dmd1.fillScreen(false);

    s =ReadWifiGraph('I');
    drawGraph("Inte",getValue(6),s);
    delay(6000);
    dmd1.fillScreen(false);

    s =ReadWifiGraph('P');
    drawGraph("Pres",getValue(6),s);
    delay(6000);
    dmd1.fillScreen(false);

    s =ReadWifiGraph('S');
    drawGraph("So.M",getValue(6),s);
    delay(6000);
    dmd1.fillScreen(false);
  }
  
  if(count==10){
    message = ReadWifi();
    count=0;
  }
  drawTemperature(getValue(10));
  drawHumidity(getValue(6));
  
  drawWDS(getValue(22),getValue(26),getValue(30),15);
  
  delay(2000);
  for(int i=0;i<19;i++){
    dmd1.scrollY(-1);  
  }
  dmd1.fillScreen(false);

  drawTemperature(getValue(10));
  drawHumidity(getValue(6));
    
  drawRGP(getValue(34),getValue(38),getValue(18),15);
  delay(2000);
  for(int i=0;i<19;i++){
    dmd1.scrollY(-1);  
  }
  dmd1.fillScreen(false);

  drawTemperature(getValue(10));
  drawHumidity(getValue(6));
  
  drawRNL(getValue(30),getValue(18),15);
  
  delay(2000);
  for(int i=0;i<19;i++){
    dmd1.scrollY(-1);  
  }
  dmd1.fillScreen(false);
  count++;
}
//
String getVane(String s){
    return s;
}

String getValue(int j){
  uint8_t ok =0,m=0;
  String sp=String("");
  for(int i=0;i<message.length();i++){
    char c = message.charAt(i);
    //Serial.print(c);
    if(c=='"'){
      if(ok==0){
         if(j==m)
          ok =1;
         else
          m++;
      }else{
         break;
      } 
    }else{
      if(ok==1)
        sp.concat(c);  
    }
      
  }
  return sp;
}

String ReadWifi(){
  Serial.println("R");
  String rec=String("");
  unsigned long st = millis();
  uint8_t et =0;
  while(((millis() - st) < 10000UL) && et == 0) {
    if(Serial.available()){
      char c= (char)Serial.read();
      rec += c;
      //Serial.write(c);
      if(c == '}'){
        et = 1;
      }
    }    
  }
  return rec;
}

String ReadWifiGraph(char c){
  
  Serial.println(c);
  String rec=String("");
  unsigned long st = millis();
  uint8_t et =0;
  while(((millis() - st) < 10000UL) && et == 0) {
    if(Serial.available()){
      char c= (char)Serial.read();
      rec += c;
      //Serial.write(c);
      if(c == '+'){
        et = 1;
      }
    }    
  }

  return rec;
  
}



void drawWDS(String WD,String WS,String RG,int h){
    dmd1.selectFont(SystemFont5x7);
    String s = F("W. Vane : ");
    s.concat(WD);
    dmd1.drawString(10,0+h,s);

    s = F("W. Speed : ");
    s.concat(WS);
    dmd1.drawString(10,9+h,s);

//    s = F("Rain Fall : ");
//    s.concat(RG);
//    dmd1.drawString(0,18+h,s);
}

void drawRGP(String P,String SM,String L,int h){
    dmd1.selectFont(SystemFont5x7);
    String s = F("Pressure : ");
    s.concat(P);
    dmd1.drawString(10,h,s);

    s = F("Soil M. : ");
    s.concat(SM);
    dmd1.drawString(10,9+h,s);

//    s = F("Solar In : ");
//    s.concat(L);
//    dmd1.drawString(0,18+h,s);
}

void drawRNL(String RG,String L,int h){
    dmd1.selectFont(SystemFont5x7);
    
    String s = F("Rain Fall : ");
    s.concat(RG);
    dmd1.drawString(10,h,s);

    s = F("Solar In : ");
    s.concat(L);
    dmd1.drawString(10,9+h,s);
}

void drawTemperature(String temp){
  dmd1.selectFont(Arial_Black_16);
  dmd1.drawString(1+ width_changer,0,temp);
  dmd1.drawCircle(21 + width_changer,1,1);
  dmd1.drawString(24 + width_changer,0,"C");

}

void drawHumidity(String h){
  dmd1.selectFont(Arial_Black_16);
  h.concat('%');
  dmd1.drawString(50+width_changer,0,h);
  
}

void drawGraph(String unit,String val,String values){

  String s = values.substring(0,values.length()-1);
  String p="";
  int j=0;
  int vals[10];
  for(int i=0;i<s.length();i++){
      if(s[i] == ','){
          vals[j++] = p.toInt();
          p="";
          continue;
      }
      p += s[i];
  }
  
  
  dmd1.selectFont(Arial_Black_16);
  dmd1.drawString(2,1,unit);
  dmd1.drawString(2,17,val);
  dmd1.selectFont(SystemFont5x7);
  dmd1.drawString(25,20,"(10h)");
  
  dmd1.drawLine(70,2,70,28);
  dmd1.drawLine(70,28,130,28);

  int x1= 70,y1=28;

  for(int i=0;i<10;i++){
    int y2 = vals[i]/4;
    y2 = 28-y2;
    Serial.println(x1);
    dmd1.drawLine(x1,y1,x1+5,y2);
    x1 +=5;
    y1=y2;
  }
}


