#include <Thread.h>
#include <StaticThreadController.h>
#include <ThreadController.h>



#include <Stepper.h>

// Easy-Driver Hookup Pins


#define stp 3
#define dir 2
#define MS1 6
#define MS2 7


const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 9, 10, 11, 12);


// Thread section

ThreadController controller = ThreadController();

// declare two thread
Thread* firstMotor = new Thread();
Thread* secondMotor = new Thread();






void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

    // Set all pins to OUTPUT
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);


  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);

  
  myStepper.setSpeed(60);

  // Enable 1/8 step -> 1600: Revolution
  // digitalWrite(MS1, HIGH);
  // digitalWrite(MS2, HIGH);


  firstMotor->onRun(firstMotorMovement);
  firstMotor->setInterval(1000);
  secondMotor->onRun(secondMotorMovement);
  secondMotor->setInterval(1000);

  controller.add(firstMotor);
  controller.add(secondMotor);
  
  
  Serial.begin(9600);
}

void loop() {
  /*
 myStepper.step(5);

  step_forward(5);
  
  myStepper.step(-199);

  step_forward(-199);
  
  myStepper.step(+1);

  step_forward(1);
 
  myStepper.step(-50);

  step_forward(-50);

  myStepper.step(+66);

  step_forward(66);

  myStepper.step(-25);

  step_forward(-25);

  myStepper.step(+75);

  step_forward(75);
  
  myStepper.step(-150);

  step_forward(-150);

  myStepper.step(+175);

  step_forward(175);

  myStepper.step(-25);

  step_forward(-25);

  myStepper.step(+100);

  step_forward(100); 
  */
  
  
  /*
  firstMotor->run();

  secondMotorMovement();
  */

  /*
  moveMotors(00);
  delay(100);
  */

  /*
  moveMotors(500);

  moveMotors(-250);

  moveMotors(100);

  moveMotors(-350);
  */


   
  moveMotors2(500, 20);

  moveMotors2(-250, 20);

  moveMotors2(100, 20);

  moveMotors2(-350, 20);
  


  
   /*
  myStepper.step(800);
  
  
  step_forward(800);

  myStepper.step(-800);

  step_forward(-800);

  delay(5000);
 
  moveMotors(600);
  moveMotors(-600);

  delay(5000);
  */
  
  /*
  moveMotors(52);
  moveMotors(124);
  moveMotors(-12);
  moveMotors(50);
  moveMotors(-80);

  moveMotors(120);

  moveMotors(52);
  moveMotors(-53);
  moveMotors(14);


  moveMotors(124);
  moveMotors(-50);
  moveMotors(12);
  moveMotors(-180);

  moveMotors(58);

  moveMotors(-10);
  moveMotors(18);
  moveMotors(-100);
  */
  
  
  
}


void moveMotors(int steps) {

  int oneStep = 1;

  if (steps < 0) {
    oneStep = -1;
    steps = steps * -1;  
  }
    

  
  
  for (int i = 0; i < steps; i++)
  {
       myStepper.step(oneStep);
       step_forward(oneStep);
  }
  
}


void moveMotors2(int steps, int delay_time) {

  int oneStep = 1;

  if (steps < 0) {
    oneStep = -1;
    steps = steps * -1;  
  }
    

  
  
  for (int i = 0; i < steps; i++)
  {
       myStepper.step(oneStep);
       step_forward(oneStep);
       delay(delay_time);
  }
  
}



void firstMotorMovement()
{
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


void secondMotorMovement()
{
  step_forward(5);
  step_forward(-199);
  step_forward(1);
  step_forward(-50);
  step_forward(66);
  step_forward(-25);
  step_forward(75);
  step_forward(-150);
  step_forward(175);
  step_forward(-25);
  step_forward(100);
}

/**
 * I need to map from 200 to 1600.
 * If the steps is negative, then change the direction.
 */
void step_forward(int steps)
{
  if (steps < 0)
  {
    digitalWrite(dir, LOW);
    // change the step to a postive value
    steps = steps * -1;
  }
  else
    digitalWrite(dir, HIGH);

  // 1600 is a full revolution
  // multiply the steps by 8
  // steps = steps * 8;

  for (int i = 0; i < steps; i++)
  {
    digitalWrite(stp, HIGH);
    delay(2);
    digitalWrite(stp, LOW);
    delay(2);
  }
}

