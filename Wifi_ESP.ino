#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

char* ssid = "SEC";
char* password = "sec12Sg920342248";
char* url ="http://www.slpiot.org/api/getLatestRecordForLCD/5bf82c59-7ec0-4f";
//[{"Record_time":"2018-04-24 12:19:03","Humidity":"73","Ext_temp":"29","Int_temp":"29","Intensity":"33","Win_dir":"28.71","Win_speed":"0","Rain_gauge":"0","Pressure":"100861","Soil_Moisture":"0"}]

ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial


void setup() {
 Serial.begin(2400);
 WiFi.mode(WIFI_STA);

 WiFiMulti.addAP(ssid, password);
}

void loop() {

  if(Serial.available()){
    char c = Serial.read();
    if(c=='R'){
      ReadData();
    }  
    Serial.flush();
  }
  
}

void ReadData(){
  
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        http.begin(url); //HTTP
        int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                String payload = getArray( http.getString());
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.println("ERR");
        }
        http.end();
    }  
}

String getArray(String h){
  String y="";
  uint8_t ch =0;
  for(int i=0;i<h.length();i++){
      char f = h.charAt(i);
      if(f=='{')
        ch=1;
      if(ch==1){
        y += f;  
      }
      if(f=='}')
        ch=0;
      
  }
  return y;
    
}

