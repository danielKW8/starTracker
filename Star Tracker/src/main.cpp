#include <Arduino.h>
#include <FastAccelStepper.h>

// ----------------------------------- STEPPER DRIVER CONFIG!!!!! -----------------------------------

//MS1 200, MS2 400, MS1+MS2 800, MS1+MS2+MS3 1600, 1/16 = 3200
int steps = 3200;
const int stepPin = 2; // Pin connected to the step input of the stepper driver
const int dirPin = 3;  // Pin connected to the direction input of the stepper driver

// ----------------------------------- IMPORTANT SETTINGS -----------------------------------

int timeInterval = 10; // Time interval in seconds for the stepper to move
int GearRatio = 200;
float siderealDay = 86164.0905; // Sidereal day in seconds

// ----------------------------------- CALCULATIONS -----------------------------------
int degreesPerInterval = degreesPerSecond * timeInterval; // Calculate degrees to move in the given time interval
int degreesPerSecond = (360 / siderealDay) * GearRatio; // Calculate degrees per second based on sidereal day and gear ratio

// ----------------------------------- Initialize Engine -----------------------------------
FastAccelStepperEngine engine = FastAccelStepperEngine();

FastAccelStepper *stepper;

void setup() {
  Serial.begin(9600);

  engine.init();

  stepper = engine.stepperConnectToPin(stepPin);

  if (stepper) {
    stepper->setDirectionPin(dirPin);
    stepper->setEnablePin(4); // Optional: Set an enable pin if your driver has one
    stepper->setAutoEnable(true); // Automatically enable the stepper when stepping

    stepper->setSpeedInHz(1000); // Set speed in Hz (steps per second)

    Serial.println("Stepper initialized.");
  } else {
    Serial.println("Failed to initialize stepper.");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  degreesToSteps(degreesPerInterval);
}

// put function definitions here:
//run stepper degrees to steps
void degreesToSteps(int degrees) {
  int stepsToMove = (degrees * steps) / 360;
  stepper->move(stepsToMove);
  Serial.print("Moving stepper " + String(stepsToMove) + " steps (" + String(degrees) + " degrees).");


  // Wait until the stepper has finished moving
  while (stepper->isRunning()) {
    delay(timeInterval * 1000); //wait timeInterval seconds
    Serial.println("Interval: " + String(timeInterval) + "seconds");
    // Wait until the stepper has finished moving
  }
}
