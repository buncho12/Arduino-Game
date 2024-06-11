
#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

float X, Y, Z, totalAccel;

int targetPosition, score = 0;

volatile bool gameActive = true;

double roll = 0.00, pitch = 0.00;  

AsyncDelay delay2s;
  
void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
  randomSeed(analogRead(0));
  delay2s.start(5000, AsyncDelay::MILLIS);
}

void loop() {

  if(CircuitPlayground.leftButton() && gameActive == false){ // Turns the game back on once you fail
    gameActive = true;
  } 

  if(gameActive == true){

    targetPosition = random(0, 10); 
  
  while (!delay2s.isExpired()){ // Game runs whenever the timer has not expired

  X = 0; // Code block takes inputs from the accelerometer to sense tilt
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }

  double x_Buff = X; // Code adapted from https://wiki.dfrobot.com/How_to_Use_a_Three-Axis_Accelerometer_for_Tilt_Sensing
  double y_Buff = Y; // Used to compute the roll and pitch of the board for tilt sensing
  double z_Buff = Z;
  roll = atan2(y_Buff , z_Buff) * 57.3;
  pitch = atan2((- x_Buff) , sqrt(y_Buff * y_Buff + z_Buff * z_Buff)) * 57.3; 

  totalAccel = (int) sqrt(roll*roll + pitch*pitch); // Finds the magnitude of pitch and roll to result in the tilt of the board
  totalAccel = map(totalAccel, 0, 50, 0, 10); // Maps tilt from incoming values to a value 0 to 10 

  CircuitPlayground.setPixelColor(totalAccel, 255, 0, 0); // Initializes a red light corresponding to the mapped total acceleration/tilt
  CircuitPlayground.setPixelColor(targetPosition, 0, 255, 0); // Initializes a green light at a random location 
  delay(100); // Checks each parameter every 100 ms
  CircuitPlayground.clearPixels();

    if (CircuitPlayground.rightButton() && totalAccel == targetPosition){ 
   
      for (int i = 0; i < 10; i++) { // Sets the entire board to a green light and plays a tone if the player successfully lines up the green and red light
        CircuitPlayground.setPixelColor(i, 0, 255, 0);
      }

      score += 1; // Adds 1 to the score each time a successful round is made
      Serial.println("Score: ");
      Serial.println(score);
      CircuitPlayground.playTone(1000, 500);

      if(score == 20){ // If the player reaches 20 score, they win. The game then stops until they decide to play again
        Serial.println("You win! Press the left button to play again!");
        gameActive = false;
        delay2s.expire();
        break;
      }

      delay2s.expire();

    } else if (CircuitPlayground.rightButton() && totalAccel != targetPosition){ // If the player times the button press incorrectly, the game stops.

      for (int i = 0; i < 10; i++) {
        CircuitPlayground.setPixelColor(i, 255, 0, 0);
      }  
      CircuitPlayground.playTone(200, 500);
      CircuitPlayground.clearPixels();

      score = 0; // Resets the score to 0
      Serial.println("You suck! Press the left button to play again!");
      gameActive = false;
      delay2s.expire();
      break;
    }

  }

  }

  if(delay2s.isExpired()) // Restarts the delay once it expires in order to change the position of the green light.
    delay2s.restart();

}
