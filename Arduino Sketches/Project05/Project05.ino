#include <Servo.h>

//Declaring the Servo (object)
Servo myServo;


int const potPin = A0;
int potVal;
int angle;



void setup()
{
	myServo.attach(9); //Attaching the servo the right pin
	Serial.begin(9600);
}

void loop()
{
	
	potVal = analogRead(potPin); //Reading from the potentiometer
	angle = map(potVal, 0, 1023, 0, 179); //Mapping the sensor value

	//Printing the potVal and angle on the monitor
	Serial.print("potVal: ");
	Serial.print(potVal);
	Serial.print("angle: ");
	Serial.print(angle);

	myServo.write(angle);

	//A little delay so the servo has time to move to its new position
	delay(15);
}