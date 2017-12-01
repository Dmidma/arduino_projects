#include <CapacitiveSensor.h> //Include the third party library

//Initialize the object with the pin 4 as sender and pin 2 as receiver
CapacitiveSensor capSensor = CapacitiveSensor(4, 2); 

//Check the right value that will suit you
//Through the serial monitor
int threshold = 500;

const int ledPin = 12;


void setup()
{
	Serial.begin(9600);
	pinMode(ledPin, OUTPUT);
}

void loop()
{	
	//Read how much time it will take to send 30 samples
	//Then print it on the monitor
	long sensorValue = capSensor.capacitiveSensor(30);
	Serial.println(sensorValue);

	//If the value is greater than the threshold 
	//we then light on the LED
	//Else it is off.
	if (sensorValue > threshold)
		digitalWrite(ledPin, HIGH);
	else
		digitalWrite(ledPin, LOW);


	delay(10);
}
