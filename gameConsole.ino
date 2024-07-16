#include <LiquidCrystal.h>
#include "pitches.h"

// Arduino pin numbers
const int SW_pin = 3; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Draw");
  lcd.setCursor(8, 0);
  lcd.print("Jump");
  lcd.setCursor(0, 1);
  lcd.print("Maze");
  lcd.setCursor(8, 1);
  lcd.print("Guess");
}

char screen = 'h'

void home(){
  
}

void loop() {
  // put your main code here, to run repeatedly:
  switch(screen){
    case 'h':
      break;
    case 'd':
      break;
    case 'j':
      break;
    case 'm':
      break;
    case 'g':
      break;
    default:
      break;
  }
}
