#include <Servo.h> //including the Servo library

Servo myServo; //Initializing the Servo object


const int piezo = A0;
const int switchPin = 2;
const int yellowLed = 3;
const int greenLed = 4;
const int redLed = 5;

int knockVal;
int switchVal;

const int quietKnock = 10;
const int loudKnock = 100;

boolean locked = false;
int numberOfKnocks = 0;


void setup()
{
	myServo.attach(9); //attach the servo to its pin

	pinMode(yellowLed, OUTPUT);
	pinMode(redLed, OUTPUT);
	pinMode(greenLed, OUTPUT);
	pinMode(switchPin, INPUT);

	//We start with an open lock <=> green Led light on
	digitalWrite(greenLed, HIGH);
	myServo.write(0);
}

void loop()
{	
	//If the lock is open
	if (!locked) {
		//We read from the switch
		switchVal = digitalRead(switchPin);

		//If it is clicked we then turn the green LED off and the red one on
		//We change the value of locked to true
		//and we close the lock by turning the servo to the right angle
		//We put a delay so the servo have time to turn
		if (switchVal == HIGH) {
			locked = true;
			digitalWrite(greenLed, LOW);
			digitalWrite(redLed, HIGH);
			myServo.write(90);
			delay(1000);
		}
	} else {
		//If the lock is closed
		//we read from the piezo
		knockVal = analogRead(piezo);

		//We test if the knock have the suited force
		//if it is the case we increment numberOfKnocks
		if (numberOfKnocks < 3 && knockVal > 0)
			if (checkForKnock(knockVal))
				numberOfKnocks++;
	}
	
	//We test if the numberOfKnocks is equal to 3
	//if it is the case, we unlock the lock by turning the servo to the initiale angle
	//We turn the green LED on, and the red one off
	//And we put numberOfKnocks to zero
	if (numberOfKnocks == 3) {
		locked = false;
		numberOfKnocks = 0;
		myServo.write(0);
		delay(20);
		digitalWrite(greenLed, HIGH);
		digitalWrite(redLed, LOW);
	}
}

//This function will test the value given from the piezo
//If it have the suited force it will turn the yellow LED for a brief moment
//and return true
//else it will return false
boolean checkForKnock(int value)
{
	if (value > quietKnock && value < loudKnock) {
		digitalWrite(yellowLed, HIGH);
		delay(50);
		digitalWrite(yellowLed, LOW);
		return true;
	}
	else {
		return false;
	}
}
