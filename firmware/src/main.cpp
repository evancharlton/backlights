#define FASTLED_ESP8266_NODEMCU_PIN_ORDER

#include <Arduino.h>
#include <FastLED.h>
#include <WiFiManager.h>
#include <FirebaseESP8266.h>
#include "config.h"

#define NUM_LEDS_PER_MONITOR 9

#define COLOR_MODE BRG
// #define COLOR_MODE GRB

#define DATA_PIN 5

FirebaseData rgbaLeft;
FirebaseData rgbaRight;

CRGB leds[NUM_LEDS_PER_MONITOR * 2];

void readRGBA(FirebaseData &dbo, uint8_t monitor_id);

void setup()
{
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, DATA_PIN, COLOR_MODE>(leds, NUM_LEDS_PER_MONITOR * 2); // GRB ordering is assumed

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  bool white = false;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");

    FastLED.showColor(white ? CRGB::White : CRGB::Blue);
    white = !white;
    delay(300);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //4. Enable auto reconnect the WiFi when connection lost
  Firebase.reconnectWiFi(true);

  if (!Firebase.beginStream(rgbaLeft, "/rgba/left"))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection for RGBA (left) ...");
    Serial.println("REASON: " + rgbaLeft.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
    FastLED.showColor(CRGB(0xFF, 0x00, 0x00));
    return;
  }

  if (!Firebase.beginStream(rgbaRight, "/rgba/right"))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't begin stream connection for RGBA (right) ...");
    Serial.println("REASON: " + rgbaRight.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
    FastLED.showColor(CRGB(0xFF, 0x00, 0x00));
    return;
  }
}

void loop()
{
  readRGBA(rgbaRight, 0);
  readRGBA(rgbaLeft, 1);
}

void readRGBA(FirebaseData &dbo, uint8_t monitorNum /* 0 or 1 */)
{
  if (!Firebase.readStream(dbo))
  {
    Serial.println("------------------------------------");
    Serial.println("Can't read stream data for RGBA ...");
    Serial.println("REASON: " + dbo.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
    return;
  }

  if (dbo.streamTimeout())
  {
    Serial.println("Stream timeout, resume streaming...");
    Serial.println();
    return;
  }

  if (!dbo.streamAvailable())
  {
    return;
  }

  if (dbo.dataType() != "string")
  {
    Serial.println("Error: unknown data type for RGBA; ignoring");
    Serial.print("\tReceived: ");
    Serial.println(dbo.dataType());
    return;
  }

  String hex = dbo.stringData();
  Serial.print("New rgba data received: ");
  Serial.println(hex);

  uint32_t rgba = strtoul(hex.c_str(), NULL, 16);
  uint8_t r = (rgba >> 24) & 0xFF;
  uint8_t g = (rgba >> 16) & 0xFF;
  uint8_t b = (rgba >> 8) & 0xFF;
  uint8_t a = (rgba >> 0) & 0xFF;

  CRGB color = CRGB(r, g, b);

  uint8_t offset = monitorNum * NUM_LEDS_PER_MONITOR;
  for (uint8_t i = 0; i < NUM_LEDS_PER_MONITOR; i++)
  {
    leds[offset + i] = color;
  }
  FastLED.setBrightness(0xFF);
  FastLED.show();
}