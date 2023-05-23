
// alla bibotek som behövs är här.
#include "U8glib.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

unsigned long delayTime;
bool buttonPressed = false;
int page = 0;

void setup() {
  u8g.begin();
  u8g.setFont(u8g_font_8x13B); // Set the font for displaying text
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();

  Serial.begin(9600);
  while (!Serial);

  unsigned status;

  // default settings
  status = bme.begin();

  if (!status) {
    while (1) delay(10); // Infinite loop to handle BME280 initialization failure
  }

  delayTime = 200;
}

void loop() {
  u8g.firstPage();
  do {
    printValues(); // visa mätningarna på skärmen
  } while (u8g.nextPage());

  delay(delayTime);

  Serial.println(page);
// sak som kolla om en knapp trycks ner eller inte. Om knapp trycks så ska page öka med 1
  if (digitalRead(6) == HIGH && !buttonPressed) {
    buttonPressed = true;
    page = page + 1;
    if (page == 4) {
      page = 0;
    }
    Serial.println("Button pressed");
  } else if (digitalRead(7) == LOW) {
    buttonPressed = false;
    Serial.println("Button released");
  }

}



void printValues() {
  u8g.setFont(u8g_font_6x10);
// beroende på vad page är visar den olika saker på skärmen. 
  switch (page) {
    case 0:
      u8g.setPrintPos(0, 30); // Set the position for printing text
      u8g.print("Temperature = ");
      u8g.print(bme.readTemperature());
      u8g.print(" ");
      u8g.print((char)176);
      u8g.print("C");
      break;
    case 1:
      u8g.setPrintPos(0, 30);
      u8g.print("Pressure = ");
      u8g.print(bme.readPressure() / 100.0F);
      u8g.print(" hPa");
      break;
    case 2:
      u8g.setPrintPos(0, 30);
      u8g.print("Altitude = ");
      u8g.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
      u8g.print(" m");
      break;
    case 3:
      u8g.setPrintPos(0, 30);
      u8g.print("Humidity = ");
      u8g.print(bme.readHumidity());
      u8g.print(" %");
      break;

  }
 
}
