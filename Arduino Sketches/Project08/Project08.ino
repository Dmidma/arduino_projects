const int switchPin = 8;

unsigned long previousTime = 0;

int switchState = 0;
int previousSwitchState = 0;

int led = 2;

long interval = 3000; // Interval of 3 seconds but you can increase or decrease it.

void setup()
{
	for (int x = 2; x < 8; x++)
		pinMode(x, OUTPUT);

	pinMode(switchPin, INPUT);
    
}

void loop()
{
	unsigned long currentTime = millis();

	int threeSec;

	if (currentTime - previousTime >= interval)
	{
		previousTime = currentTime;
		digitalWrite(led, HIGH);
		led++;

		// If it's the seventh led we blink all the led for three seconds
		//Then we start all over again 
		if (led == 7)
		{	
			delay(500);

			threeSec = 0;
			while (threeSec != 3000)
			{
				for (int x = 2; x < 8; x++)
					digitalWrite(x, LOW);

				delay(500); //delay for a second

				for (int x = 2; x < 8; x++)
					digitalWrite(x, HIGH);

				delay(500);
				
				threeSec += 1000;
			}

			for (int x = 2; x < 8; x++)
					digitalWrite(x, LOW);
			led = 2;
		}
	}

	//If the switchState changes, we then repeat every thing from the start
	switchState = digitalRead(switchPin);
    
	if (switchState != previousSwitchState)
	{
		for (int x = 2; x < 8; x++)
			digitalWrite(x, LOW);

		led = 2;
		previousTime = currentTime;
	}
	previousSwitchState = switchState;
}
