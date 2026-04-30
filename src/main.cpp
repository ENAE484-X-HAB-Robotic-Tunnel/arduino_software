#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>

#define numLegs 6
const float angleToSteps = 11.1111; // 200 steps, 20x1 reduction -> 4000 effective steps; 11.11 effective steps per degree
const float lengthToAngle = 360*(1000.0/31.1); // 31.1mm spool circumference
const float stepsPerMeter = angleToSteps*lengthToAngle;

enum Mode { CONTROL, MANUAL };
Mode mode = CONTROL;
// Mode mode = MANUAL;

const bool disableMovement = false;

// STEP = PUL, DIR = DIR
const int stepPins[numLegs] = {10, 13, 7, 38, 52, 4};
const int dirPins[numLegs]  = {9,  12, 6, 34, 50, 3};

AccelStepper steppersList[numLegs] = {
  AccelStepper(AccelStepper::DRIVER, stepPins[0], dirPins[0]),
  AccelStepper(AccelStepper::DRIVER, stepPins[1], dirPins[1]),
  AccelStepper(AccelStepper::DRIVER, stepPins[2], dirPins[2]),
  AccelStepper(AccelStepper::DRIVER, stepPins[3], dirPins[3]),
  AccelStepper(AccelStepper::DRIVER, stepPins[4], dirPins[4]),
  AccelStepper(AccelStepper::DRIVER, stepPins[5], dirPins[5]),
};

MultiStepper multi;

float l_legs[numLegs];
float l_init[numLegs] = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5}; // Calibrate zero position
float l_input[numLegs];
long positions[numLegs];

String inputBuffer = "";
void parseInput(String msg);

void setup() {
	Serial.begin(115200);

    for (int i = 0; i < numLegs; i++) {
        l_legs[i] = l_init[i];
    }

	for (int i = 0; i < numLegs; i++) {
		// steppersList[i].setMaxSpeed(4000);
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
    // Strip leading garbage bytes
    int start = 0;
    while (start < msg.length() && !isDigit(msg[start]) && msg[start] != '-') {
        start++;
    }
    msg = msg.substring(start);
    
    char buffer[100];
    msg.toCharArray(buffer, sizeof(buffer));

    int index = 0;
    char *token = strtok(buffer, ",");

    while (token != NULL && index < numLegs) {
        l_input[index] = atof(token);
        token = strtok(NULL, ",");
        index++;
    }

    if (index != numLegs) {
        Serial.println("Parse error");
        return;
    }

    float delta[numLegs];
    for (int i = 0; i < numLegs; i++) {
        if (mode == CONTROL) {
            delta[i] = l_input[i] - l_init[i];
            l_legs[i] = l_input[i];
        }
        else { // Manual mode - relative motion
            delta[i] = l_legs[i] - l_init[i] + l_input[i];
            l_legs[i] += l_input[i];
        }
        positions[i] = long(delta[i] * stepsPerMeter);
    }
	
    Serial.print("(Arduino) Leg Lengths: ");
	for (int i = 0; i < numLegs; i++) {
		Serial.print(l_legs[i], 4);
		if (i < numLegs - 1) Serial.print(", ");
	}
	Serial.println();

    if (!disableMovement) {
        multi.moveTo(positions);
        multi.runSpeedToPosition();
    }

    Serial.println("(Arduino) Done");
}

void loop() {
    readSerial();
}