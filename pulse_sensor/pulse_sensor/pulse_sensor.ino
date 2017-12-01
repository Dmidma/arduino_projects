
/*  Pulse Sensor Multi Sensor
     by
     Joel Murphy and Yury Gitman   http://www.pulsesensor.com *   Updated Winter 2017
     World Famous Electronics llc.  Public Domain. 2017
  ----------------------  Notes ----------------------  ----------------------
  This code:
  1) Blinks an LED to two user's Live Heartbeat   PIN 13 and PIN 12
  2) Fades an LED to two user's Live HeartBeat    PIN 5 and PIN 9
  3) Determines BPMs for both users
  4) Prints All of the Above to Arduino Serial Plotter or our Processing Visualizer
  Plug the Pulse Sensor RED wires into UNO pins 7 and 8 for 5V power! 
  Plug the Pulse Sensor BLACK wires into the GND pins
  Plug the Pulse Sensor PURPLE wires into Analog 0 and Analog 1 pins
  Read Me:
  https://github.com/WorldFamousElectronics/PulseSensor_Amped_Arduino/blob/master/README.md
  ----------------------       ----------------------  ----------------------
*/

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2

//  Variables
const int numPulseSensors = 2;
const int threshold = 530;
int pulsePin[numPulseSensors];                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin[numPulseSensors];                // pin to blink led at each beat
int fadePin[numPulseSensors];                  // pin to do fancy classy fading blink at each beat
int fadeRate[numPulseSensors];                 // used to fade LED on with PWM on fadePin




// Volatile Variables, used in the interrupt service routine!
volatile int BPM[numPulseSensors];                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal[numPulseSensors];                // holds the incoming raw data
volatile int IBI[numPulseSensors];             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse[numPulseSensors];     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS[numPulseSensors];        // becomes true when Arduoino finds a beat.

// Regards Serial OutPut  -- Set This Up to your needs
static boolean serialVisual = false;   // Set to 'false' by Default.  Re-set to 'true' to see Arduino Serial Monitor ASCII Visual Pulse

volatile int rate[numPulseSensors][10];                    // array to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime[numPulseSensors];           // used to find IBI
volatile int P[numPulseSensors];                      // used to find peak in pulse wave, seeded
volatile int T[numPulseSensors];                     // used to find trough in pulse wave, seeded
volatile int thresh[numPulseSensors];                // used to find instant moment of heart beat, seeded
volatile int amp[numPulseSensors];                   // used to hold amplitude of pulse waveform, seeded
volatile boolean firstBeat[numPulseSensors];        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat[numPulseSensors];      // used to seed rate array so we startup with reasonable BPM

// SET THE SERIAL OUTPUT TYPE TO YOUR NEEDS
// PROCESSING_VISUALIZER works with Pulse Sensor Processing Visualizer
//      https://github.com/WorldFamousElectronics/PulseSensor_Amped_Processing_Visualizer
// SERIAL_PLOTTER outputs sensor data for viewing with the Arduino Serial Plotter
//      run the Arduino Serial Plotter at 115200 baud: Tools/Serial Plotter or Command+L
static int outputType = PROCESSING_VISUALIZER;

void setup() {

  setStuph();                       // initialize variables and pins

  Serial.begin(250000);             // we agree to talk fast!

  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS
}


void loop() {

  serialOutput() ;

  for (int i = 0; i < numPulseSensors; i++) {
    if (QS[i] == true) {
      fadeRate[i] = 0;         // Makes the LED Fade Effect Happen
      // Set 'fadeRate' Variable to 255 to fade LED with pulse
      serialOutputWhenBeatHappens(i);   // A Beat Happened, Output that to serial.
      QS[i] = false;
    }
  }

  ledFadeToBeat();                      // Makes the LED Fade Effect Happen
  delay(20);                            //  take a break
}




// FADE BOTH LEDS
void ledFadeToBeat() {
  for (int j = 0; j < numPulseSensors; j++) {
    fadeRate[j] += 15;
    fadeRate[j] = constrain(fadeRate[j], 0, 255); //  keep RGB LED fade value from going into negative numbers!
    analogWrite(fadePin[j], fadeRate[j]);         //  fade LED
  }

}


// INITIALIZE VARIABLES AND INPUT/OUTPUT PINS
void setStuph() {

  // use power from Arduino pins to power PulseSensors with +5V Power!
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

//    Possible Power for 3rd PulseSensor on Pin 7, un-comment
//      pinMode(7,OUTPUT);
//      digitalWrite(7, HIGH);


/*
 * Initialize variables for each pulse sensor so we can find the beats
 */

  for (int i = 0; i < numPulseSensors; i++) {
    lastBeatTime[i] = 0;
    P[i] = T[i] = 512;
    amp[i] = 0;
    thresh[i] = threshold;
    amp[i] = 0;               // used to hold amplitude of pulse waveform, seeded
    firstBeat[i] = true;      // used to seed rate array so we startup with reasonable BPM
    secondBeat[i] = false;
    IBI[i] = 600;             // int that holds the time interval between beats! Must be seeded!
    Pulse[i] = false;         // "True" when User's live heartbeat is detected. "False" when not a "live beat".
    QS[i] = false;
    switch (i) {
      case  0:
        pulsePin[i] = 0;    // pulse pin Analog 0
        blinkPin[i] = 13;   // blink output for pulse 0
        fadePin[i] = 5;     // fade output for pulse 0
        break;
      case  1:
        pulsePin[i] = 1;    // pulse pin Analog 1
        blinkPin[i] = 12;   // blink output for pulse 1
        fadePin[i] = 9;     // fade output for pulse 1
        break;
      // add more if you need to here
      default:
        break;
    }

    pinMode(blinkPin[i], OUTPUT);        // pin that will blink to your heartbeat!
    digitalWrite(blinkPin[i], LOW);
    pinMode(fadePin[i], OUTPUT);         // pin that will fade to your heartbeat!
    analogWrite(fadePin[i], 255);

  }
}


void interruptSetup(){     
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
} 


// THIS IS THE TIMER 2 INTERRUPT SERVICE ROUTINE. 
// Timer 2 makes sure that we take a reading every 2 miliseconds
ISR(TIMER2_COMPA_vect){                           // triggered when Timer2 counts to 124

  cli();                                          // disable interrupts while we do this
  
  for(int i=0; i<numPulseSensors; i++){
    Signal[i] = analogRead(pulsePin[i]);          // read the Pulse Sensors
  }
  sampleCounter += 2;  // keep track of the time in mS with this variable
  
  for(int i=0; i<numPulseSensors; i++){

                          
    int N = sampleCounter - lastBeatTime[i];      // monitor the time since the last beat to avoid noise

    //  find the trough and peak of the pulse wave
    if(Signal[i] < thresh[i] && N > (IBI[i]/5)*3){// avoid dichrotic noise by waiting 3/5 of last IBI
      if (Signal[i] < T[i]){                      // T is the trough
        T[i] = Signal[i];                         // keep track of lowest point in pulse wave 
      }
    }

    if(Signal[i] > thresh[i] && Signal[i] > P[i]){// thresh condition helps avoid noise
      P[i] = Signal[i];                           // P is the peak
    }                                             // keep track of highest point in pulse wave

    //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
    // signal surges up in value every time there is a pulse
    if (N > 250){                                 // avoid high frequency noise
      if ( (Signal[i] > thresh[i]) && (Pulse[i] == false) && (N > (IBI[i]/5)*3) ){  
        IBI[i] = sampleCounter - lastBeatTime[i]; // measure time between beats in mS
        
        Pulse[i] = true;                          // set the Pulse flag when we think there is a pulse
        digitalWrite(blinkPin[i],HIGH);           // turn on pin 13 LED
        lastBeatTime[i] = sampleCounter;          // keep track of time for next pulse
  
        if(secondBeat[i]){                        // if this is the second beat, if secondBeat == TRUE
          secondBeat[i] = false;                  // clear secondBeat flag
          for(int j=0; j<=9; j++){                // seed the running total to get a realisitic BPM at startup
            rate[i][j] = IBI[i];                      
          }
        }
  
        if(firstBeat[i]){                         // if it's the first time we found a beat, if firstBeat == TRUE
          firstBeat[i] = false;                   // clear firstBeat flag
          secondBeat[i] = true;                   // set the second beat flag
          sei();                                  // enable interrupts again
          return;                                 // IBI value is unreliable so discard it
        }   
  
  
        // keep a running total of the last 10 IBI values
        word runningTotal = 0;                    // clear the runningTotal variable    
  
        for(int j=0; j<=8; j++){                  // shift data in the rate array
          rate[i][j] = rate[i][j+1];              // and drop the oldest IBI value 
          runningTotal += rate[i][j];             // add up the 9 oldest IBI values
        }
  
        rate[i][9] = IBI[i];                      // add the latest IBI to the rate array
        runningTotal += rate[i][9];               // add the latest IBI to runningTotal
        runningTotal /= 10;                       // average the last 10 IBI values 
        BPM[i] = 60000/runningTotal;              // how many beats can fit into a minute? that's BPM!
        QS[i] = true;                             // set Quantified Self flag 
        // QS FLAG IS NOT CLEARED INSIDE THIS ISR
      }                       
    }

    if (Signal[i] < thresh[i] && Pulse[i] == true){   // when the values are going down, the beat is over
      digitalWrite(blinkPin[i],LOW);            // turn off pin 13 LED
      Pulse[i] = false;                         // reset the Pulse flag so we can do it again
      amp[i] = P[i] - T[i];                     // get amplitude of the pulse wave
      thresh[i] = amp[i]/2 + T[i];              // set thresh at 50% of the amplitude
      P[i] = thresh[i];                         // reset these for next time
      T[i] = thresh[i];
    }

    if (N > 2500){                              // if 2.5 seconds go by without a beat
      thresh[i] = threshold;                    // set thresh default
      P[i] = 512;                               // set P default
      T[i] = 512;                               // set T default
      lastBeatTime[i] = sampleCounter;          // bring the lastBeatTime up to date        
      firstBeat[i] = true;                      // set these to avoid noise
      secondBeat[i] = false;                    // when we get the heartbeat back
    }
  }
  sei();                                   // enable interrupts when youre done!
}// end isr



void serialOutput(){   // Decide How To Output Serial.
  switch(outputType){
    case PROCESSING_VISUALIZER:
      for(int i=0; i<numPulseSensors; i++){
        sendDataToSerial('a'+i, Signal[i]);
      }
      break;
    case SERIAL_PLOTTER:  // open the Arduino Serial Plotter to visualize these data
      for(int i=0; i<numPulseSensors; i++){
//        Serial.print(BPM[i]);     //
//        Serial.print(",");        //  UNCOMMENT THESE LINES TO SEE BPM AND IBI IN THE ARDUINO SERIAL PLOTTER
//        Serial.print(IBI[i]);     //
//        Serial.print(",");        //
        Serial.print(Signal[i]);
        if(i!=numPulseSensors-1){ Serial.print(","); }
      }
      Serial.println();
      break;
    default:
      break;
  }

}



void serialOutputWhenBeatHappens(int p){
  char c;
  switch(outputType){
      case PROCESSING_VISUALIZER:    
        c = 'A'+ p;                   // send first sensor BPM with prefix 'A'
        sendDataToSerial(c,BPM[p]);   // send successive sensor BPM with prefix 'B', 'C', etc
        c = 'M'+ p;                   // send first sensor IBI with prefix 'M'
        sendDataToSerial(c,IBI[p]);   // send successive sensor IBI with prefix 'N', 'O', etc 
      break;
    default:
      break;
  }
}


//  Sends Data to Pulse Sensor Processing App, Native Mac App, or Third-party Serial Readers.
void sendDataToSerial(char symbol, int data ){
    Serial.print(symbol);
    Serial.println(data);
  }

