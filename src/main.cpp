#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define PUL1 11
#define DIR1c 13

#define PUL2 5
#define DIR2 7

// 200 steps, 10x1 reduction -> 2000 effective steps
// 5.5555 effective steps per degree

#define STEPTOANGLE 5.55555

AccelStepper stepper1(AccelStepper::DRIVER, PUL1, DIR2);
AccelStepper stepper2(AccelStepper::DRIVER, PUL2, DIR2);

MultiStepper steppers;

uint32_t startTime;
uint32_t curTime;
uint8_t stopped = 0;

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);

	stepper1.setMaxSpeed(500);
	stepper2.setMaxSpeed(500);

	steppers.addStepper(stepper1);
	steppers.addStepper(stepper2);

	startTime = millis();
}



void loop() {
	// put your main code here, to run repeatedly:
	// if (Serial.available() > 0){
	// 	serialParse(step_count);
	// 	Serial.println(step_count);
	// }

	long positions[2];
	positions[0] = STEPTOANGLE * 360; // gives two total rotations
	positions[1] = STEPTOANGLE * 720; // gives 4 total rotations

	steppers.moveTo(positions);
	steppers.runSpeedToPosition();

	delay(1000);
	positions[0] = -STEPTOANGLE * 360;
	positions[1] = -STEPTOANGLE * 720;

	steppers.moveTo(positions);
	steppers.runSpeedToPosition();

	delay(1000);
}

// put function definitions here:
// void serialParse(int &step_count) {
// 	step_count = Serial.read();
// }


// void setup()
// {  
//    stepper.setMaxSpeed(1000);
//    stepper.setSpeed(50);	
// }

// void loop()
// {  
//    stepper.runSpeed();
// }



/*

Multiple steppers

#include <AccelStepper.h>
#include <MultiStepper.h>

AccelStepper stepper1(AccelStepper::FULL4WIRE, 2, 3, 4, 5);
AccelStepper stepper2(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;

void setup() {
  Serial.begin(9600);

  // Configure each stepper
  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);

  // Then give them to MultiStepper to manage
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
}

void loop() {
  long positions[2]; // Array of desired stepper positions
  
  positions[0] = 1000;
  positions[1] = 50;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
  
  // Move to a different coordinate
  positions[0] = -100;
  positions[1] = 100;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
}
*/