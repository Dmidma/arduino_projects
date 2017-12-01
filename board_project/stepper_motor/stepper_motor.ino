#include <Stepper.h>

const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 9, 10, 11, 12);

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  myStepper.setSpeed(50);
  
  Serial.begin(9600);
}

void loop() {
  
  myStepper.step(5);

  myStepper.step(-199);

  myStepper.step(+1);

  myStepper.step(-50);

  myStepper.step(+66);

  myStepper.step(-25);

  myStepper.step(+75);

  myStepper.step(-150);

  myStepper.step(+175);

  myStepper.step(-25);

  myStepper.step(+100);
  
  
}
