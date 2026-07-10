#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include "esp_log.h"

#define ssid "Wokwi-GUEST"
#define password ""
#define DHTPIN 15
#define DHTTYPE 22
DHT dht(DHTPIN,DHTTYPE);

static const char *TAG = "Wokwi_App";

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
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo("host.wokwi.internal","8088", &hints, &res)) != 0){
      ESP_LOGI(TAG,"DNS Resolution Failed");
      return;
  }

  int socket_fd;

  if ((socket_fd = socket(res->ai_family,res->ai_socktype,res->ai_protocol) == -1 )){
    ESP_LOGI(TAG,"Socket Creation Error");
    return;
  }
  char s [INET6_ADDRSTRLEN];

  inet_ntop(res->ai_family,getaddr_info((sockaddr *)res->ai_addr),s , sizeof s);
  ESP_LOGI(TAG,"Attepmting to connect to "+ s);

  if (connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1){
        ESP_LOGI(TAG,"Cannot Connect");
        closesocket(socket_fd);
        return;
  }
    ESP_LOGI(TAG,"Connected to "+ s);


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