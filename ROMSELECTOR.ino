#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//このぷろぐらむ、鴨川ネギが書いたの
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const int outputPin1 = 14; // 出力ピン1のピン番号
const int outputPin2 = 15; // 出力ピン2のピン番号
const int outputPin3 = 16; // 出力ピン3のピン番号
int number = 0;
int lastSwitchState1 = HIGH;
int lastSwitchState2 = HIGH;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long debounceDelay = 50;

const char* titles[] = {"Title 1", "Title 2", "Title 3", "Title 4", "Title 5", "Title 6", "Title 7", "Title 8"}; // タイトルを入れるとこ

void setup() {
  Serial.begin(9600); // シリアル通信の初期化して、ボタンたちちゃんと働いてるかウォッチするの
  pinMode(2, INPUT_PULLUP); //ピン二本だけだから直接ピン番号でよくないってなった
  pinMode(4, INPUT_PULLUP);//内部プルアップっていいよね
  pinMode(outputPin1, OUTPUT);
  pinMode(outputPin2, OUTPUT);
  pinMode(outputPin3, OUTPUT);
  
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000); // ウェイトを入れてOLEDが起動するのを待つの
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

}

void loop() {
  int switchState1 = digitalRead(2);
  int switchState2 = digitalRead(4);
  
  if (switchState1 != lastSwitchState1) {
    lastDebounceTime1 = millis();
  }
  if (switchState2 != lastSwitchState2) {
    lastDebounceTime2 = millis();
  }
  
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (switchState1 != lastSwitchState1) {
      lastSwitchState1 = switchState1;
      if (lastSwitchState1 == LOW) {
        number--;
        if (number < 0) {
          number = 7;  // 0未満の場合は7にラッープ！
          Serial.println("Switch 1 Pressed");
        }
      }
    }
  }
  
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (switchState2 != lastSwitchState2) {
      lastSwitchState2 = switchState2;
      if (lastSwitchState2 == LOW) {
        number++;
        if (number > 7) {
          number = 0;  // 7を超える場合は0にラッープ！
          Serial.println("Switch 2 Pressed");
        }
      }
    }
  }

  // タイトルをOLEDに映って貰うの
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(titles[number]);
  display.display();

  // 数字を二進数に変換して出力ピンに出力するの
  digitalWrite(outputPin1, (number >> 2) & 0b1);
  digitalWrite(outputPin2, (number >> 1) & 0b1);
  digitalWrite(outputPin3, number & 0b1);
}
