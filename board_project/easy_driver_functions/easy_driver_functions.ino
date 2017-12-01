// Easy-Driver Hookup Pins
#define stp 3
#define dir 2
#define MS1 6
#define MS2 7
#define EN 6


void setup() 
{

	// Set all pins to OUTPUT
	pinMode(stp, OUTPUT);
	pinMode(dir, OUTPUT);
	pinMode(MS1, OUTPUT);
	pinMode(MS2, OUTPUT);
	pinMode(EN, OUTPUT);

  
  
	// step_forward();
}



void loop()
{

  

  step_dir(200);
  step_dir(-120);
  step_dir(60);
  step_dir(-25);
  step_dir(145);
 

}

void step_dir(int steps)
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


void step_forward()
{

  // Enable Driver
  digitalWrite(EN, LOW);

  // set DIR to HIGH
  digitalWrite(dir, HIGH);

   
  // Enable full step
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);

	for (int i = 0; i < 1600; i++)
	{
		digitalWrite(stp, HIGH);
		delay(1);
		digitalWrite(stp, LOW);
		delay(1);
	}

}


void disable_driver()
{
	
	// Reset STEP and DIR pins
	digitalWrite(stp, LOW);
	digitalWrite(dir, LOW);

	// Reset to Full Step (2 phases)
	digitalWrite(MS1, LOW);
	digitalWrite(MS2, LOW);

	// ENABLE: HIGH -> Disable FET functionalities
	digitalWrite(EN, HIGH);
}
