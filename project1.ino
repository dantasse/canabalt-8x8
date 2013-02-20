
#include <avr/io.h>

// The pin numbers on the LED matrix (in case I have to change them)
// so "pin1 = 12" means that LED matrix pin 1 is in Arduino D12
const int pin1 = 12;
const int pin2 = 15;
const int pin3 = 1;
const int pin4 = 2;
const int pin5 = 16;
const int pin6 = 4;
const int pin7 = 14;
const int pin8 = 10;
const int pin9 = 8;
const int pin10 = 3;
const int pin11 = 5;
const int pin12 = 11;
const int pin13 = 0;
const int pin14 = 9;
const int pin15 = 6;
const int pin16 = 7;

// Translating LED pin numbers into sensible coordinates
const int rows[] = {pin9, pin14, pin8, pin12, pin1, pin7, pin2, pin5};
const int rowsLen = 8;
const int cols[] = {pin13, pin3, pin4, pin10, pin6, pin11, pin15, pin16};
const int colsLen = 8;

const int debugPin = 13;

const int DROP = -1;

// array to easily reverse a byte
int reverse[256];

boolean jumping = false;
int yPos = 4;
long lastYPosMoveTime = millis();
long lastBuildingMoveTime = millis();
int moveRate = 500;
int score = 0;

// heights of the buildings
int heights[] = {3, 3, 3, 3, 3, DROP, 2, 2};

void restart() {
  jumping = false;
  yPos = 4;
  lastYPosMoveTime = millis();
  lastBuildingMoveTime = millis();
  moveRate = 500;
  score = 0;
  heights[0] = 3;
  heights[1] = 3;
  heights[2] = 3;
  heights[3] = 3;
  heights[4] = 3;
  heights[5] = DROP;
  heights[6] = 2;
  heights[7] = 2;
}

void setup() {
  // set every row and column to output
  // DDRD = pins 0 to 7 = columns
  // DDRB = 5 rows, DDRC = the other 3 rows
  DDRD = B11111111;
  DDRB = B00111111; // set 5 rows + D13 (debug) to turn it off
  DDRC = B00000111;
  PORTB = B00000000; // B and C are rows. setting 0 = on, 1 = off
  PORTC = B00000000;
  
  for (int i = 0; i < 256; i++) {
    int r = i;
    r = (r & B00001111) << 4 | (r & B11110000) >> 4;
    r = (r & B00110011) << 2 | (r & B11001100) >> 2;
    r = (r & B01010101) << 1 | (r & B10101010) >> 1;
    reverse[i] = r;
  }
}

int data[] = {
  B00000000, 
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

int zero[] = {
  B00000000,
  B00001110,
  B00001010,
  B00001010,
  B00001010,
  B00001110,
  B00000000,
  B00000000
};
int one[] = {
  B00000000,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000010,
  B00000000,
  B00000000
};
int two[] = {
  B00000000,
  B00001110,
  B00000010,
  B00000100,
  B00001000,
  B00001110,
  B00000000,
  B00000000
};
int three[] = {
  B00000000,
  B00001110,
  B00000010,
  B00001110,
  B00000010,
  B00001110,
  B00000000,
  B00000000
};
int four[] = {
  B00000000,
  B00001010,
  B00001010,
  B00001110,
  B00000010,
  B00000010,
  B00000000,
  B00000000
};
int five[] = {
  B00000000,
  B00001110,
  B00001000,
  B00001110,
  B00000010,
  B00001110,
  B00000000,
  B00000000
};
int six[] = {
  B00000000, 
  B00001110,
  B00001000,
  B00001110,
  B00001010,
  B00001110,
  B00000000,
  B00000000
};
int seven[] = {
  B00000000,
  B00001110,
  B00000010,
  B00000010,
  B00000100,
  B00001000,
  B00000000,
  B00000000
};
int eight[] = {
  B00000000,
  B00001110,
  B00001010,
  B00001110,
  B00001010,
  B00001110,
  B00000000,
  B00000000
};
int nine[] = {
  B00000000,
  B00001110,
  B00001010,
  B00001110,
  B00000010,
  B00001110,
  B00000000,
  B00000000
};
int* numbers[] = {zero, one, two, three, four, five, six, seven, eight, nine};

// TODO pass data[] as a param to this function
// or not, whatever, everything's global here anyway
void displayData() {
  
  // display data[]
  for (int i = 0; i < rowsLen; i++) {
    PORTD = reverse[data[i]];
    if (i < 5) {
      PORTB = B011111 & ~(1 << i); // turn off D13
      PORTC = B111;
    } else {
      PORTB = B11111;
      PORTC = ~(1 << (i - 5));
    }
    delayMicroseconds(200); //give the light time to shine
  }
}

// displays score for a certain number of milliseconds
void displayScore(int ms) {
  int ones = score % 10;
  int tens = (score % 100) / 10;
  int hundreds = score / 100;
  
  for (int i = 0; i < 8; i++) {
    data[i] = numbers[tens][i];
    if (hundreds > 0) {
      data[i] |= (numbers[hundreds][i] << 4);
    }
  }
  long start = millis();
  while (millis() - start < ms) {
    displayData();
  }
}

void die() {
  PORTD = B11111111;
  PORTB = B00000000;
  PORTC = B00000000;
  delay(500);
  displayScore(3000);
  restart();
}


void loop() {

  // if you're on the ground and you hit the jump button
  if ((yPos == heights[1] + 1) && (PINC & B1000)) {
    jumping = true;
  }

  long now = millis();
  if ((now - lastBuildingMoveTime) > moveRate) {
    // cycle heights
    for (int i = 0; i < 7; i++) {
      heights[i] = heights[i+1];
    }
    if (heights[6] == DROP) {
      heights[7] = random(5);
    } else { // heights[6] != DROP
      if (random(5) == 0) {
        heights[7] = DROP;
      } else {
        heights[7] = heights[6];
      }
    }
   
    // speed up
    if (moveRate > 400) {
      moveRate -= 10;
    } else if (moveRate > 300) {
      moveRate -= 8;
    } else if (moveRate > 200) {
      moveRate -= 5;
    }
    score++;
    lastBuildingMoveTime = millis();
  }
  
  // process a vertical move (up or down)
  if ((now - lastYPosMoveTime) > 120) {
    if (yPos >= 7) {
      jumping = false;
    }
    if (jumping && (PINC & B1000)) { // keep jumping if you're holding button
      yPos++;
    } else if (yPos == heights[1] + 1) {
      // do nothing; it's on a roof
    } else {
      yPos--;
    }
    lastYPosMoveTime = millis();
  }
  
  // check for death
  if (yPos <= 0 || yPos <= heights[1]) {
    die();
  }
    
  // set data[]:
  for (int i = 0; i < 8; i++) {
    data[i] = 0;
  }
  // add our hero
  data[7 - yPos] |= B01000000;
  // add buildings
  for (int col = 0; col < 8; col++) {
    if (heights[col] != DROP) {
      for (int i = 7 - heights[col]; i < 8; i++) {
        data[i] |= (1 << (7 - col));
      }
    }
  }
  
  displayData();
}
