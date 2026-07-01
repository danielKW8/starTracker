#include <Arduino.h>
#include <FastAccelStepper.h>

// ----------------------------------- STEPPER DRIVER CONFIG!!!!! -----------------------------------
namespace StepperData {
  //Pin Assignments
  const int stepPin = 6; 
  const int dirPin = 5;   
  const int MS1 = 11;
  const int MS2 = 10;
  const int MS3 = 9;
  const int shutter = 2;
  //stepper settings

  const int steps = 3200;
  const int gearRatio = 200; 

  //Timing
  const int timeInterval = 10; // Time interval in seconds for the stepper to move
  const float siderealDay = 86164.0905; // Sidereal day in seconds  
}

// ----------------------------------- CALCULATIONS -----------------------------------
namespace calculations {
  const float degreesPerSecond = (360.0 / StepperData::siderealDay) * StepperData::gearRatio; // Calculate degrees per second based on sidereal day and gear ratio

  const float degreesPerInterval = degreesPerSecond * StepperData::timeInterval; // Calculate degrees to move in the given time interval

  //MOTOR SPEED
  const float stepsToMovePerInterval = (degreesPerInterval * StepperData::steps) / 360.0; // Calculate the speed in steps to move per interval
}

// ----------------------------------- Initialize Engine -----------------------------------
FastAccelStepperEngine engine = FastAccelStepperEngine();

FastAccelStepper *stepper;

void stepperMode() {
  pinMode(StepperData::MS1, OUTPUT);
  pinMode(StepperData::MS2, OUTPUT);
  pinMode(StepperData::MS3, OUTPUT);

  // Set the microstepping mode to 1/16
  digitalWrite(StepperData::MS1, HIGH);
  digitalWrite(StepperData::MS2, HIGH);
  digitalWrite(StepperData::MS3, HIGH);
}

void setup() {
  Serial.begin(9600);

  engine.init();
  stepperMode(); // Set the microstepping mode before connecting the stepper

  stepper = engine.stepperConnectToPin(StepperData::stepPin);

  if (stepper) {
    stepper->setDirectionPin(StepperData::dirPin);
    stepper->setAutoEnable(true); // Automatically enable the stepper when stepping
    stepper->setSpeedInHz(calculations::stepsToMovePerInterval); // Set speed in Hz (steps per second)
    stepper->setAcceleration(1000); // Set acceleration in steps/s^2

    Serial.println("Stepper initialized.");
  } else {
    Serial.println("Failed to initialize stepper.");
  }
}

// put function definitions here:
//run stepper degrees to steps
void degreesToSteps(float degrees) {
  int stepsToMove = (degrees * StepperData::steps) / 360;
  stepper->move(stepsToMove);
  Serial.print("Moving stepper " + String(stepsToMove) + " steps (" + String(degrees) + " degrees).");


  // Wait until the stepper has finished moving
  while (stepper->isRunning()) {
    delay(StepperData::timeInterval * 1000); //wait timeInterval seconds
    Serial.println("Interval: " + String(StepperData::timeInterval) + "seconds");
    // Wait until the stepper has finished moving
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  degreesToSteps(calculations::degreesPerInterval);
}
