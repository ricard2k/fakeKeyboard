#include <Mouse.h>
#include <Keyboard.h>
#include "limits.h"

#define LED 10
#define PUSH_BUTTON 20

#define MOUSE_DIRECTION_N 1
#define MOUSE_DIRECTION_NE 2
#define MOUSE_DIRECTION_E 3
#define MOUSE_DIRECTION_SE 4
#define MOUSE_DIRECTION_S 5
#define MOUSE_DIRECTION_SW 6
#define MOUSE_DIRECTION_W 7
#define MOUSE_DIRECTION_NW 8

unsigned long keyRetardTimer;
unsigned int lineCounter;
unsigned long keyboardOrMouseTimer;
boolean keyboardOrMouse;

int status;
bool ledStatus;

//Activation button 
bool lastButtonState;
int debounceButton;
unsigned long ledBlinkTimer;

void newKeyRetardTimer() {
  keyRetardTimer =  millis() + (5 * random(100));
}

void newKeyboardOrMouseTimer() {
  keyboardOrMouseTimer =  millis() + (1000 * random(36,89));
}

void newLedBlinkTimer() {
  ledBlinkTimer =  millis() + 200;
}

void setup() { 

  // initialize pins
  pinMode(LED, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT);
 
  // initialize mouse control:
  Mouse.begin();
  Keyboard.begin();

  //initialize random numbers reading fron unconnected pin.
  randomSeed(analogRead(0));

  // Variable initialization
  status=0;
  lineCounter = 0;
  keyRetardTimer=0;
  keyboardOrMouse=false;
  keyboardOrMouseTimer=0;
  
  debounceButton=0;
  lastButtonState=LOW;
  ledStatus=LOW;
  ledBlinkTimer = 0;
  
}

void sendKey() {
  if (keyRetardTimer < millis()) { //Execute one time afer timer ends: < millis()
    lineCounter ++;        
    if (lineCounter > 80) {
        lineCounter = 0;
        Keyboard.write('\n');
    }
    Keyboard.write(125 - random(77));
    newKeyRetardTimer();
  }
}

void mouseMove() {
  long randNumber = random(6);
  switch (random(1,9)) {
    case MOUSE_DIRECTION_N:
      Mouse.move(0, randNumber, 0);
      break;
    case MOUSE_DIRECTION_NE:
      Mouse.move(1.5*randNumber, randNumber, 0);
      break;
    case MOUSE_DIRECTION_E:
      Mouse.move(1.5*randNumber, 0,0);
      break;
    case MOUSE_DIRECTION_SE:
      Mouse.move(1.5*randNumber, -1*randNumber, 0);
      break;
    case MOUSE_DIRECTION_S:
      Mouse.move(0, -1*randNumber, 0);
      break;
    case MOUSE_DIRECTION_SW:
      Mouse.move(-1.5*randNumber, -1*randNumber, 0);
      break;
    case MOUSE_DIRECTION_W:
      Mouse.move(-1.5*randNumber, 0, 0);
      break;
    case MOUSE_DIRECTION_NW:
      Mouse.move(-1.5*randNumber, randNumber,0);
      break;
  }
}

void readPushButton() {
  boolean buttonState = digitalRead(PUSH_BUTTON);

  if ( buttonState != lastButtonState) {
    debounceButton++;
  } else {
    debounceButton=0;
  }

  if (debounceButton > 10) {
    lastButtonState = buttonState;
    if (lastButtonState == HIGH) {
      status = status == 2 ? 0 : status + 1; 
      keyboardOrMouse = true; // set mouse by default
    }
  }
}

void loop() {

  readPushButton();

  switch (status)
  {
  case 1:
    digitalWrite(LED, HIGH);
    mouseMove();
    break;

  case 2:
    digitalWrite(LED, ledStatus);
    if (ledBlinkTimer < millis()) {
      ledStatus = !ledStatus;
      newLedBlinkTimer();
    }

    if (keyboardOrMouseTimer < millis()) { 
      keyboardOrMouse = !keyboardOrMouse;
      newKeyboardOrMouseTimer();
    }

    if (keyboardOrMouse) {
      sendKey();
    } else {
      mouseMove();
    }
    break;

  default:
    digitalWrite(LED, LOW);
    break;
  } 
}
