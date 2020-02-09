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
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

long int stop1;
float pi;
float desired_angle;
float rpm;
float wheel_dist;
float wheel_radius;
float turn_time;
float current_angle;
int sensorValue = 0;
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
  stop1=millis()+(turn_time * 1000);  // 3 seconds
  
 Serial.println(turn_time);
}
void loop() {
      
  // put your main code here, to run repeatedly:
//   sensorValue = analogRead(sensorPin);
//   current_angle = (desired_angle * millis() ) / turn_time ;
//   Serial.println(current_angle);
  if (millis()<stop1) {
    
     myMotor->run(FORWARD);
     myMotor2->run(FORWARD);
     myMotor->setSpeed(125);
     myMotor2->setSpeed(125);
     
   

  } else {
     myMotor->run(BACKWARD);
     myMotor2->run(BACKWARD);
     myMotor->setSpeed(0);
     myMotor2->setSpeed(0);
  }

  
}
