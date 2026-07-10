#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>


#define ssid "Wokwi-GUEST"
#define password ""
#define DHTPIN 15
#define DHTTYPE 22
DHT dht(DHTPIN,DHTTYPE);

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(200);
  dht.begin();
  WiFi.begin(ssid,password,11);
  while (WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");

  int status;
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("host.wokwi.internal","8088", &hints, &res))

  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  float Temp = dht.readTemperature();
  float Humidity = dht.readHumidity();

  if (isnan(Temp) || isnan(Humidity) ){
    Serial.println("Failed to read temperature and humidity");
  }
    Serial.println("");
    Serial.print("Temperature is: ");
    Serial.print(Temp);
    Serial.print("  | Humidity is: ");
    Serial.print(Humidity);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}