const int switchPin = 2;
const int motorPin = 9;
int switchState = 0;



//Setting up the motorPin to OUTPUT and the switchPin to INPUT
void setup()
{
	pinMode(motorPin, OUTPUT);
	pinMode(switchPin, INPUT);	
}



//Reading from the switch, then testing, and finally making the motor spin
void loop()
{
	switchState = digitalRead(switchPin);

	if (switchState == HIGH)
		digitalWrite(motorPin, HIGH);
	else
		digitalWrite(motorPin, LOW);
}