#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "esp_log.h"

#define ssid "Wokwi-GUEST"
#define password ""
#define PORT "8088"
#define DHTPIN 15
#define DHTTYPE 22
DHT dht(DHTPIN,DHTTYPE);

static const char *TAG = "Wokwi_App";

  int socket_fd;


// put function declarations here:
int myFunction(int, int);

void *getaddr_info(struct sockaddr *sa){
  if (sa->sa_family == AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
    if (sa->sa_family == AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr);
    }
}

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
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("host.wokwi.internal",PORT, &hints, &res)) != 0){
      Serial.println("DNS Resolution Failed");
      return;
  }


  if ((socket_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol)) == -1 ){
    Serial.println("Socket Creation Error");
    return;
  }
  char s [INET6_ADDRSTRLEN];

  inet_ntop(res->ai_family,getaddr_info((sockaddr *)res->ai_addr),s , sizeof s);

Serial.println("Attempting to connect to server directly...");
if (connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1) {
    Serial.printf("Cannot Connect! Error code: %d\n", errno);
        while(true) { delay(1000); } // Safely stalls the ESP32 here so it never enters loop()
}
Serial.println("Successfully Connected to local server!");
freeaddrinfo(res);
/*

  if (connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1){
        Serial.println("Cannot Connect");
        close(socket_fd);
        return;
  }
*/
// ... inside setup() after WiFi is connected ...
/*
socket_fd = socket(AF_INET, SOCK_STREAM, 0);
if (socket_fd == -1) {
    Serial.println("Socket Creation Error!");
    return;
}

struct sockaddr_in server_addr;
memset(&server_addr, 0, sizeof(server_addr));
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8088); // Your server port

// Use 10.13.37.1 based on your active Wokwi network interface string
if (inet_pton(AF_INET, "192.168.100.105", &server_addr.sin_addr) <= 0) {
    Serial.println("Invalid address / Address not supported");
}

*/

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

  char payload [16];
  dtostrf(Temp, 6, 2, payload);

  int len,byte_sen;
  len = strlen(payload);

  if ((byte_sen = send(socket_fd, payload, len, 0)) == 0){
    Serial.printf(TAG,"Failed To send ");
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}