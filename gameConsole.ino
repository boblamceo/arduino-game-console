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

bool containsObstacle(char* array, int startIndex, int endIndex) {
  for (int i = startIndex; i <= endIndex; i++) {
    if (array[i] == 'i') {
      return true;
    }
  }
  return false;
}

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
char drawMatrix[2][17] = { "                ", "                " };

void newScreen() {
  lcd.clear();

  for (int row = 0; row < 2; row++) {
    lcd.setCursor(0, row);
    lcd.print(drawMatrix[row]);
  }
  lcd.setCursor(drawPos[0], drawPos[1]);
  lcd.print("x");

  delay(500);
}

void draw() {
  char currDirection;
  currDirection = direction(X_pin, Y_pin);
  switch (currDirection) {
    case 'r':
      if (drawPos[0] < 15) {
        drawPos[0] += 1;
        newScreen();
      }
      break;
    case 'l':
      if (drawPos[0] > 0) {
        drawPos[0] -= 1;
        newScreen();
      }
      break;
    case 'u':
      if (drawPos[1] == 1) {
        drawPos[1] = 0;
        newScreen();
      }
      break;
    case 'd':
      if (drawPos[1] == 0) {
        drawPos[1] = 1;
        newScreen();
      }
      break;
    default:
      break;
  }
  if (digitalRead(SW_pin) == 0) {
    if (drawMatrix[drawPos[1]][drawPos[0]] == 'o') {
      drawMatrix[drawPos[1]][drawPos[0]] = ' ';
    } else {
      drawMatrix[drawPos[1]][drawPos[0]] = 'o';
    }

    Serial.println(drawPos[1]);
    Serial.println(drawPos[0]);
    newScreen();
  }
}

bool charJumped = false;
bool charJumpedTwo = false;

char board[17] = "                ";

int score = 0;

void jump() {
  lcd.setCursor(9 - (score == 0 ? 1 : (int)log10(abs(score)) + 1), 0);
  lcd.print("Score: " + String(score));
  int randomNumber;
  randomNumber = random(4, 10);
  bool hasObstacle;
  hasObstacle = containsObstacle(board, 15 - randomNumber, 15);
  if(!hasObstacle){
    board[15] = 'i';
  }

  for (int i = 0; i < 15; i++) {
    board[i] = board[i + 1];
  }
  board[15] = ' ';
  lcd.setCursor(0, 1);
  lcd.print(board);
  if(charJumpedTwo){
    charJumped = false;
    charJumpedTwo = false;
  }
     lcd.setCursor(1, 0);
  if(charJumped){
    charJumpedTwo = true;
    lcd.print("o");
    if(board[1] == 'i'){
      score += 1;
    }
  } else {
    lcd.print("  ");
    lcd.setCursor(1, 1);
    lcd.print("o");
    if(board[1] == 'i'){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GAME OVER");
      lcd.setCursor(0, 1);
      lcd.print("Score: " + String(score));
      while(true){

      }
    }
  }

  char currentDirection;
  currentDirection = direction(X_pin, Y_pin);
  if(currentDirection == 'u'){
    charJumped = true;
  }
  /*
    1. if obstacles, move them left by 1
    2. generate a random number from 5 to 7. If there aren't any obstacles for the last x squares, generate obstacle on last square
    3. if up, then go down
    4. if joystick up, then jump
  */
  if(score >= 15){
delay(50);
  } else if(score >= 10){
    delay(200);
  } else if(score >= 5){
    delay(500);
  } else {
    delay(700);
  }
    
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
        delay(1000);
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
