#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define numLegs 6
#define numSteppers numLegs
#define angleToSteps 11.1111 // 200 steps, 20x1 reduction -> 4000 effective steps; 11.11 effective steps per degree
#define lengthToAngle 360*(1000/31.1) // 31.1mm spool circumference
#define stepsPerMeter angleToSteps*lengthToAngle
// NOTES: RADIUS?, 

enum Mode { CONTROL, CALIBRATION };
Mode mode = CALIBRATION;

// STEP = PUL, DIR = DIR
const int stepPins[numSteppers] = {10, 13, 7, 38, 52, 4};
const int dirPins[numSteppers]  = {9,  12, 6, 34, 50, 3};

AccelStepper steppersList[numSteppers] = {
  AccelStepper(AccelStepper::DRIVER, stepPins[0], dirPins[0]),
  AccelStepper(AccelStepper::DRIVER, stepPins[1], dirPins[1]),
  AccelStepper(AccelStepper::DRIVER, stepPins[2], dirPins[2]),
  AccelStepper(AccelStepper::DRIVER, stepPins[3], dirPins[3]),
  AccelStepper(AccelStepper::DRIVER, stepPins[4], dirPins[4]),
  AccelStepper(AccelStepper::DRIVER, stepPins[5], dirPins[5]),
};

MultiStepper multi;

double l_legs[numSteppers];
float l_legs_init[numLegs] = {0.45, 0.45, 0.45, 0.45 ,0.45 ,0.45}; // Calibrate zero position
long positions[numSteppers];

String inputBuffer = "";
void parseInput(String msg);

void setup() {
	Serial.begin(115200);

	for (int i = 0; i < numSteppers; i++) {
		steppersList[i].setMaxSpeed(500);
		multi.addStepper(steppersList[i]);
	}
}

void readSerial() {
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            parseInput(inputBuffer);
            inputBuffer = "";
        }
        else if (c != '\r') {
            inputBuffer += c;
        }
    }
}

void parseInput(String msg) {

    char buffer[100];
    msg.toCharArray(buffer, sizeof(buffer));

    int index = 0;
    char *token = strtok(buffer, ",");

    while (token != NULL && index < numSteppers) {
        l_legs[index] = atof(token);
        token = strtok(NULL, ",");
        index++;
    }

    if (index != numSteppers) {
        Serial.println("Parse error");
        return;
    }

    for (int i = 0; i < numSteppers; i++) {
        float delta = l_legs[i] - l_legs_init[i];

        if (mode == CONTROL) positions[i] = (long) (delta * stepsPerMeter);
        // else positions[i] = (l_legs[i]) * stepsPerMeter;
        else positions[i] = (long) (delta * stepsPerMeter);
        Serial.println(positions[i], 4);

    }
	
	Serial.print("(Arduino) Delta Lengths: ");
	for (int i = 0; i < numSteppers; i++) {
		Serial.print(l_legs[i] - l_legs_init[i], 4);
		if (i < numSteppers - 1) Serial.print(", ");
	}
	Serial.println();

	Serial.print("(Arduino) Steps       :");
	for (int i = 0; i < numSteppers; i++) {
		Serial.print(positions[i], 4);
		if (i < numSteppers - 1) Serial.print(", ");
	}
	Serial.println();

    // multi.moveTo(positions);
    // multi.runSpeedToPosition();

    // Serial.println("(Arduino) Done");
    // Serial.println(delta);
}

void loop() {
    if (mode == CONTROL) readSerial();
    else {
        while (Serial.available()) {
            char c = Serial.read();

            if (c == '\n') {
            parseInput(inputBuffer);
            inputBuffer = "";
            }
            else if (c != '\r') {
            inputBuffer += c;
            }
        }
    }
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