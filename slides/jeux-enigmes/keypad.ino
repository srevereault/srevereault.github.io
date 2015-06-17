/* Keypad BRAIN
 *
 * Inspiré des tutos Arduino Keypad et LCD 1604
 *
 * Needed libraries
 * http://arduino.cc/playground/uploads/Code/Keypad.zip
 * http://arduino.cc/playground/uploads/Code/Password.zip
 */

#include <Keypad.h>
#include <Password.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int locked = 1;
int passinput = 0;
int lockedled = 12;
int unlockedled = 13;
int passpos = 0;

long ledflashvar = 0;
long ledflashtime = 300;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {2, 3, 4, 5};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Password password = Password("BA45");

void setup(){
  pinMode(lockedled, OUTPUT);
  pinMode(unlockedled, OUTPUT);
  lcd.init();
  lcd.backlight();
  reset_digicode();
}

void reset_digicode(){
  digitalWrite(lockedled, 255);
  digitalWrite(unlockedled, 0);
  lcd.setCursor(3, 0);
  lcd.print("B.R.A.I.N.");
  lcd.setCursor(0, 1);
  lcd.print("Entrer code ____");
}

void loop(){
  char key = keypad.getKey();
  if(locked){
    if(passinput){
      unsigned long ledcurrentvar = millis();
      if(ledcurrentvar - ledflashvar > ledflashtime) {
        ledflashvar = ledcurrentvar;
        digitalWrite(lockedled, !digitalRead(lockedled));
      }
    }
    else{
      digitalWrite(lockedled, 255);
    }
    digitalWrite(unlockedled, 0);
  }
  if (key != NO_KEY){
    //Serial.println(key);
    lcd.setCursor(12+passpos, 1);
    lcd.print(key);
    password.append(key);
    passinput = 1;
    passpos += 1;
    if(key == '*'){
      lcd.setCursor(12, 1);
      lcd.print("____");
      password.reset();
      passpos = 0;
      passinput = 0;
      locked = 1;
    }
    if(password.evaluate()) {
      locked = !locked;
      password.reset();
      passpos = 0;
      passinput = 0;
      digitalWrite(lockedled, 0);
      digitalWrite(unlockedled, 255);
      lcd.setCursor(0, 0);
      lcd.print("   B.R.A.V.O.   ");
      lcd.setCursor(0, 1);
      lcd.print("Pousser la porte");
      delay(5000);
      reset_digicode();
    }
  }
}
