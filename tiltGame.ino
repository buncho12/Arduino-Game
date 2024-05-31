#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

AsyncDelay 

int targetPosition;
volatile bool gameActive = true;
float x, y, nx, ny, currentPosition

void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
}

/* add a timer for the game, you only have a few seconds to get it right and make it decrease each time 
   also make sure that the angle calculations work */

void loop() {

  if(CircuitPlayground.leftButton() && gameActive == false){
    gameActive = true;
  }

  targetPosition = random(0, 10);

  x = CircuitPlayground.motionX();
  y = CircuitPlayground.motionY();
  nx = x / 10.0;
  ny = y / 10.0;
  currentPosition = atan((ny/nx)) * 180 / 3.14; 
  if(currentPosition > 0.0) { 
    if(nx < 0.0)
      currentPosition += 180;
  } else { 
    if(ny > 0.0)
      currentPosition += 180;
    else
      currentPosition += 360;
  }
  if(currentPosition == 360.0) 
    currentPosition = 0;
  
  currentPosition = (int) angle % 10;

  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(currentPosition, 0, 255, 0);
  CircuitPlayground.setPixelColor(targetPosition, 255, 0, 0);

  if (CircuitPlayground.rightButton() && gameActive) {
    if (currentPosition == targetPosition) {
      CircuitPlayground.playTone(1000, 500);
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 0, 255, 0);
      }
    } else {
      CircuitPlayground.playTone(200, 500);
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.strip.setPixelColor(i, 255, 0, 0);
      }
    }
    CircuitPlayground.strip.show();
    gameActive = false;
  }

  delay(50);  
}