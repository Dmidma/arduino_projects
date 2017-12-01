int sensorValue;

int sensorLow = 1023;
int sensorHigh = 0;

const int ledPin = 13;


void setup()
{
	//We light up the LED until we finish the calibration
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);


	//Calibration of the sensorHigh and sensorLow
	while (millis() < 5000)
	{
		sensorValue = analogRead(A0);
		if (sensorValue > sensorHigh)
		{
			sensorHigh = sensorValue;
		}
		if (sensorValue < sensorLow)
		{
			sensorLow = sensorValue;
		}
	}

	//We light off the LED after the calibration
	digitalWrite(ledPin, LOW);
}


void loop()
{	
	//We read sensorValue
	sensorValue = analogRead(A0);

	//We map it
	int pitch = map(sensorValue, sensorLow, sensorHigh, 50, 4000);

	//We make noise
	tone(8, pitch, 20);

	//We give a little a bit of time
	delay(10);
}