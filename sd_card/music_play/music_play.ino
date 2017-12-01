
#define STEREO_OR_16BIT 

#include <TMRpcm.h>



#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch
 
void setup(){

  Serial.begin(9600);


  pinMode(9, OUTPUT);

  
  tmrpcm.speakerPin = 9; //11 on Mega, 9 on Uno, Nano, etc
 
 
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
  return;   // don't do anything more if not
  }
  if (SD.exists("test2.wav"))
  {
     Serial.println("Here");
     tmrpcm.volume(2);
     tmrpcm.setVolume(1);
     delay(5000);
     tmrpcm.play((char*) "test2.wav");   
     if (tmrpcm.isPlaying())
        Serial.println("Yes I am");
     else
        Serial.println("No I am no");
  }
  else
    Serial.println("No");
 
      
 
}
 
void loop(){  

     
      
  
    }
  

