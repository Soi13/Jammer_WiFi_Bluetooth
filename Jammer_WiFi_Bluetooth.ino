#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <string>
#include "images.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 //or set to the pin you wired to RST
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(9, 10); // CE, CSN
byte i = 45;
const int wifiFrequencies[] = {2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462};
const char* modes[] = {"BLE & All 2.4 GHz", "Just Wi-Fi", "Waiting Idly :("};
uint8_t attack_type = 2;

void displayMessage(const char* line, uint8_t x = 55, uint8_t y = 22, const unsigned char* bitmap = helpy_menu_image) {
  radio.powerDown();
  SPI.end();
  delay(10);

  display.clearDisplay();
  if (bitmap != nullptr) {
    display.drawBitmap(0, 0, bitmap, 128, 64, WHITE);
  }
  display.setTextSize(1);
  String text = String(line);
  int16_t cursor_y = y;
  int16_t maxWidth = 128 - x;
  while (text.length() > 0) {
    int16_t charCount = 0;
    int16_t lineWidth = 0;
    while (charCount < text.length() && lineWidth < maxWidth) {
      charCount++;
      lineWidth = 6 * charCount;
    }
    if (charCount < text.length()) {
      int16_t lastSpace = text.substring(0, charCount).lastIndexOf(' ');
      if (lastSpace > 0) {
        charCount = lastSpace + 1;
      }
    }
    display.setCursor(x, cursor_y);
    display.println(text.substring(0, charCount));
    text = text.substring(charCount);
    cursor_y += 10;
    if (cursor_y > 64) break;
  }
  display.display();

  SPI.begin();
  radio.powerUp();
  delay(5);
  radio.startConstCarrier(RF24_PA_MAX, i);
}

void addvertising() {
  for (size_t i = 0; i < 3; i++) {
    displayMessage("", 60, 22, helpy_big_image);
    delay(310);
    displayMessage("", 60, 22, nullptr);
    delay(300);
  }
  displayMessage("Jammer got up. Click the button and discover all modes!", 65, 6);
}

void fullAttack() {
  for (size_t i = 0; i < 80; i++) {
    radio.setChannel(i);
  }
}

void wifiAttack() {
  for (int i = 0; i < sizeof(wifiFrequencies) / sizeof(wifiFrequencies[0]); i++) {
    radio.setChannel(wifiFrequencies[i] - 2400);
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C or 0x3D
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("Hello!");
  display.display();
  delay(1000);
  if (radio.begin()) {
    delay(200);
    radio.setAutoAck(false); 
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(5);
    radio.setAddressWidth(3);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.printPrettyDetails();
    radio.startConstCarrier(RF24_PA_MAX, i);
    addvertising();
  } else {
    Serial.println("BLE Jammer couldn't be started!");
    displayMessage("Jammer Error!");
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
