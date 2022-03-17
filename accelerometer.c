// --------------------------------------------------------------------------------------
//Mark Tan - 2021, May 15

//This program calculates the average acceleration of a moving sonar sensor
//within a user-specified start and end position.
//Note: calculations become inaccurate past a distance range of 20cm

// Code for Sonar sourced from APSC 101- 2020WT2:
// https://canvas.ubc.ca/courses/57724/pages/arduino-mechanical-and-demo-videos
// --------------------------------------------------------------------------------------

#include <Servo.h>

#include <NewPing.h>      // include the NewPing library for this program

#define VCC_PIN 13
#define TRIGGER_PIN 12    // sonar trigger pin will be attached to Arduino pin 12
#define ECHO_PIN 11       // sonar echo pint will be attached to Arduino pin 11
#define GROUND_PIN 10
#define MAX_DISTANCE 200  // fmaximum distance set to 200 cm
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // initialize NewPing


#define TRUE 1
#define FALSE 0

Servo myservo1 ;

// defines variables ----------------------

int previousDistance = 0;

const long interval = 500;  //interval at which to calculate velocity/acceleration (milliseconds)
unsigned long previousMillis = 0;     // used to compare to current time

int positionDifference;
long timeDifference;

// position distances ----------
int squatDistance = 35;
int standingDistance = 45;

// --------------------

int squatPosition;

int checked = FALSE; 

long velocity;

long acceleration;

long maxVelocity = 0;

//do the following things once

void setup() {
  Serial. begin(9600);            // set data transmission rate to communicate with computer
  pinMode(ECHO_PIN, INPUT) ;  
  pinMode(TRIGGER_PIN, OUTPUT) ;
  pinMode(GROUND_PIN, OUTPUT);    // tell pin 10 it is going to be an output
  pinMode(VCC_PIN, OUTPUT);       // tell pin 13 it is going to be an output
  digitalWrite(GROUND_PIN,LOW);   // tell pin 10 to output LOW (OV, or ground)
  digitalWrite(VCC_PIN, HIGH) ;   // tell pin 13 to output HIGH (+5V)


  myservo1.attach(6);                  // attaches the servo on pin 9 to the servo object


}

void loop() {

  // calculate distance
  delay(10);                             // wait for this number of milliseconds
  int distance = sonar.ping_cm();   // read the sonar sensor, using a variable

  // CALCULATING VELOCITY -------------------
  
  // get current time
  unsigned long currentMillis = millis();

  if (distance <= squatDistance){                           //SQUAT POSITION

     //Serial.print("-- SQUATTING - ") ; 
     
     // comparsion time equal to current until out of loop
     previousMillis = currentMillis; 
     // comparison distance
     squatPosition = distance;

     checked = FALSE;

     velocity = 0;
  }

  if (distance >= standingDistance){                        //STANDING POSITION
    
     //Serial.print(" -- STANDING - ") ;

     positionDifference = distance - squatPosition;
     timeDifference = currentMillis - previousMillis;

     // ONLY CALCULATE VELOCITY ONCE REACHED STANDING HEIGHT
     if (checked == FALSE){
      
        velocity = positionDifference*1000 / timeDifference;

        checked = TRUE;
      
     }  
     
  }

  // check for negative values
  if (velocity < 0){
    velocity = -1*velocity;
    
    //Serial.println(" ------- ########## Negative ################ ") ;

   }

  // check for distance error velocities 

  if (velocity > 150){
   velocity = 0;
  }

  // caculate max velocity

  if (velocity > maxVelocity){

    maxVelocity = velocity;
  }

  // Calculate acceleration -------
  acceleration = velocity / timeDifference;         // velocity = change in velocity. With the
                                                    // initial velocity = 0;

  

   //int servoValX = map(distance, 4 , 150, 180, 0) ; // sonar values to servo

   //int servoValX = map(velocity, 0 , 100, 0, 180) ; // velocity values to servo

   int servoValX = map(velocity, 0 , 50, 0, 180) ; // acceleration values to servo

   //int servoValX = map(maxVelocity, 0 , 50 , 0, 180) ; // MAX velocity values to servo

   
   myservo1.write(servoValX);                            //write the calculated value to the servo
 
    
  Serial.print(" Distance: ") ;                // Prints the distance on the Serial Monitor
  Serial.print(distance);

  Serial.print(" / Velocity: (cm/s)  ") ;      // Prints acceleration to the Serial Monitor
  Serial.println(acceleration) ;      
  
 

  /*
 
  Serial.print(" / Velocity: (cm/s)  ") ;      // Prints velocity to the Serial Monitor
  Serial.println(velocity) ;      
  
  Serial.print(" -- Time Elapsed: (ms) ") ;      // Prints the time elapsed on the Serial Monitor
  Serial.print(currentMillis);

  Serial.print(" -- TIME DIFF (ms): ") ;      // Prints the time difference on the Serial Monitor
  Serial.print(timeDifference);

  Serial.print(" -- DISTANCE DIFF (cm): ") ;      // Prints the distance difference on the Serial Monitor
  Serial.print(positionDifference);

  Serial.print(" / SERVO X ") ;      // Prints serovo value to the Serial Monitor
  Serial.print(servoValX) ;    
  
  Serial.print(" / checkVal =  ") ;      // Prints to the Serial Monitor
  Serial.println(checked) ;     

  */
  
}