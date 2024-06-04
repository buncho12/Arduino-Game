
#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

float X, Y, Z, totalAccel;

int targetPosition;

volatile bool gameActive = true;

double roll = 0.00, pitch = 0.00;  

AsyncDelay delay2s;
  
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  randomSeed(analogRead(0));
  delay2s.start(20000, AsyncDelay::MILLIS);
}

void loop() {

  if(CircuitPlayground.leftButton() && gameActive == false){
    gameActive = true;
  }

  if(gameActive = true){

    targetPosition = random(0, 10);
  
  while (!delay2s.isExpired()){
  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  double x_Buff = X; //code adapted from https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
  double y_Buff = Y;
  double z_Buff = Z;
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3; 

  totalAccel = (int) sqrt(roll*roll + pitch*pitch);
  totalAccel = map(totalAccel, 0, 50, 0, 10);
  Serial.println(totalAccel);

  CircuitPlayground.setPixelColor(totalAccel, 255, 0, 0);
  CircuitPlayground.setPixelColor(targetPosition, 0, 255, 0);
  delay(100);
  CircuitPlayground.clearPixels();

    if (CircuitPlayground.rightButton()) {
    if (totalAccel == targetPosition) {
      CircuitPlayground.playTone(1000, 500);
      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 0, 255, 0);
        delay2s.expire();
      }
    } else {
      fail();
    }
  }

  }

  if(delay2s.isExpired()){
    delay2s.restart();
  }

  } else {
    CircuitPlayground.clearPixels();
  }

}

//issues: tilt thing doesn't work like i want it to, and I don't know how to stop flickering

void fail() {
  CircuitPlayground.playTone(200, 500);
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 255, 0, 0);
  }   
  gameActive = false;
}