int button = 2;

void setup()
{
    Serial.begin(9600);
    pinMode(button, INPUT); 
}

void loop()
{
 
    if (digitalRead(button) == HIGH)
    {
        Serial.print('B');
        delay(10); // prevents overwhelming the serial port
    }
}
