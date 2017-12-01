#include <LiquidCrystal.h> //Include the LiquidCrystal library

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Initialize the lcd


const int switchPin = 6;
int switchState = 0;
int prevSwitchState = 0;
int reply;

void setup()
{
	
    lcd.begin(16, 2); //Size of the screen 16 cols, 2 rows

	pinMode(switchPin, INPUT);


	//The first thing that will appear on the LCD is "Ask the Crystall Ball"
	lcd.print("Ask the");
	lcd.setCursor(0, 1);
	lcd.print("Crystall Ball!");
}


void loop()
{
	//Reading the switch State
	switchState = digitalRead(switchPin);
        

    //If the switch state is changed then we produce a random number
    //and with that number we print a different msg each time in the LCD
	if (switchState != prevSwitchState)
		if (switchState == LOW)
		{
			reply = random(8);
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("The ball says:");
			lcd.setCursor(0, 1);

			switch (reply)
			{
				case 0:
					lcd.print("Yes");
				break;
				
				case 1:
					lcd.print("Most likely");
				break;
				
				case 2:
					lcd.print("Certainly");
				break;
				
				case 3:
					lcd.print("Outlook good");
				break;
				
				case 4:
					lcd.print("Unsure");
				break;
				
				case 5:
					lcd.print("Ask again");
				break;
				
				case 6:
					lcd.print("Doubtful");
				break;
				
				case 7:
					lcd.print("No");
				break;
			}
		}

	//Saving the swtich state to test it in the next run through
	prevSwitchState = switchState;

}
