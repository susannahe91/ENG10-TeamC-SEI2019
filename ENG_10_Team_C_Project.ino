/*
 * ENG 10 Team C -- Got Sleep?
 * Team Members: Aurelio Perez, Ethan Stephenson, Nick Krolikowski, Susanna He
 * Project: Roommate-Friendly Alarm
 */

#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include <SPI.h>

#define MOVE_THRESHOLD 50

float value; // value of sound pressure level
bool leftButtonPressed;
bool rightButtonPressed;
int inputHour = 0;
float X, Y, Z; // variables to store the respective coordinates

void setup() 
{
  Serial.begin(9600);
  CircuitPlayground.begin();
}

bool isMoving()
{
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();  

  double storedVector = X*X;
  storedVector += Y*Y;
  storedVector += Z*Z;
  storedVector = sqrt(storedVector);

  delay(100);
  
  X = CircuitPlayground.motionX();
  Y = CircuitPlayground.motionY();
  Z = CircuitPlayground.motionZ();

  double newVector = X*X;
  newVector += Y*Y;
  newVector += Z*Z;
  newVector = sqrt(newVector); 
  
  if (abs(10*newVector - 10*storedVector) > MOVE_THRESHOLD) 
  {
    return true;
  }
  return false;
}

void loop() 
{
  leftButtonPressed = CircuitPlayground.leftButton();
  rightButtonPressed = CircuitPlayground.rightButton();
  value = CircuitPlayground.mic.soundPressureLevel(30);  // Take 30 milliseconds of sound data to calculate  
  bool deactivated = false;
  bool activated = false;

  if (leftButtonPressed)
  {
    deactivated = false;
    inputHour++;
    CircuitPlayground.redLED(HIGH);
  }

  if (rightButtonPressed) 
  {
    CircuitPlayground.redLED(HIGH);
    delay(500);
    CircuitPlayground.redLED(LOW);
    int i;
    for(i = 0; i < inputHour; i++)
    {   
      delay(1000); //1000ms = 1 second, for testing
      //delay(3600000); //1 hour         
    }
    activated = true;
    while(!deactivated)
    {
      value = CircuitPlayground.mic.soundPressureLevel(30);          
      if(value > 90 || isMoving())
      {
        deactivated = true;
        activated = false;
        inputHour = 0;
      }        
      delay(250);
      
      CircuitPlayground.playTone(330, 2200, false);
      if(value > 90 || isMoving())
      {
        deactivated = true;
        activated = false;
        inputHour = 0;
      }   
      delay(250);
      CircuitPlayground.playTone(262, 2200, false);
      if(value > 90 || isMoving())
      {
        deactivated = true;
        activated = false;
        inputHour = 0;
      }        
    }
  }
  delay(1000);
  CircuitPlayground.redLED(LOW);
}
