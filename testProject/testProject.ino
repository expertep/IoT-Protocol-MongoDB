
#include "ESP8266WiFi.h"
#include "DHT.h"

const char *ssid = "NSTDA-Project";
const char *passw = "Password WIFI";

#define host "localhost"
#define port 4000
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

String response;
float value;
String _str, _res, _t, _h;
float t, h;


void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, passw);

  Serial.print("WiFi connecting..");

  while ((WiFi.status() != WL_CONNECTED)) {
    delay(200);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected !");
    sayHi();
  } else {
    Serial.println("Disconnected !");
  }
}

void loop() {
  t = dht.readTemperature();
  h = dht.readHumidity();
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println("Temperature is " + String(t) + " celcuis");
  Serial.println("Humidity is " + String(h) + " %RH");
  Serial.println("----------------------------------------");
  WriteDHT(t, h);
  delay(5000);

}

String sayHi() {
  WiFiClient client;
  if (client.connect(host, port)) {
    _str = "GET /";
    _str += " HTTP/1.1\r\n";
    _str += "Host: ";
    _str += host;
    _str += ":";
    _str += port;
    _str += "\r\n";
    _str += "Connection: keep-alive\r\n\r\n";

    client.print(_str);

    delay(3000);

    while (client.available()) {
      _res = client.readStringUntil('\r');
    }

    return _res;

  } else {
    //..
  }
}

String WriteDHT (float t, float h) {

  _t = String(t);
  _h = String(h);

  WiFiClient client;
  if (client.connect(host, port)) {

    _str = "GET /writedht/";
    _str += _t;
    _str += "/";
    _str += _h;
    _str += " HTTP/1.1\r\n";
    _str += "Host: ";
    _str += host;
    _str += ":";
    _str += port;
    _str += "\r\n";
    _str += "Connection: keep-alive\r\n\r\n";

    client.print(_str);

    delay(3000);
    Serial.println("Add Data to MongoDB Success");
    while (client.available()) {
      _res = client.readStringUntil('\r');
    }

    return _res;
  } else {
    //Nothing..
  }
}
