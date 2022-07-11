//#include "HX711.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//actuators
#define toy 4
#define swing 5  //relay logic reverse
#define fan 6
#define light 8

//sensors 
#define wetSensor 3
#define tempSensor A1
#define soundSensor 2


//global variables 
#define MAX_TEMP 27 
//#define CALIBRATION_FACTOR -7050.0
//#define LOADCELL_DOUT_PIN  3
//#define LOADCELL_SCK_PIN  2



static const uint8_t PIN_MP3_TX = 10; // RX for audio
static const uint8_t PIN_MP3_RX = 11; // TX for audio
SoftwareSerial mp3Serial(PIN_MP3_RX, PIN_MP3_TX);


//class objects created here
//HX711 scale;
DFRobotDFPlayerMini player;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
//  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
//  scale.set_scale(calibration_factor);
  
  mp3Serial.begin(9600);
  if (player.begin(mp3Serial)) {
   Serial.println("MP3 OK");
  }
  else {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
  
  //Actuators pins setup
  pinMode(toy,OUTPUT);
  pinMode(swing,OUTPUT);
  pinMode(fan,OUTPUT);
  pinMode(light,OUTPUT);

  digitalWrite(toy,LOW);
  digitalWrite(swing,HIGH);
  digitalWrite(fan,LOW);
  digitalWrite(light,HIGH);
  
  delay(100);

  //sensors pins setup
  pinMode(wetSensor,INPUT);
  pinMode(tempSensor,INPUT);

}



//This function can turn on or off the Toy 
void onOffToy(String state)
{ 
  Serial.println("Toy Function");
  if(state=="ON")
    {
       digitalWrite(toy,HIGH);
      
       delay(100);
       
    }
  else
    {
       digitalWrite(toy,LOW);
      
       delay(100);
    }
}



//This function can turn on or off the Fan
void onOffFan(String state)
{
  
  if(state=="ON")
    {
       digitalWrite(fan,HIGH);
       delay(100);
    }
  else
    {
       digitalWrite(fan,LOW);
       delay(100);
    }
  
}



//This function can turn on or off the Swing Mode of Cradle
void onOffSwingingCradle(String state)
{
    if(state=="ON")
    {
       digitalWrite(swing,LOW);
       delay(100);
    }
  else
    {
       digitalWrite(swing,HIGH);
       delay(100);
    } 
}


//This function can turn on or off the Light of Cradle
void onOffCradleLight(String state)
{
    if(state=="ON")
    {
       digitalWrite(light,LOW); //relay logic is reverse
       delay(100);
    }
  else
    {
       digitalWrite(light,HIGH); //relay logic is reverse
       delay(100);
    } 
}




//float checkBabyWeight()
//{ 
//  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
//  float weightInLbs = scale.get_units();
//  return weightInLbs;
//}

boolean wetDetection()
{
 
  if(digitalRead(wetSensor))
    {
       Serial.println("WET DETECTED IN CRADLE");
       return true;
    }

  return false;  
    
}

boolean checkHeat()
{
   float tempValue;
   int tempAdcValue = analogRead(tempSensor);
   tempValue = ((tempAdcValue/1024.0)*3300);
   tempValue = (tempValue/10);
   Serial.print("Temperature Value : ");
   Serial.print(tempValue);
   if(tempValue>=MAX_TEMP)
     {
       return true;
     }
   return false;  

}

boolean babyCryDetection()
{
  
  if(digitalRead(soundSensor)){
    Serial.println("Baby Cry Detected");
    return true;
  }
  return false;
}


void playPauseMusic(String action,String type)
{
    if(action=="PAUSE")
    {
       player.pause();
       delay(500);
       Serial.println("Music Paused");
    }
    else
    {
        player.volume(30);
        if(type=="LORRY")
           {
            player.play(1);
           }
        else if(type=="SLEEP")
           {
            player.play(3);
           } 
        else 
           {
            player.play(5);
           }
    }
}


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0)
  {
    char Incoming_value = Serial.read();
    Serial.println(Incoming_value);  
    
    if(Incoming_value=='A')
      {
        onOffSwingingCradle("ON");
        Serial.write("1");
      }
    else if(Incoming_value=='B')
      {
        onOffSwingingCradle("OFF");
      }    
    else if(Incoming_value=='C')
      {
        playPauseMusic("PLAY","SLEEP");
      }    
    else if(Incoming_value=='D')
      {
        playPauseMusic("PAUSE","SLEEP");
      }   
    else if(Incoming_value=='E')
      {
        playPauseMusic("PLAY","LORRY");
      }    
    else if(Incoming_value=='F')
      {
        playPauseMusic("PAUSE","LORRY");
      }   
    else if(Incoming_value=='G')
      {
        onOffToy("ON");
      }    
    else if(Incoming_value=='H')
      {
        onOffToy("OFF");
      } 
        else if(Incoming_value=='I')
      {
         playPauseMusic("PLAY","NATURE");
      }    
    else if(Incoming_value=='J')
      {
         playPauseMusic("PAUSE","NATURE");
      }
    else if(Incoming_value=='K')
      {
        onOffFan("ON");
      }
    else if(Incoming_value=='L')
      {
        onOffFan("OFF");
      }      
    else if(Incoming_value=='M')
      {
        onOffCradleLight("ON");
      }
    else if(Incoming_value=='N')
      {
        onOffCradleLight("OFF");
      } 
    

    //
    
  }
  boolean heatStatus = checkHeat();
    if(heatStatus)
    {
      onOffFan("ON");
      //Serial.println("Fan ON");
    }
    else{
      onOffFan("OFF");
       //Serial.println("Fan OFF");
    }

    boolean wetStatus = checkHeat();
    if(wetStatus)
    {
      Serial.println("Wet Detected");
      //Serial.write("1");
    }
    else{
      Serial.println("DRY Detected");
      //Serial.write("0");
    }

    boolean cryStatus = babyCryDetection();
    if(cryStatus)
    {
        Serial.println("Baby Crying");
//      onOffSwingingCradle("ON");
//      onOffFan("ON");
//      onOffToy("ON");
     
    }
    else{
      //Serial.println("Baby No Crying");
//      onOffSwingingCradle("OFF");
//      onOffFan("OFF");
//      onOffToy("OFF");
    }
    delay(300);

}
