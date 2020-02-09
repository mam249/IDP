#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "matplotlibcpp.h"
//namespace plt = matplotlibcpp;
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);
// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
Adafruit_DCMotor *clawMotor = AFMS.getMotor(3);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);
using namespace std;



// constants won't change. Used here to set a pin number:
const int AmberledPin =  6;// the number of the LED pin
const int RedledPin =  5;
const int GreenledPin =  7;
// Variables will change:
int AmberledState = LOW;             // ledState used to set the LED
int RedledState = LOW;  
int GreenledState = LOW;  

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)


int stage;
unsigned long time_at_end_of_stage0; // the time at end of stage 0 is the time at end of stage -1 
unsigned long time_at_end_of_stage1; //the time at the end of each individual stage
unsigned long time_at_end_of_stage2;
unsigned long time_at_end_of_stage3;
unsigned long time_at_end_of_dropstage;
unsigned long time_at_end_of_scanstage;
int sensorpin1 = 1;                 // analog pin used to connect the sharp sensor
int sensorpin2 = 0;                 // analog pin used to connect the sharp sensor
int hallsensor = A2;
int origin;
int i = 0;
bool liveMine;


float stop1;
float stophighest;
float pi;
float desired_angle;
float rpm;
float wheel_dist;
float wheel_radius;
float turn_time;
float current_angle;
float length_of_time;
float highest_angle;
float clear_path_angle;
int r_speed;
int l_speed;
int oldlspeed;
int oldrspeed;
// variable to store the value coming from the sensor
int trig = 2; 
int echo = 3; 
long lecture_echo; 
long cm;
long min_cm = 100000L;
float min_distance;
 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           // set up Serial library at 9600 bps
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pi= 3.141592653589793238;
  wheel_radius=0.049; // values measured on the actual robot 
  wheel_dist=0.23;
  rpm=19.61;
  desired_angle= pi/2;
  turn_time = (15 * desired_angle * wheel_dist * 0.9 )/ (rpm * pi * wheel_radius); //calculates the turn time required to achieve a 90 degree (or other desired angle) turn 
  stop1=millis()+(turn_time * 1000); //adds the turn time to the current millis so robot knows when to stop 
  pinMode(AmberledPin, OUTPUT);//for the amber led
  pinMode(RedledPin, OUTPUT);
  pinMode(GreenledPin, OUTPUT);
  pinMode(echo, INPUT); 
  pinMode(trig, OUTPUT); 
  pinMode(hallsensor, INPUT);
  stage = -2; //starting the robot at stage -2 

 time_at_end_of_dropstage = 0 ;

  
}


void rightmotor(int r_speed) {
    //check to see if the old speed matches the speed we want
  if (r_speed == oldrspeed){
    return;}

  else {
    
    //set the speed of the motor to the new speed
    myMotor2->setSpeed(r_speed);
    //store the new speed as the old speed for future changes 
    oldrspeed = r_speed;
    
  }
}
//repeat same for left motor 
void leftmotor(int l_speed) {
  if (l_speed == oldlspeed){
    return;}

  else {
    
    //choose whichever motor is the right one 
    myMotor->setSpeed(l_speed);
    oldlspeed = l_speed;
  }
}





void loop() {
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (AmberledState == LOW) {
      AmberledState = HIGH;
    } else {
      AmberledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(AmberledPin, AmberledState);
  }
  
 unsigned long offset_time = millis();




  //put the flashing orange light here so it is always on. 
  
if ( stage == -2) {
  Serial.println("stage-2");
  Serial.println (stop1 + time_at_end_of_dropstage );

        if (millis()< stop1 + time_at_end_of_dropstage ) { //timeatendofdropstage is so the code runs more than once
            //start rotating the robot
             myMotor->run(BACKWARD); 
             myMotor2->run(BACKWARD);
             leftmotor(125);
             rightmotor(125);
    
             length_of_time = (turn_time*1000)-(stop1 + time_at_end_of_dropstage - millis());
        //     Serial.println(length_of_time);
            //take readings from the ultrasound 
             digitalWrite(trig, LOW); 
             delayMicroseconds(20); 
             digitalWrite(trig, HIGH); 
             delayMicroseconds(20);
             digitalWrite(trig, LOW);
             lecture_echo = pulseIn(echo, HIGH); 
             cm = lecture_echo / 58; 
             current_angle = (length_of_time * desired_angle) /(turn_time *1000) ;
//              Serial.print(length_of_time);
//              Serial.print(" ");
//              Serial.print(current_angle);
//              Serial.print(" ");
              Serial.println(cm);
              
            if (cm!=0){  // prevents any 0 readings from the ultrasound sensor being considered
             if (cm<min_cm) {
              highest_angle = current_angle;
              min_cm = cm;
              Serial.print(min_cm);
              Serial.print("   -   ");
              Serial.println(highest_angle);
             }  }
        } else {
             myMotor->run(FORWARD);
             myMotor2->run(FORWARD);
             leftmotor(0);
             rightmotor(0); //stops motor 
             //sending the robot to the next stage 
             stage = -1;

          }
}


if (stage == -1) { 

       Serial.println("stage-1");
                 Serial.println(highest_angle);
                 //printing was to help us debug

         
         if (millis() < 2*stop1 + time_at_end_of_scanstage) { // stop 1 is time it takes to rotate, we x2 because it already rotated once (and added in the time offset from previous stage)
             myMotor->run(FORWARD);
             myMotor2->run(FORWARD);
             leftmotor(125);
             rightmotor(125); //both motors forwards mean it will rotate
             }
    
    
          else {
            myMotor->run(BACKWARD);
             myMotor2->run(BACKWARD);
             leftmotor(0);
             rightmotor(0); //backward+speed=0 was the most effective way to brake wheels quickly
    
             stophighest = (highest_angle / desired_angle) * stop1 ; //time to rotate a certain required angle
             
             time_at_end_of_stage0 = millis(); //for continuity
             
             stage = 0;
            
    
          }
          
          }
          if (millis()>4.5*60*1000){
            delay(60*1000); //THIS WILL SHUT DOWN THE ROTATION IF TIME IS UP
          }
          



 
 if (stage == 0)  {
  Serial.println("stage 0");
  

    if (millis()< stophighest*0.9 + time_at_end_of_stage0) { //lets the robot rotate for the time corresponding to the angle of the closest mine
        
         
         myMotor->run(BACKWARD);
         myMotor2->run(BACKWARD);
         leftmotor(125);
         rightmotor(125);
       
    
   

      } else {
         myMotor->run(FORWARD);
         myMotor2->run(FORWARD);
         myMotor->setSpeed(0);
         myMotor2->setSpeed(0);
         
         //redefine variables and set the robot to the next stage 
         stophighest = stop1 - stophighest;
         time_at_end_of_stage1 = millis();
         
         stage = 1 ;
         
      }


} else if (stage ==1)  { 
  
      Serial.println("stage 1");
      delay (1000);
      origin = analogRead(sensorpin2); //we planned to have the back sensor read an original value to return to
      Serial.println(origin);
      stage = 2;
      

} else if (stage ==2) {
  
    Serial.println("stage 2");
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD); //moves robot forward towards the mine 
    leftmotor(150);
    rightmotor(150);
    int val = analogRead(sensorpin1);       // reads the value of the sharp sensor
    int secondVal = analogRead(sensorpin2);
    Serial.print("Val:  ");
    Serial.print(val);            // prints the value of the sensor to the serial monitor
    Serial.print(" ");
    Serial.println(origin);
    if (val<400){ //means that there is a mine
      myMotor->run(BACKWARD);
      myMotor2->run(FORWARD);
      leftmotor(0); //casues robot to stop
      rightmotor(0);
      AmberledState = HIGH; //turns on amber light 
      myMotor->run(BACKWARD); //sends robot backwards to allow hall effect sensor to read 
      myMotor2->run(FORWARD);
      leftmotor(50);
      rightmotor(50);
      delay (950);
      myMotor->run(BACKWARD); //stops robot 
      myMotor2->run(FORWARD);
      leftmotor(0);
      rightmotor(0);
      delay(1000);
  
      int hallValue = analogRead(hallsensor); //reading hall effect 
      if (hallValue < 900) {
  // LED STUFF OCCURS HERE AND SET A VALUE TO ENSURE IT KNOWS ITS LIVE
        digitalWrite( RedledPin, HIGH);
        liveMine = true;
        delay(5000);
        digitalWrite( RedledPin, LOW);
      }
      else {
        liveMine = false;
  // LED STUFF OCCURS HERE AND SET A VALUE TO ENSURE IT KNOWS ITS NOT LIVE
        digitalWrite( GreenledPin, HIGH);
        delay(5000);
        digitalWrite( GreenledPin, LOW);  
      }
      myMotor->run(BACKWARD); // move robot to allow claw to grab the mine 
      myMotor2->run(FORWARD);
      leftmotor(50);
      rightmotor(50);
      delay (400);
      myMotor->run(BACKWARD);//stop robot
      myMotor2->run(FORWARD);
      leftmotor(0);
      rightmotor(0);
      delay(500);
      clawMotor->run(FORWARD); //pick up sequence 
      clawMotor->setSpeed(100);
      delay(5000); 
      clawMotor->run(RELEASE);
      if (liveMine = true){
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
           previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
            if (RedledState == LOW) {
                RedledState = HIGH;
            } else {
                RedledState = LOW;
            }

    // set the LED with the ledState of the variable:
    digitalWrite(RedledPin, AmberledState);
  }
      }

      while (secondVal < 450) { //keep robot moving back until the reading on the back ir sensor is more than 450 
      unsigned long newTime = millis();
      Serial.println(secondVal);
      myMotor->run(BACKWARD);
      myMotor2->run(FORWARD);
      if (secondVal>450){
              leftmotor(0);
      rightmotor(0);
      }
      leftmotor(150);
      rightmotor(150);
      if ((newTime-millis())>10000){
        break;
      }
  //    Serial.println("True");
      }
   //   else {
      myMotor->run(RELEASE);
      myMotor2->run(RELEASE);
      //redefine values and send to next stage 
      time_at_end_of_stage2 = millis();
      stage = 3;
      
  
   // }
}
}

  
    
 else if (stage ==3)  { 
  Serial.println("stage 3");
    
    
     if (millis() < (stophighest + time_at_end_of_stage2 )) { //i.e rotate back to start orientation
         
           myMotor->run(BACKWARD);
           myMotor2->run(BACKWARD);
           leftmotor(125);
           rightmotor(125);
           
         
       
       
     
  
        } else {
           myMotor->run(BACKWARD);
           myMotor2->run(BACKWARD);
           leftmotor(0);
           rightmotor(0); //brake
           time_at_end_of_stage3 = millis();
           if (liveMine == true) {
              stage = 4; //jumps to sequence for when mine is live
           }else{
              stage = 5; //jumps to sequence for when mine is dummy
           }
            
           }
          }  

 else if (stage == 4)  { 
  
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD); // move forwards
    leftmotor(150);
    rightmotor(150);
    delay(4000); //the set time it takes to get to the red square
    myMotor->run(BACKWARD);
    myMotor2->run(FORWARD);
    //rotate 90
    if (millis()< stop1 + time_at_end_of_stage3) { //rotating to drop mine in red box
         myMotor->run(BACKWARD);
         myMotor2->run(BACKWARD);
         leftmotor(125);
         rightmotor(125);
    } else{
        myMotor->run(BACKWARD); //braking
        myMotor2->run(BACKWARD);
        leftmotor(0);
        rightmotor(0);
    }
    clawMotor->run(BACKWARD);//releasing cube from claw
    clawMotor->setSpeed(128);
    delay(1500); //time it takes to release cube
    clawMotor->run(RELEASE);
    leftmotor(0); //stopping claw
    rightmotor(0);
    //put stuff 
if (millis()< 2*stop1 + time_at_end_of_stage3+1500) { //1500 to account for claw delay
         myMotor->run(FORWARD);
         myMotor2->run(FORWARD);
         leftmotor(125);
         rightmotor(125);
    } else{
        myMotor->run(BACKWARD);
        myMotor2->run(BACKWARD);
        leftmotor(0);
        rightmotor(0);
    }
    myMotor->run(BACKWARD);
    myMotor2->run(FORWARD);
    leftmotor(150);
    rightmotor(150);
    delay(4000); //the set time it takes to get to the red square
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD);
    stage = -2 ;
    time_at_end_of_dropstage = millis();
}

 else if (stage == 5)  { 
  
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD);
    leftmotor(150);
    rightmotor(150);
    delay(2500); //the set time it takes to get to the green square
    myMotor->run(BACKWARD);
    myMotor2->run(FORWARD);
    //rotate 90
    if (millis()< stop1 + time_at_end_of_stage3) {//rotating to drop mine in green box
         myMotor->run(BACKWARD);
         myMotor2->run(BACKWARD);
         leftmotor(125);
         rightmotor(125);
    } else{
        leftmotor(0);
        rightmotor(0);
    }
    clawMotor->run(BACKWARD);//releasing cube from claw
    clawMotor->setSpeed(128);
    delay(1500); //time it takes to release cube
    clawMotor->run(RELEASE);
    leftmotor(0); //stopping claw
    rightmotor(0);
    //put stuff
    if (millis()< 2*stop1 + time_at_end_of_stage3+1500) {//1500 to account for claw delay
         myMotor->run(FORWARD);
         myMotor2->run(FORWARD);
         leftmotor(125);
         rightmotor(125);
    } else{
        myMotor->run(BACKWARD);
        myMotor2->run(BACKWARD);
        leftmotor(0);
        rightmotor(0);
    }
    myMotor->run(BACKWARD);
    myMotor2->run(FORWARD);
    leftmotor(150);
    rightmotor(150);
    delay(2500); //the set time it takes to get to the green square
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD);
    stage = -2 ;
    if (millis()>4.5*60*1000) {
        time_at_end_of_dropstage = 0;
    }
    else {time_at_end_of_dropstage = millis();
    }
    
    
}
    
    
  
       
  
      
}
