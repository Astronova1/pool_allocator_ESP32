#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"

#define ssid "Wokwi-GUEST"
#define password ""
#define DHTPIN 2
#define DHTTYPE 22
DHT dht(DHTPIN,DHTTYPE);

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();
  delay(200);
  WiFi.begin(ssid,password,11);
  while (WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");

  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}