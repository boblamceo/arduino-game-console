#include <LiquidCrystal.h>
#include "pitches.h"

// Arduino pin numbers
const int SW_pin = 3;  // digital pin connected to switch output
const int X_pin = 0;   // analog pin connected to X output
const int Y_pin = 1;   // analog pin connected to Y output

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int cursorX = 0;
int cursorY = 0;

char direction(int X_pin, int Y_pin) {
  if (analogRead(X_pin) > 600) {
    return 'r';
  } else if (analogRead(X_pin) < 400) {
    return 'l';
  } else if (analogRead(Y_pin) > 600) {
    return 'd';
  } else if (analogRead(Y_pin) < 400) {
    return 'u';
  }
  return 'n';
}

void setScreen(char screen) {
  lcd.clear();
  switch (screen) {
    case 'd':
      lcd.setCursor(0, 0);
      lcd.print("Draw<");
      lcd.setCursor(10, 0);
      lcd.print("  Jump");
      lcd.setCursor(0, 1);
      lcd.print("Maze");
      lcd.setCursor(10, 1);
      lcd.print(" Guess");
      lcd.setCursor(0, 0);
      cursorX = 0;
      cursorY = 0;
      break;
    case 'j':
      lcd.setCursor(0, 0);
      lcd.print("Draw");
      lcd.setCursor(10, 0);
      lcd.print(" >Jump");
      lcd.setCursor(0, 1);
      lcd.print("Maze");
      lcd.setCursor(10, 1);
      lcd.print(" Guess");
      lcd.setCursor(10, 0);
      cursorX = 10;
      cursorY = 0;
      break;
    case 'm':
      lcd.setCursor(0, 0);
      lcd.print("Draw");
      lcd.setCursor(10, 0);
      lcd.print("  Jump");
      lcd.setCursor(0, 1);
      lcd.print("Maze<");
      lcd.setCursor(10, 1);
      lcd.print(" Guess");
      lcd.setCursor(0, 1);
      cursorX = 0;
      cursorY = 1;
      break;
    case 'g':
      lcd.setCursor(0, 0);
      lcd.print("Draw");
      lcd.setCursor(10, 0);
      lcd.print("  Jump");
      lcd.setCursor(0, 1);
      lcd.print("Maze");
      lcd.setCursor(10, 1);
      lcd.print(">Guess");
      lcd.setCursor(10, 1);
      cursorX = 10;
      cursorY = 1;
      break;
    default:
      break;
  }
}

void setup() {
  pinMode(SW_pin, INPUT_PULLUP);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  lcd.begin(16, 2);
  setScreen('d');
}

char screen = 'h';

int drawPos[2] = { 0, 0 };
char drawMatrix[2][16] = { { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }, { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' } };

void newScreen() {
  lcd.clear();

  for (int row = 0; row < 2; row++) {
    for (int square = 0; square < 16; square++) {
      Serial.println(row);
      Serial.println(square);
      char matrixSquare = drawMatrix[row][square];
      lcd.setCursor(row, square);
      lcd.print(matrixSquare);
    }
  }
  lcd.setCursor(drawPos[0], drawPos[1]);
  lcd.print("x");
}

void draw() {
  char currDirection;
  currDirection = direction(X_pin, Y_pin);
  switch (currDirection) {
    case 'r':
      drawPos[0] += 1;
      newScreen();
      break;
  }
}

void jump() {
  lcd.setCursor(0, 0);
  lcd.print("Jump");
}

void maze() {
  lcd.setCursor(0, 0);
  lcd.print("Maze");
}

void guess() {
  lcd.setCursor(0, 0);
  lcd.print("Guess");
}

void home() {
  char currentDirection;
  currentDirection = direction(X_pin, Y_pin);
  switch (currentDirection) {
    case 'r':
      if (cursorX == 0) {
        if (cursorY == 0) {
          setScreen('j');
        } else {
          setScreen('g');
        }
      }
      break;
    case 'l':
      if (cursorX == 10) {
        if (cursorY == 0) {
          setScreen('d');
        } else {
          setScreen('m');
        }
      }
      break;
    case 'd':
      if (cursorY == 0) {
        if (cursorX == 0) {
          setScreen('m');
        } else {
          setScreen('g');
        }
      }
      break;
    case 'u':
      if (cursorY == 1) {
        if (cursorX == 0) {
          setScreen('d');
        } else {
          setScreen('j');
        }
      }
      break;
    default:
      break;
  }
  if (digitalRead(SW_pin) == 0) {
    Serial.println("YUP");
    if (cursorX == 0) {
      if (cursorY == 0) {
        screen = 'd';
        newScreen();
      } else {
        screen = 'm';
        lcd.clear();
      }
    } else {
      if (cursorY == 0) {
        screen = 'j';
        lcd.clear();
      } else {
        screen = 'g';
        lcd.clear();
      }
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (screen) {
    case 'h':
      home();
      break;
    case 'd':
      draw();
      break;
    case 'j':
      jump();
      break;
    case 'm':
      maze();
      break;
    case 'g':
      guess();
      break;
    default:
      break;
  }
}
