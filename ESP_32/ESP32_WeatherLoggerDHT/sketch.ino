#include <WiFi.h>
  const char* ssid = "Wokwi-GUEST";
  const char* password = "";
  
void setup(){
  Serial.begin(96000);
  WiFi.begin(ssid,password);
  Serial.print("Connecting to wifi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(1000);
  }
    Serial.println(WiFi.localIP());
}

void loop(){
  delay(1000);
}