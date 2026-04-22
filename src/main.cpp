#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define NUM_STEPPERS 6
#define STEPTOANGLE 5.55555 // 200 steps, 10x1 reduction -> 2000 effective steps; 5.5555 effective steps per degree

const int stepPins[NUM_STEPPERS] = {11, 5, -1, -1, -1, -1}; // FILL ME IN !!!!!
const int dirPins[NUM_STEPPERS]  = {13, 7, -1, -1, -1, -1}; // FILL ME IN !!!!!

AccelStepper steppersList[NUM_STEPPERS] = {
  AccelStepper(AccelStepper::DRIVER, stepPins[0], dirPins[0]),
  AccelStepper(AccelStepper::DRIVER, stepPins[1], dirPins[1]),
  AccelStepper(AccelStepper::DRIVER, stepPins[2], dirPins[2]),
  AccelStepper(AccelStepper::DRIVER, stepPins[3], dirPins[3]),
  AccelStepper(AccelStepper::DRIVER, stepPins[4], dirPins[4]),
  AccelStepper(AccelStepper::DRIVER, stepPins[5], dirPins[5])
};

MultiStepper multi;

long positions[NUM_STEPPERS];
void anglesToSteps(float angles[], long positions[]) {
  for (int i = 0; i < NUM_STEPPERS; i++) {
    positions[i] = angles[i] * STEPTOANGLE;
	// positions[i] = steppersList[i].currentPosition() + angles[i] * STEPTOANGLE; // Relative Motion ---
  }
}

// uint32_t startTime;
// uint32_t curTime;
// uint8_t stopped = 0;


void setup() {
	Serial.begin(115200);

	for (int i = 0; i < NUM_STEPPERS; i++) {
		steppersList[i].setMaxSpeed(500);
		multi.addStepper(steppersList[i]);
	}

	// startTime = millis();
}

void loop() {

	// Possibly jerkey - No acc/dec
	float targetAngles[NUM_STEPPERS] = {0, 0, 0, 0, 0, 0};
	anglesToSteps(targetAngles, positions);

	multi.moveTo(positions);
	multi.runSpeedToPosition();

	delay(1000);

	float targetAngles2[NUM_STEPPERS] = {30, -45, 60, -30, 15, 90};
	anglesToSteps(targetAngles2, positions);

	multi.moveTo(positions);
	multi.runSpeedToPosition();

	delay(1000);

	// // manual synchronization, but smoother movement
	// for (int i = 0; i < NUM_STEPPERS; i++) {
	// 	steppersList[i].moveTo(positions[i]);
	// }

	// bool running = true;
	// while (running) {
	// 	running = false;
	// 	for (int i = 0; i < NUM_STEPPERS; i++) {
	// 		if (steppersList[i].distanceToGo() != 0) {
	// 			steppersList[i].run();
	// 			running = true;
	// 		}
	// 	}
	// }
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