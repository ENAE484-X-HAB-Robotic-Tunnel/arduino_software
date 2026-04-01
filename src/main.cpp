#include <Arduino.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
/*
	1.8 deg step angle
*/
// put function declarations here:
#define PUL 11
#define DIR 13

AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR);
unsigned long startTime;
unsigned long curTime;
int stopped = 0;

void serialParse(int&);


void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);

	stepper.setMaxSpeed(5000);
	stepper.setAcceleration(20);
	stepper.setSpeed(2000);
	startTime = millis();
}



void loop() {
	// put your main code here, to run repeatedly:
	int step_count;
	if (Serial.available() > 0){
		serialParse(step_count);
		Serial.println(step_count);
	}

	curTime = millis();

	if ((curTime - startTime) <= 10000){
		stepper.runSpeed();
	}
	else if (stopped == 0){
		stepper.stop();
		Serial.println("Stopped");
		stopped = 1;
	}
}

// put function definitions here:
void serialParse(int &step_count) {
	step_count = Serial.read();
}


// void setup()
// {  
//    stepper.setMaxSpeed(1000);
//    stepper.setSpeed(50);	
// }

// void loop()
// {  
//    stepper.runSpeed();
// }