const int controlPin1 = 2;
const int controlPin2 = 3;
const int enablePin = 9;
const int directionSwitchPin = 4;
const int onOffSwitchStatePin = 5;
const int potPin = A0;

int onOffSwitchState = 0;
int previousOnOffSwitchState = 0;
int directionSwitchState = 0;
int previousDirectionSwitchState = 0;


int motorEnabled = 0;
int motorSpeed = 0;
int motorDirection = 1;


void setup()
{
	pinMode(directionSwitchPin, INPUT);
	pinMode(onOffSwitchStatePin, INPUT);
	pinMode(controlPin1, OUTPUT);
	pinMode(controlPin2, OUTPUT);
	pinMode(enablePin, OUTPUT);

	digitalWrite(enablePin, LOW); //So the motor is stopped when the program starts
}


void loop()
{
	//We read the state of the two switches
	onOffSwitchState = digitalRead(onOffSwitchStatePin);
	delay(1);

	directionSwitchState = digitalRead(directionSwitchPin);

	//We read the value of the potontiometer and divide it by 4
	//to provide the motor speed
	motorSpeed = analogRead(potPin) / 4;

	//If the on/off switch is pressed
	//We, then change the status of the motor
	if (onOffSwitchState != previousOnOffSwitchState)
		if (onOffSwitchState == HIGH)
			motorEnabled = !motorEnabled;

	//If the directio switch is pressed
	//We, then change the direction of the motor
	if (directionSwitchState != previousDirectionSwitchState)
		if (directionSwitchState == HIGH)
			motorDirection = !motorDirection;

	//If it is in the first direction
	//Pin 2 of the H-bridge will be HIGH
	//and pin 3 of the H-bridge will be LOW
	//And vice-versa
	if (motorDirection == 1) {
		digitalWrite(controlPin1, HIGH);
		digitalWrite(controlPin2, LOW);
	}
	else {
		digitalWrite(controlPin2, HIGH);
		digitalWrite(controlPin1, LOW);	
	}

	//We check the status of the motor
	//If it is enabled we then turn the motor
	//else, we don't
	if (motorEnabled == 1)
		analogWrite(enablePin, motorSpeed);
	else
		analogWrite(enablePin, 0);

	//We save the state of the switches in the previous variable 
	//so we can test in the next run through
	previousDirectionSwitchState = directionSwitchState;
	previousOnOffSwitchState = onOffSwitchState;
}