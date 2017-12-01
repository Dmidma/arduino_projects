
const int sensorPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {



  Serial.println(2076 / (analogRead(sensorPin) - 11));

  delay(2000);

  
  //int value = analogRead(sensorPin);

  /*
  float volts = analogRead(sensorPin)*0.0048828125;  // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1); // worked out from datasheet graph
  delay(1000); // slow down serial port 
 
    delay(2000);
  */
  
  

}
