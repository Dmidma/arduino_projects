int input = A0;

void setup() 
{
  Serial.begin(9600);
  
}

void loop() 
{ 

  int sensorValue = analogRead(input);
  float voltage = sensorValue * (0.5 / 1023.0);

  
  Serial.println(voltage);
  delay(1);

}
