

// Library
#include <LiquidCrystal.h>

#include "DHT.h"

#define DHTPIN 8

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int temp = dht.readTemperature();
int humid = dht.readHumidity();


void setup() {
  lcd.begin(16, 2);
  analogWrite(8,15);
}

void loop() {

  dht.begin();
  
  lcd.setCursor(0, 0);
  lcd.print("Temp. : ");
  temp = dht.readTemperature();
  lcd.print(temp);  
  lcd.setCursor(0, 1);
  lcd.print("Humid. : ");
  humid = dht.readHumidity();
  lcd.print(humid);
  lcd.print("%");
  
  delay(1000);
}
