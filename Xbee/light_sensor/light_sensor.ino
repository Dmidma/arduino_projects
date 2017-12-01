 int LED = 10;
int debugLed = 13;
int analogValue = 0;


void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode(debugLed, OUTPUT);
  Serial.begin(9600);
}


void loop()
{
 
  
  if (Serial.available() >= 21)
  {
    if (Serial.read() == 0x7E) {
      
      Serial.println("Begin");
      
        byte current_value; 
       do
       {
         current_value = Serial.read();

         
         Serial.println(current_value, HEX);
           
       } while (current_value != 0x7E);

       
       Serial.println("End");
    }
    

  }
  
  
  
  /*
  if (Serial.available() >= 21)
  {
    

    
    if (Serial.read() == 0x7E)
    {
      
      
      digitalWrite(debugLed, HIGH);
      delay(10);
      digitalWrite(debugLed, LOW);
      
      for (int i = 0; i < 18; i++)
      {
        byte discard = Serial.read();
      }
      
      int analogHigh = Serial.read();
      Serial.println("High:" + analogHigh);
      int analogLow = Serial.read();
      Serial.println("Low:" + analogLow);
      analogValue = analogLow + (analogHigh * 256);
    }
  }
  
  if (analogValue > 0 && analogValue <= 350)
  {
    digitalWrite(LED, LOW);
  }
  
  
  if (analogValue > 350 && analogValue <= 750) 
  {
    digitalWrite(LED, HIGH);
  }
  
  if (analogValue > 750 && analogValue <= 1023)
  {
    digitalWrite(LED, LOW);
  }
   */
  
}
