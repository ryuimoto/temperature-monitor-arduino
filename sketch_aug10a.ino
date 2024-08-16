#include <DHT.h>
#include "WiFiS3.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define LEDPIN 13

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "HG8045-309A-bg";
char pass[] = "8narydcd";
char server[] = "192.168.1.80";
int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {
  Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);
  dht.begin();

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
}

void loop() {
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.println(temperature);

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

  if (temperature >= 35) {
    digitalWrite(LEDPIN, HIGH);
    delay(1000);
    digitalWrite(LEDPIN, LOW);
  }

  if (client.connect(server, 1000)) {
  // データの作成
  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);
  client.println("POST /temperature HTTP/1.1");
  client.println("Host: 192.168.1.80");
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);
  client.stop();
}
}
