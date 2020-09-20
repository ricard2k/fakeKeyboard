
#include "Keyboard.h"
#include "Mouse.h"
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
unsigned long activityTimer;
boolean keyboardOrMouse;

bool active;

//Activation button 
bool lastButtonState;
int debounceButton;

void newKeyRetardTimer() {
  keyRetardTimer =  millis() + (5 * random(100));
}

void newActivityTimer() {
  activityTimer =  millis() + (1000 * random(36,89));
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
  active=false;
  lineCounter = 0;
  keyRetardTimer=0;
  keyboardOrMouse=false;
  activityTimer=0;
  
  debounceButton=0;
  lastButtonState=LOW;

  // Set initial state for human feedback signals
  digitalWrite(LED, active);
  
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
      active = !active; 
      digitalWrite(LED, active);
    }
  }
}

void loop() {

  readPushButton();

  if (active) { 
    
    if (activityTimer < millis()) { 
      keyboardOrMouse = !keyboardOrMouse;
      newActivityTimer();
    }

    if (keyboardOrMouse) {
      sendKey();
    } else {
      mouseMove();
    }
  }
  
}
