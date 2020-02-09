#include <ArduinoSTL.h>
#include <vector>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
using namespace std;
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
float time_at_end_of_first_stage;
float time_at_end_of_second_stage;
float time_at_end_of_third_stage;
int sensorpin1 = 0;                 // analog pin used to connect the sharp sensor
int sensorpin2 = 1;                 // analog pin used to connect the sharp sensor
int hallsensor = 3;
int origin;
int i = 0;
bool liveMine;
float min_distance;


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
int trig = 2; 
int echo = 3; 
long lecture_echo; 
long cm;
  std::vector< float > distances;
  std::vector< float > angles;
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
  highest_angle = 1.3;
  stophighest = (highest_angle / desired_angle) * stop1 ;
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
}


void rightmotor(int r_speed) {
  if (r_speed == oldrspeed){
    return;
  }
  else {
    //choose whichever motor is the right one 
    myMotor2->setSpeed(r_speed);
    oldrspeed = r_speed;
  }
}

void leftmotor(int l_speed) {
  if (l_speed == oldlspeed){
    return;
  }
  else {
    //choose whichever motor is the right one 
    myMotor->setSpeed(l_speed);
    oldlspeed = l_speed;
  }
}

void loop() {
  ///breaking down the movement into time to highest then time to clear path by using the angle and finding  ratio of the total stop1
    if (millis()< stop1) {
        
         myMotor->run(FORWARD);
         myMotor2->run(FORWARD);
         leftmotor(125);
         rightmotor(125);

         length_of_time = (turn_time*1000)-(stop1 - millis());
    //     Serial.println(length_of_time);
         digitalWrite(trig, HIGH); 
         delayMicroseconds(10); 
         digitalWrite(trig, LOW); 
         lecture_echo = pulseIn(echo, HIGH); 
         cm = lecture_echo / 58; 
         current_angle = (length_of_time * desired_angle) /(turn_time *1000) ;
         Serial.print(length_of_time);
         Serial.print(" ");
         Serial.println (current_angle);
         if (cm!=0) {
          distances.push_back( cm );
          angles.push_back( current_angle );
         }
      } else {
         myMotor->run(BACKWARD);
         myMotor2->run(BACKWARD);
         leftmotor(0);
         rightmotor(0);
         min_distance = *min_element(distances.begin(), distances.end()); // check if min value is returned
         // PUT IN FIND FUNC
         for (i=0; i<distances.size(); i++){
          if (distances[i] = min_distance) {
            desired_angle = angles[i];
            break;
          }
         }
      }

     
     if (millis() < 2*stop1) {
         myMotor->run(BACKWARD);
         myMotor2->run(BACKWARD);
         leftmotor(125);
         rightmotor(125);
         }


      
        
         

      }
      }
      if (millis()>4.5*60*1000){
        delay(60*1000); //THIS WILL SHUT DOWN THE ROTATION IF TIME IS UP
      }
      
}
