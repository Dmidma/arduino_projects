/**
 *
 * Alarm_Clock.ino
 *
 * Oussema Hidri
 * d.oussema.d@gmail.com
 *
 * This sketch is for an alarm clock project, which uses an LCD to print time
 * and date.
 * The alarm clock can be used like a reminder, or simply an alarm clock 
 * that will deactivate only with a secret knock pattern.
 */

#include <LiquidCrystal.h>
#include <avr/pgmspace.h>

#include "pitches.h"
#include "tetris.h"

// initialize the LCD
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

// defining the two buttons, one for switching, the other for action
const char chSwitchPin = 6;
const char acSwitchPin = 7;

// An array that contains
// seconds, minutes, hours, day, month, year;
short dateTimeTab[] = {00, 29, 5, 26, 04, 2012}; 
char xCor[] = {11, 8, 5, 4, 7, 12};
char yCor[] = {1, 1, 1, 0, 0, 0};

// Number of days in each month
char monthDay[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Parameters for the reminder
unsigned int reminder = 0;
unsigned char houRem = 0;
unsigned char minRem = 0;

// Parameters for the Alarm
int houAlarm = 0;
int minAlarm = -1; 

// Parameters for the secret knock code
const int maxRange = 20;
int secret[maxRange] = {0};
char secretIndex;
int code[maxRange] = {0};

// Parameters to calculate the gaps
unsigned long now = 0;
unsigned long startTime = 0;
unsigned long piezoInput = 0;

struct Mode
{
   char xLCD;
   char yLCD;
   char cLCD;
   boolean actLCD;
};

struct Mode tabModes[3];
char currentMode = -1;


unsigned long firstPoint;
unsigned long secondPoint;

void setup()
{	
	
	pinMode(chSwitchPin, INPUT);
	pinMode(acSwitchPin, INPUT);

		
	initMode(&tabModes[0], 0, 0, 'C', false);
	initMode(&tabModes[1], 15, 0, 'R', false);
	initMode(&tabModes[2], 15, 1, 'A', false);

	lcd.begin(16, 2);

	// Making sure that Feb have the right amount of days
	monthDay[1] =  ((dateTimeTab[5] % 4 == 0 && 
					 dateTimeTab[5] % 100 != 0) || 
					 dateTimeTab[5]%400==0) ? 29 : 28;
}

void loop()
{	
	

	// The first block is for switching modes
	// If The mode is not activated the we delete it from the LCD
	// The initial point is (0, 0) and no mode is printed
	// The first mode is C -> changing the time and date
	// The second mode is R -> Setting a reminder
	// The third mode is A -> Setting an alarm 
	if (digitalRead(chSwitchPin))
	{
		// deleting the previous mode from the LCD
		if (currentMode != -1 && !tabModes[currentMode].actLCD)
			deleteCharLCD(tabModes[currentMode].xLCD, 
				tabModes[currentMode].yLCD);

		// change to the next mode
		currentMode++;

		// return to initial point
		if (currentMode == 3)
			currentMode = -1;

		// print the current mode into the LCD
		if (currentMode != -1 && !tabModes[currentMode].actLCD)
			printCharLCD(tabModes[currentMode].xLCD, 
				tabModes[currentMode].yLCD, 
				tabModes[currentMode].cLCD);	
	}

	// test if the activate button is pressed and we are in one of the three modes
	if (currentMode != -1 && digitalRead(acSwitchPin))
	{	

		// activate the current mode
		tabModes[currentMode].actLCD = !tabModes[currentMode].actLCD;
		
		switch (currentMode)
		{
			// first mode: C->changing the time and date
			case 0:
			{
				// iterating over the array of date and time
				int currentIndex = 6;
				while (tabModes[0].actLCD)
				{
					currentIndex--;

					// still not the end of the array
					if (currentIndex != -1)
					{
						printTimeDate();
						printCharLCD(xCor[currentIndex], yCor[currentIndex], '_');
						delay(1000);
					}

					while (!digitalRead(acSwitchPin) && currentIndex != -1)
					{
						if (digitalRead(chSwitchPin))
						{
							switch (currentIndex)
							{
								case 4:
									rightValue(currentIndex, 13, 1);
								break;
								case 3:
									// cheking the correctness of February
									monthDay[1] =  ((dateTimeTab[5] % 4 == 0 && 
					 								 dateTimeTab[5] % 100 != 0) || 
					 								 dateTimeTab[5]%400==0) ? 29 : 28;
									rightValue(currentIndex, monthDay[dateTimeTab[4] - 1] + 1, 1);
								break;
								case 2:
									rightValue(currentIndex, 24, 0);
								break;
								case 1:
									rightValue(currentIndex, 60, 0);
								break;
								case 0:
									rightValue(currentIndex, 60, 0);
			  					break; 
								default:
									dateTimeTab[currentIndex]++;	
									delay(500);
									printTimeDate();
							}
						}
					}

					// reaching the end of the array
					if (currentIndex == -1)
					{	
						// deactivate the C mode
						tabModes[0].actLCD = false;
					
						// delete the indicator of the mode from LCD
						deleteCharLCD(tabModes[0].xLCD, tabModes[0].yLCD);

						// return the initial mode
						currentMode = -1;
					}
				}
			}
			break;

			// second mode: R->setting a reminder
			case 1:
			{
				// Setting the reminder on
				if (reminder == 0)
				{	
					// first point (used to calculate time spent in here)
					firstPoint = millis();

					delay(1500);
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("Reminder for :");
					lcd.setCursor(0, 1);
					
					if (reminder / 60 < 10)
						lcd.print('0');
					lcd.print(reminder / 60);
					lcd.print("hours & ");
					lcd.setCursor(10, 1);
					if (reminder %60 < 10)
						lcd.print('0');
					lcd.print(reminder % 60);
					lcd.print("mins");
					while (!digitalRead(acSwitchPin))
					{
						if (digitalRead(chSwitchPin))
						{
							reminder += 15;
							lcd.setCursor(0, 1);
							if (reminder / 60 < 10)
								lcd.print('0');
							lcd.print(reminder / 60);
							lcd.setCursor(10, 1);
							lcd.print(reminder % 60);
							if (reminder % 60 == 0)
								lcd.print(0);
							delay(500);
						}
					}
					minRem += (reminder % 60) + dateTimeTab[1];
					if (minRem >= 60)
					{
						minRem -= 60;
						houRem++;
					}
					houRem += (reminder / 60) + dateTimeTab[2];
					if (houRem >= 24)
						houRem -= 24;
					lcd.clear();
					if (tabModes[2].actLCD)
						printCharLCD(tabModes[2].xLCD, tabModes[2].yLCD, tabModes[2].cLCD);
					printCharLCD(tabModes[1].xLCD, tabModes[1].yLCD, tabModes[1].cLCD);
					printTimeDate();
					currentMode = -1;
					
					// 
					secondPoint = millis();
					addingWastedTime(firstPoint, secondPoint);
				}
				// the user want to stop reminder
				else 
				{
					firstPoint = millis();

					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("Stop Reminder ?");
					lcd.setCursor(4, 1);
					lcd.print("Yes / No");
					boolean ok = true;
					while (ok)
					{
						delay(1000);
						//Stoping the reminder 
						if (digitalRead(chSwitchPin))
						{
							reminder = 0;
							houRem = 0;
							minRem = 0;
							currentMode = -1;
							lcd.clear();
							if (tabModes[2].actLCD)
								printCharLCD(tabModes[2].xLCD, tabModes[2].yLCD, tabModes[2].cLCD);
							printTimeDate();
							tabModes[1].actLCD = false;
							ok = false;
						}
						
						//Not stopping the reminder
						if (digitalRead(acSwitchPin))
						{
							ok = false;
							currentMode = -1;
							tabModes[1].actLCD = true;
							lcd.clear();
							printCharLCD(tabModes[1].xLCD, tabModes[1].yLCD, tabModes[1].cLCD);
							if (tabModes[2].actLCD)
								printCharLCD(tabModes[2].xLCD, tabModes[2].yLCD, tabModes[2].cLCD);
							printTimeDate();	
							delay(1000);
							printTimeDate();
						}
					}

					// second point (used to calculate time spent in here)
					secondPoint = millis();
					addingWastedTime(firstPoint, secondPoint);
				}
			}
			break;

			// third mode: A->setting an alarm
			case 2:
			{
				// activating (configure) the alram
				if (minAlarm == -1)
				{	
					// first point (used to calculate time spent in here)
					firstPoint = millis();

					delay(1000);
					lcd.clear();
					lcd.setCursor(1, 0);
					lcd.print("Set Alarm at:");
					lcd.setCursor(4, 1);
					lcd.print("00:0_");
					

					minAlarm = 0;
					houAlarm = 0;
					// index that will be incremented when the user press acSwitchPin
					// when it reaches 2 which means the user pressed the button
					// 3 times, the Alarm is set on.
					int setIndex = 0; 
					while (setIndex != 2)
					{	
						delay(500);
						lcd.setCursor(4, 1);
						if (houAlarm < 10)
							lcd.print("0");
						lcd.print(houAlarm);
						lcd.print(":");
						if (minAlarm < 10)
							lcd.print("0");
						lcd.print(minAlarm);

						if (digitalRead(chSwitchPin))
						{
							switch(setIndex)
							{
								case 0:
								{
									minAlarm++;
									if (minAlarm == 60)
										minAlarm = 0;
								}
								break;

								case 1:
								{
									houAlarm++;
									if (houAlarm == 24)
										houAlarm = 0;
								}
								break;
							}
						}

						// Printing a '_' so the user knows that he is changing
						// the hours
						if (setIndex == 1 && houAlarm == 0)
						{
							lcd.setCursor(4, 1);
							lcd.print("0_");
						}

						if (digitalRead(acSwitchPin))
						{
							setIndex++;
						}
					}

					

					// Setting the Alarm Secret code
					for (int i = 0; i < maxRange; i++)
					{
						secret[i] = 0;
						code[i] = 0;
					}
				
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("Set the code");
					lcd.setCursor(0, 1);
					lcd.print("...");
					delay(1000);
					listenToKnock(secret, true);

					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("done =)");
					delay(2000);
				
					// Proccede to the normal function of the clock
					tabModes[2].actLCD = true;
					lcd.clear();
					if (tabModes[1].actLCD)
						printCharLCD(tabModes[1].xLCD, tabModes[1].yLCD, tabModes[1].cLCD);

					printCharLCD(tabModes[2].xLCD, tabModes[2].yLCD, tabModes[2].cLCD);
					printTimeDate();
					currentMode = -1;

					// second point (used to calculate time spent in here)
					secondPoint = millis();
					addingWastedTime(firstPoint, secondPoint);
				}
				// the user wants to stop the alarm
				else
				{	
					// first point (used to calculate time spent in here)
					firstPoint = millis();

					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("Stop Alarm ?");
					lcd.setCursor(4, 1);
					lcd.print("Yes / No");
					boolean ok = true;
					while (ok)
					{
						delay(1000);

						// Stoping the Alarm 
						if (digitalRead(chSwitchPin))
						{
							houAlarm = 0;
							minAlarm = -1;
							currentMode = -1;
							lcd.clear();
							if (tabModes[1].actLCD)
								printCharLCD(tabModes[1].xLCD, tabModes[1].yLCD, tabModes[1].cLCD);
							printTimeDate();
							tabModes[2].actLCD = false;
							ok = false;
						}
						
						// Not stopping the Alarm
						if (digitalRead(acSwitchPin))
						{
							ok = false;
							currentMode = -1;
							tabModes[2].actLCD = true;
							lcd.clear();
							printCharLCD(tabModes[2].xLCD, tabModes[2].yLCD, tabModes[2].cLCD);
							if (tabModes[1].actLCD)
								printCharLCD(tabModes[1].xLCD, tabModes[1].yLCD, tabModes[1].cLCD);
							printTimeDate();	
							delay(1000);
							printTimeDate();
						}
					}

					// second point (used to calculate time spent in here)
					secondPoint = millis();
					addingWastedTime(firstPoint, secondPoint);
				}
			}
			break;
		}		
	}

	

	// Check if the reminder is activated and if it is the rigth time
	if (tabModes[1].actLCD &&
		dateTimeTab[1] == minRem && 
		dateTimeTab[2] == houRem)
	{		
		// first point (used to calculate time spent in here)
		firstPoint = millis();

		for (int i = 0; i < 5; i++)
		{
			tone(8, 587, 880);
			delay(1000);
		}
			
		// We inc the minRem and houRem
		// If the minutes gets over 60 we subtract 60mins
		// and we add 1 hour to the houRem
		// If the hours gets over 24 we subtract 24 hours
		minRem += (reminder % 60);
		if (minRem >= 60)
		{
			minRem -= 60;
			houRem++;
		}
		houRem += (reminder / 60);
		if (houRem >= 24)
			houRem -= 24;

		// second point (used to calculate time spent in here)
		secondPoint = millis();
		addingWastedTime(firstPoint, secondPoint);
	}

	// Check if the alarm is activated and if it is the rigth time
	if (tabModes[2].actLCD && 
		dateTimeTab[1] == minAlarm && 
		dateTimeTab[2] == houAlarm)
	{	
		// first point (used to calculate time spent in here)
		firstPoint = millis();

		lcd.clear();
		lcd.setCursor(3, 0);
		lcd.print("Game Over!");
		lcd.setCursor(0, 1);
		lcd.print("It's time ");
			
		playTetris();

		lcd.clear();

		for (int i = 1; i < 3; i++)
			if (tabModes[i].actLCD)
				printCharLCD(tabModes[i].xLCD, tabModes[i].yLCD, tabModes[i].cLCD);

		// second point (used to calculate time spent in here)
		secondPoint = millis();
		addingWastedTime(firstPoint, secondPoint);
	}

	
	incTime(999);
	printTimeDate();
}


/**
 * initMode() initializes the modes.
 * It set the coordinates (@param x, @param y) and the @param c.
 * The @param act is used for knowing if the mode is activated or not.
 */
void initMode(struct Mode *m, int x, int y, char c, boolean act)
{
	m->xLCD = x;
	m->yLCD = y;
	m->cLCD = c;
	m->actLCD = act;
}


/**
 * @function incTime() increments the time and the date.
 * It uses @param delayTime to delay the incrementation.
 */
void incTime(int delayTime)
{	
	delay(delayTime);
	dateTimeTab[0]++;

	// Seconds
	if (dateTimeTab[0] == 60)
	{
		dateTimeTab[0] = 0;
		dateTimeTab[1]++;
	}

	// Minutes
	if (dateTimeTab[1] == 60)
	{
		dateTimeTab[1] = 0;
		dateTimeTab[2]++;
	}

	//Hours
	if (dateTimeTab[2] == 24)
	{
		dateTimeTab[2] = 0;
		dateTimeTab[3]++;
	}

	// Days
	if (dateTimeTab[3] == (monthDay[dateTimeTab[4] - 1] + 1))
	{
		dateTimeTab[3] = 1;
		dateTimeTab[4]++;
	}

	// Months
	if (dateTimeTab[4] == 13)
	{
		dateTimeTab[4] = 1;
		dateTimeTab[5]++;
	}
}


/**
 * @function printTimeDate() prints time and date into the LCD.
 * The format of the date/time is conserved in:
 * DD/MM/YYYY
 *  HH:MM:SS
 */
void printTimeDate()
{
	// Time
	lcd.setCursor(4, 1);
	if (dateTimeTab[2] < 10)
		lcd.print("0");
	lcd.print(dateTimeTab[2]);
	lcd.print(":");
	if (dateTimeTab[1] < 10)
		lcd.print("0");
	lcd.print(dateTimeTab[1]);
	lcd.print(":");
	if (dateTimeTab[0] < 10)
		lcd.print("0");
	lcd.print(dateTimeTab[0]);

	// Date
	lcd.setCursor(3, 0);
	if (dateTimeTab[3] < 10)
		lcd.print("0");
	lcd.print(dateTimeTab[3]);
	lcd.print("/");
	if (dateTimeTab[4] < 10)
		lcd.print("0");
	lcd.print(dateTimeTab[4]);
	lcd.print("/");
	lcd.print(dateTimeTab[5]);
}

/**
 * @param printCharLCD() prints @param c into the LCD
 * at the location (@param x, @param y).
 */
void printCharLCD(int x, int y, char c)
{
	lcd.setCursor(x, y);
	lcd.print(c);
}

/**
 * deleteCharLCD() prints a white space into the LCD
 * at the location (@param x, @param y).
 * It is used to clear single charcter from the LCD.
 */
void deleteCharLCD(int x, int y)
{
	lcd.setCursor(x, y);
	lcd.print(" ");
}

/**
 * rightValue() will check if the value at @param index is equal
 * to @param compareTo, if so it will change it to @param initTo.
 * 
 * Months:  from 1 to 12
 * Days:    from 1 to 30 || 31 || 28 || 29
 * Hours:   from 00 to 23
 * Minutes: from 00 to 59
 * Seconds: from 00 to 59
 */
void rightValue(int index, int compareTo, int initTo)
{
	dateTimeTab[index]++;
	printTimeDate();
	if (dateTimeTab[index] == compareTo)
	{
		dateTimeTab[index] = initTo;
		printTimeDate();
	}
	delay(250);
}

/**
 * @function playTetris() plays the alarm song and test if
 * the user has entered the right sequence of knocks to stop it.
 */
void playTetris()
{
	boolean equals = false;

	while (!equals)
	{
		for (int thisNote = 0; thisNote < 384; thisNote++) 
	    {
	    	int noteDuration = 1000 / pgm_read_byte_near(tetrisNoteDurations + thisNote);
	    	tone(8, pgm_read_word_near(tetrisMelody + thisNote), noteDuration);
	    	int pauseBetweenNotes = noteDuration * 1.3;
	      	delay(pauseBetweenNotes);
	      	
	      	if (digitalRead(acSwitchPin))
	      	{	
	      		noTone(8);
	      		int i;
	      		lcd.clear();
	      		lcd.setCursor(0, 0);
	      		lcd.print("Enter Code after");
	      		for (i = 3; i > 0; i--)
	      		{
	      			lcd.setCursor(0, 1);
	      			lcd.print(i);
	      			lcd.print(" seconds");
	      			delay(1000);
	      		}
	      		lcd.clear();
	      		lcd.setCursor(0, 0);
	      		lcd.print("Now");
	      		listenToKnock(code, false);

	      		i = 0;
				equals = true;
				while (i < secretIndex)
				{
					if (secret[i] - code[i] > 100 || secret[i] - code[i] < -100)
						equals = false;

					i++;
				}

				delay(500);

				if (equals)
				{       
                                        houAlarm = 0;
                                        minAlarm = -1;
					break;
				}
				else
				{
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("Wrong!!!");
				}
	      	}
	    }
	}
    
}



/**
 * @function listenToKnock() detects the knocks through the piezo on A0
 * and saves the delays between them in an array.
 * @param notCode is true when we are saving it, else for testing.
 */
void listenToKnock(int tab[], boolean notCode)
{
	int index = 0;
	boolean inLoop = true;
	delay(1000);
	while (inLoop)
	{
		piezoInput = analogRead(A0);
		if (piezoInput >= 40)
		{
			now = millis();
			while (!digitalRead(acSwitchPin))
			{
				piezoInput = analogRead(A0);
				if (piezoInput >= 40)
				{                   
				    startTime = now;

				    now = millis();
				    tab[index] = now - startTime;
				    delay(125);
				    index++;
				}
			}
			if (notCode)
				secretIndex = index + 1;
			inLoop = false;
		}
	}
}


/**
 * @function addingWastedTime() calclute the time in seconds
 * between @param fisrtPoint and @param secondPoint,
 * then it increments the current time to the right amount.
 */
void addingWastedTime(long fisrtPoint, long secondPoint)
{	
	// result will holds the time between two points in seconds
	int result = (secondPoint - firstPoint) / 1000;


	for (int i = 0; i < result; i++)
		incTime(0);

	if (result % 1000 >= 500)
		incTime(0);
}
