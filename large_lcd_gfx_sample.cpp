#include "U8glib.h"
#include <DHT.h>
#include <DallasTemperature.h>
#include <OneWire.h>

char temp_string[5]; // empty char array to hold the converted temp float
char hum_string[5];  // empty char array to hold the converted humidity float
float hum_float;

// display constructor for u8g library - this one depends on your type of
// display
U8GLIB_ST7920_128X64 u8g(13, 11, 12, U8G_PIN_NONE);

// DS18B20 section
// ================================================================

// one wire data probe to pin 6
#define ONE_WIRE_BUS_PIN 6

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress Probe_01 = {0x28, 0xFF, 0x0A, 0x75, 0xA3, 0x15, 0x04, 0xBC};
// each DS18B20 has each own unique address - use address finder sketch

// DHT11 section
// ================================================================== tell the
// DHT library where to expect a DHT sensor and which type
DHT DHT_sens(8, DHT11); //  connected to pin 8 of the Arduino

void setup() {

  DHT_sens.begin(); // start DHT sensoring
  sensors.begin();  // START dallas temperature sensoring
  sensors.setResolution(Probe_01,
                        10); // set DS18B20 sensor resolution to 10 bit
  Serial.begin(9600);        // start serial port
  Serial.print("DS18B20 Temperature Control Library Version ");
  Serial.println(DALLASTEMPLIBVERSION);
  Serial.println();
  Serial.println("DHT11 humidity sensor (temp not implemented)");
  Serial.print("Number of Devices found on bus = ");
  Serial.println(sensors.getDeviceCount());
  Serial.println("============================================");
  Serial.print("Getting temperature, humidity... ");
  Serial.println();

  // Select font
  // =================================================================

  // u8g.setFont(u8g_font_unifont);
  // u8g.setFont(u8g_font_courB10);
  // u8g.setFont(u8g_font_7x14B);
  u8g.setFont(u8g_font_8x13);

  u8g.setColorIndex(1); // Instructs the display to draw with a pixel on.
}

void loop() {
  u8g.firstPage();
  do {
    draw();
  } while (u8g.nextPage());
}

void draw() {

  u8g.drawFrame(0, 0, 128, 31);  // upper frame
  u8g.drawFrame(0, 33, 128, 31); // lower frame

  // DS18B20 section
  // ==============================================================
  sensors.requestTemperatures();

  Serial.print("temperature: ");
  float tempC = sensors.getTempC(Probe_01);
  Serial.print(tempC, 1);
  Serial.println(" *C\t");
  if (tempC == -127.00) {
    Serial.print("Error getting temperature  ");
  } else {

    // DHT 11 section
    // ================================================================

    hum_float = DHT_sens.readHumidity();
    Serial.print("Humidity: ");
    Serial.print(hum_float, 1);
    Serial.print(" %\t");
    Serial.println();

    // convert floats into char u8g strings
    u8g.drawStr(15, 13, "Temperature"); // do this for temperature
    dtostrf(tempC, 3, 1, temp_string);
    u8g.drawStr(33, 27, temp_string);
    u8g.drawStr(70, 27, "*C");

    u8g.drawStr(26, 45, "Humidity"); // do this for humidity
    dtostrf(hum_float, 3, 1, hum_string);
    u8g.drawStr(40, 60, hum_string);
    u8g.drawStr(75, 60, "%");
  }
}
