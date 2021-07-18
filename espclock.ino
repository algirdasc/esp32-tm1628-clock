/*
Credits goes to: https://github.com/BlockThor/TM1628

1. VCC
2. GND
3. Strobe
4. clock
5. Data in/out
*/

#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>
#include <ArduinoOTA.h>
#include <TM1628.h>

#define WIFI_SSID         "router"
#define WIFI_PASS         "letmein"
#define WIFI_HOST         "esp32clock"

#define MQTT_HOST         "router.lan"
#define MQTT_PORT         1883
#define MQTT_USER         "esp"
#define MQTT_PASS         "esp123"

#define MQTT_TEMPERATURE  "heating/temperatures/bedroomr"
#define MQTT_HUMIDITY     "heating/humidities/bedroom"

#define NTP_SERVER        "router.lan"
#define NTP_OFFSET        3600 * 3
#define NTP_DST_OFFSET    3600 * 2
#define TZ                "EET-2EEST,M3.5.0/3,M10.5.0/4" // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

#define DIO     19
#define CLK     18
#define STB     5

int intensity = 7;
int temperature = 0;
int humidity = 0;
time_t now;
struct tm timeinfo;
TM1628 display(DIO, CLK, STB);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);


void blinkSeconds()
{
  display.setLEDon(LED_SECONDS1);
  display.setLEDon(LED_SECONDS2);
  delay(500);
  display.setLEDoff(LED_SECONDS1);
  display.setLEDoff(LED_SECONDS2);
  delay(500);
}

void blinkWiFi()
{
  display.setLEDon(LED_WIFI);
  delay(500);
  display.setLEDoff(LED_WIFI);
  delay(500);
}

void checkIntensity()
{
  int hour = getTime("%H");
  int calculated_intensity = intensity;
  
  if (hour > 8 && hour < 22) {
    calculated_intensity = 7;
  } else {
    calculated_intensity = 1;
  }

  if (calculated_intensity != intensity) {
    intensity = calculated_intensity;
    display.begin(ON, intensity); 
  }
}

void showTime(int hour, int minute)
{
  display.setTime(hour, minute);
}

void showTemperature(int temp)
{
  Serial.println(temp);
  
  if (!temp) {
    display.setChar(4, '-');
    display.setChar(5, '-');
  } else {
    if (temp >= 10) {
      display.setSeg(4, temp / 10);      
    } else {
      display.setChar(4, ' ');      
    }
    display.setSeg(5, temp % 10);
  }

  display.setChar(6, 'C');  
}

void showHumidity(int humidity)
{  
  if (!humidity) {
    display.setChar(7, '-');   
    display.setLastSegment(11); 
  } else {
    if (humidity >= 10) {
      display.setSeg(7, humidity / 10);
    } else {
      display.setChar(7, ' ');
    }
    display.setLastSegment(humidity % 10);
  }
}

int getTime(char* format)
{
  char _time[5];
  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(_time, 80, format, &timeinfo);
  return String(_time).toInt();
}

void mqttCallback(char* topic, byte* payload, unsigned int length)
{     
  payload[length] = '\0';
  String p = String((char*) payload);

  Serial.println(topic);

  if (strcmp(topic, MQTT_TEMPERATURE) == 0) {
    temperature = p.toInt();
  } else if (strcmp(topic, MQTT_HUMIDITY) == 0) {
    humidity = p.toInt();
  }
}

void mqttReconnect()
{
  display.setLEDon(LED_FARENHEIT);
  if (mqttClient.connect(WIFI_HOST, MQTT_USER, MQTT_PASS)) {      
      mqttClient.subscribe(MQTT_TEMPERATURE);
      mqttClient.subscribe(MQTT_HUMIDITY);
  }
  display.setLEDoff(LED_FARENHEIT);
}

void setup()
{
  Serial.begin(115200);
  configTime(NTP_OFFSET, NTP_DST_OFFSET, NTP_SERVER);
  setenv("TZ", TZ, 1);
  tzset();    
  
  display.begin(ON, intensity);

  WiFi.setHostname(WIFI_HOST);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    blinkWiFi();
  }
  
  ArduinoOTA.onStart([]() {
    blinkWiFi();
  });
    
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {  
    showHumidity(progress);
  });

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);
  mqttReconnect();
}

void loop() { 
  ArduinoOTA.handle();

  if (!mqttClient.connected()) {
    mqttReconnect();
  } else {  
    mqttClient.loop();
  }

  checkIntensity();
    
  showTime(getTime("%H"), getTime("%M"));
  showTemperature(temperature);
  showHumidity(humidity);

  if (WiFi.status() != WL_CONNECTED) {
    display.setLEDoff(LED_WIFI);
  } else {
    display.setLEDon(LED_WIFI);
  }

  display.setLEDon(LED_CELCIUS);
  display.setLEDon(LED_PERCENT);
  display.setLEDon(LED_PERCENT_DOTS);

  blinkSeconds();
}
