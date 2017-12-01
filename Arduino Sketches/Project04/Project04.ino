const int greenLEDPin = 9;
const int redLEDPin = 11;
const int blueLEDPin = 10;

const int redSensorPin = A0;
const int greenSensorPin = A1;
const int blueSensorPin = A2;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int redSensorValue = 0;
int greenSensorValue = 0;
int blueSensorValue = 0;

void setup()
{
	Serial.begin(9600);

	pinMode(greenLEDPin, OUTPUT);
	pinMode(redLEDPin, OUTPUT);
	pinMode(blueLEDPin, OUTPUT);
}

void loop()
{
	
	//Reading from the photoresistors input

	redSensorValue = analogRead(redSensorPin);
	delay(5);
	greenSensorValue = analogRead(greenSensorPin);
	delay(5);
	blueSensorValue = analogRead(blueSensorPin);
	delay(5);


	//Dividing the sensor reading (0-1023) by 4 so we can get (0-255)

	redValue = redSensorValue / 4;
	greenValue = greenSensorValue / 4;
	blueValue = blueSensorValue / 4;

	//Printing the raw and mapped sensor values on the monitor

	Serial.print("Raw Sensor Values:\nRed ");
	Serial.println(redSensorValue);
	Serial.print("Green ");
	Serial.println(greenSensorValue);
	Serial.print("Blue ");
	Serial.println(blueSensorValue);
	Serial.print("Mapped Sensor Values:\nRed ");
	Serial.println(redValue);
	Serial.print("Green ");
	Serial.println(greenValue);
	Serial.print("Blue ");
	Serial.println(blueValue);

	//Configurate the RGB LED with the right values

	analogWrite(redLEDPin, redValue);
	analogWrite(greenLEDPin, greenValue);
	analogWrite(blueLEDPin, blueValue);
}