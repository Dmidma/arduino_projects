int switchState = 0;

void setup()
{
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(2, INPUT);
}

void loop() 
{
  switchState = digitalRead(2);
  
  if (switchState == LOW) // The button is not pressed
  {
    digitalWrite(3, HIGH); //Green LED is on
    digitalWrite(4, LOW); //Red LED is off
    digitalWrite(5, LOW); //Red LED is off
  }
  else  //The button is pressed
  {
    digitalWrite(3, LOW); //Green LED is off
    digitalWrite(4, LOW); //Red LED is off
    digitalWrite(5, HIGH);  //Red LED is on
    
    delay(250); //Wait for a quarter second
    
    //Toggle the LEDs

    digitalWrite(4, HIGH); //Red LED is on
    digitalWrite(5, LOW); //Red LED is off
    
    delay(250); //Wait for a quarter second
  }

}
