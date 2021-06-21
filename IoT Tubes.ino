#include <DHT.h>
#include <ESP8266WiFi.h>           //libraries
#define DHTTYPE DHT11

String apiKey = "96SLPLVVWQTWF0RK";          //thingspeak credentials
const char* server = "api.thingspeak.com";

#define SSID "mataharikita"            //wifi credentials
#define PASS "aninafif"

const int DHTPin = D1;
const int rain = D0;           //pin assignment
const int smoke = A0;

DHT dht(DHTPin, DHTTYPE);          //dht set
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(rain, INPUT);
  pinMode(smoke, INPUT);          //pin setup

  //------------------------
  
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Trying to connect...");      //wifi connect
    delay(5000);
  }
  
  //------------------------
  
  Serial.print("Connected to ");         //tersambung, lanjut ke loop
  Serial.println(SSID);

  //------------------------
}

/////////////////////////////////////////////

void loop() {
  float Humidity = dht.readHumidity();
  float Temp = dht.readTemperature();       //assign start
  float smoke = analogRead(smoke);
  float rainD = analogRead(rain);
  //-------------------------------//

    if (isnan(Humidity) || isnan(Temp))
  {
    Serial.println("Failed to read from DHT sensor!");   //dht detect
    return;
  }

  //-------------------------------//

  Serial.println();

  if (digitalRead(rain) == HIGH) {
    Serial.println("Digital Val: Not Raining");      //serial Rain sensor
  } else {
    Serial.println("Digital Val: Raining");
  }
  
  //-------------------------------//

  Serial.print("Smoke Analogue Val: ");
  Serial.println(smoke);
  if (smoke > 800) {
    Serial.println("Smoke Detected");        //serial smoke detector
  } else {
    Serial.println("Smoke Not Detected");
  }

  //-------------------------------//
  
	Serial.print("Temperature: ");
    Serial.print(Temp);
    Serial.println("°C");
    Serial.print("Humidity: ");                           //serial temp & humid
    Serial.print(Humidity);
    Serial.println("%");
    Serial.println("Send to Thingspeak.");

  //-------------------------------//

  if (client.connect(server, 80)) 
  {

    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(Temp);
    postStr += "&field2=";                      //send things to thingspeak
    postStr += String(Humidity);
    postStr += "&field3=";
    postStr += String(smoke);
    postStr += "&field4=";
    postStr += String(rainD);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
  }
  
  client.stop();
  Serial.println();
  Serial.println("Waiting...");
  
  delay(15000);
}
