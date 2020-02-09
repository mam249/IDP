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



int stage = 0;
float time_at_end_of_stage1;
float time_at_end_of_stage2;
float time_at_end_of_stage3;
int sensorpin1 = 0;                 // analog pin used to connect the sharp sensor
int sensorpin2 = 1;                 // analog pin used to connect the sharp sensor
int hallsensor = 3;
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);           // set up Serial library at 9600 bps
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  pi= 3.141592653589793238;
  wheel_radius=0.049;
  wheel_dist=0.23;
  rpm=19.608;
  desired_angle= pi/2;
  turn_time = (15 * desired_angle * wheel_dist )/ (rpm * pi * wheel_radius);
  stop1=millis()+(turn_time * 1000);
  
  
  
 
  highest_angle = pi/4;
  stophighest = (highest_angle / desired_angle) * stop1 ;
}


void rightmotor(int r_speed) {
  if (r_speed == oldrspeed){
    return;}

  else {
    
    //choose whichever motor is the right one 
    myMotor2->setSpeed(r_speed);
    oldrspeed = r_speed;
    
  }
}

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
  ///breaking down the movement into time to highest then time to clear path by using the angle and finding  ratio of the total stop1
 
 if (stage == 0)  {
  Serial.println("stage 0");
  

    if (millis()< stophighest) {
        
         
         myMotor->run(BACKWARD);
         myMotor2->run(BACKWARD);
         leftmotor(125);
         rightmotor(125);

         length_of_time = (turn_time*1000)-(stop1 - millis());
    //     Serial.println(length_of_time);
         current_angle = (length_of_time * desired_angle) /(turn_time *1000) ;
         Serial.print(length_of_time);
         Serial.print(" ");
         Serial.println (current_angle);
       
     
     
   

      } else {
         myMotor->run(FORWARD);
         myMotor2->run(FORWARD);
         myMotor->setSpeed(0);
         myMotor2->setSpeed(0);
         
         
         stophighest = stop1- stophighest;
         time_at_end_of_stage1 = millis();
         
         stage = 1 ;
         
      }

} else if (stage ==1)  { 
  
      Serial.println("stage 1");
      delay (1000);
      origin = analogRead(sensorpin2);
      Serial.println(origin);
      stage = 2;
      

} else if (stage ==2) {
  
    Serial.println("stage 2");
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD);
    leftmotor(150);
    rightmotor(150);
    int val = analogRead(sensorpin1);       // reads the value of the sharp sensor
    int secondVal = analogRead(sensorpin2);
    Serial.print("Val:  ");
    Serial.print(val);            // prints the value of the sensor to the serial monitor
    Serial.print(" ");
    Serial.println(origin);
    if (val>700) { 
      myMotor->run(BACKWARD);
      myMotor2->run(FORWARD);
      leftmotor(0);
      rightmotor(0);
      delay(1000);
  
      int hallValue = analogRead(hallsensor);
      if (hallValue>100) {
  // LED STUFF OCCURS HERE AND SET A VALUE TO ENSURE IT KNOWS ITS LIVE
        liveMine = true;
        
      }
      else {
        liveMine = false;
  // LED STUFF OCCURS HERE AND SET A VALUE TO ENSURE IT KNOWS ITS NOT LIVE
  
      }
      clawMotor->run(FORWARD);
      clawMotor->setSpeed(128);
      delay(1500);
      clawMotor->run(RELEASE);
      if (liveMine = true){
        //flashing red
      }
      else {
        //flashing green
      }
      while (secondVal>origin) {
      Serial.println(secondVal);
      myMotor->run(BACKWARD);
      myMotor2->run(FORWARD);
      leftmotor(150);
      rightmotor(150);
  //    Serial.println("True");
      i=i+1;
      if (i>15000){
        break;
      }
      }
      myMotor->run(RELEASE);
      myMotor2->run(RELEASE);
      time_at_end_of_stage2 = millis();
      stage = 3;
      
  
    }
  
    
} else if (stage ==3)  { 
  Serial.println("stage 3");
    
    
     if (millis() < (stophighest + time_at_end_of_stage2 )) {
         
           myMotor->run(BACKWARD);
           myMotor2->run(BACKWARD);
           leftmotor(125);
           rightmotor(125);
           
         
       
       
     
  
        } else {
           myMotor->run(BACKWARD);
           myMotor2->run(BACKWARD);
           leftmotor(0);
           rightmotor(0);
           time_at_end_of_stage3 = millis();
           if (liveMine == true) {
              stage = 4;
           }else{
              stage = 5;
           }
            
           }
          }  

} else if (stage == 4)  { 
  
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD);
    leftmotor(150);
    rightmotor(150);
    delay(1500); //change delay after testing on bench 
    myMotor->run(BACKWARD);
    myMotor2->run(FORWARD);
    leftmotor(0);
    rightmotor(0);
}

} else if (stage == 5)  { 
  
    myMotor->run(FORWARD);
    myMotor2->run(BACKWARD);
    leftmotor(150);
    rightmotor(150);
    delay(1500); //change delay after testing on bench 
    myMotor->run(BACKWARD);
    myMotor2->run(FORWARD);
    leftmotor(0);
    rightmotor(0);
}
    
    
  
       
  
      
}
// stage 4 and stage 5 are for different heights along the 
