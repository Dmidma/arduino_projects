const int sensorPin = A0;
const float baselineTemp = 27.0; //The default temeprature that will be compared to


void setup()
{
	Serial.begin(9600);

	for (int pinNumber = 2; pinNumber < 5; pinNumber++) //Configuring the pins 2, 3, 4 to OUTPUT mode
	{
		pinMode(pinNumber, OUTPUT);
		digitalWrite(pinNumber, LOW);
	}
}

void loop()
{
	int sensorVal = analogRead(sensorPin); // Reading the sensor value

	Serial.print("Sensor value: ");
	Serial.print(sensorVal);

	float voltage = (sensorVal / 1024.0) * 5.0; // converting it to Voltage

	Serial.print(", Volts: ");
	Serial.print(voltage);

	Serial.print(", degrees C: ");

	float temperature = (voltage - .5) * 100;  //converting it to degress C

	Serial.print(temperature);

	// Comparing then lighting the LEDs
	
	if (temperature < baselineTemp)
	{
		digitalWrite(2, LOW);
		digitalWrite(3, LOW);
		digitalWrite(4, LOW);
	}
	else if (temperature >= baselineTemp + 1 && temperature < baselineTemp + 2)
	{
		digitalWrite(2, HIGH);
		digitalWrite(3, LOW);
		digitalWrite(4, LOW);
	}
	else if (temperature >= baselineTemp + 2 && temperature < baselineTemp + 3)
	{
		digitalWrite(2, HIGH);
		digitalWrite(3, HIGH);
		digitalWrite(4, LOW);
	}
	else if (temperature >= baselineTemp + 4)
	{
		digitalWrite(2, HIGH);
		digitalWrite(3, HIGH);
		digitalWrite(4, HIGH);
	}

	delay(1);
}
