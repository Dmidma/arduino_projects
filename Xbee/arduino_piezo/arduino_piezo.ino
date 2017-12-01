int piezo = 4;


void setup()
{
    Serial.begin(9600);
    pinMode(piezo, OUTPUT);  
}

void loop()
{
  
  if (Serial.available() > 0)
  {
   
   if (Serial.read() == 'B')
   { 
      digitalWrite(piezo, HIGH);
      delay(10);
      digitalWrite(piezo, LOW);
   }
    
  }

}



