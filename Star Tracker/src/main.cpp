#include <Arduino.h>
#include <AccelStepper.h>

namespace StepperData {
  const int stepPin = 6; 
  const int dirPin = 5;   
  const int MS1 = 11;
  const int MS2 = 10;
  const int MS3 = 9;
  const int shutter = 2;

  const int steps = 3200;      // full steps * microstepping (16x)
  const int gearRatio = 200; 

  const int timeInterval = 10;
  const float siderealDay = 86164.0905;
}

namespace calculations {
  const float degreesPerSecond = (360.0 / StepperData::siderealDay) * StepperData::gearRatio; // Calculate degrees per second based on sidereal day and gear ratio

  const float degreesPerInterval = degreesPerSecond * StepperData::timeInterval; // Calculate degrees to move in the given time interval

  //MOTOR SPEED
  const float stepsToMovePerInterval = (degreesPerInterval * StepperData::steps) / 360.0; // Calculate the speed in steps to move per interval
}

AccelStepper stepper(AccelStepper::DRIVER, StepperData::stepPin, StepperData::dirPin);

void stepperMode() {
  pinMode(StepperData::MS1, OUTPUT);
  pinMode(StepperData::MS2, OUTPUT);
  pinMode(StepperData::MS3, OUTPUT);
  digitalWrite(StepperData::MS1, HIGH);
  digitalWrite(StepperData::MS2, HIGH);
  digitalWrite(StepperData::MS3, HIGH);
}

void setup() {
  Serial.begin(9600);

  stepperMode();
  stepper.setMaxSpeed(calculations::stepsToMovePerInterval);
  stepper.setSpeed(calculations::stepsToMovePerInterval); // constant-speed tracking, no ramp needed

  Serial.println("Stepper initialized.");
}

void loop() {
  stepper.runSpeed();
}