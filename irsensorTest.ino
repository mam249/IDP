#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

int sensorpin1 = 0;                 // analog pin used to connect the sharp sensor
int sensorpin2 = 1;                 // analog pin used to connect the sharp sensor
int val = 0;                        // variable to store the values from sensor(initially zero)
int secondVal = 0;
int origin;
int i = 0;

int trig = 2; 
int echo = 3; 
long lecture_echo; 
long cm;
void setup()
{
  Serial.begin(9600);               // starts the serial monitor
  AFMS.begin();
  delay(1000);
  origin = analogRead(sensorpin2);

}
 
void loop()
{
//  myMotor->run(FORWARD);
//  myMotor->setSpeed(255);
  val = analogRead(sensorpin1);       // reads the value of the sharp sensor
  secondVal = analogRead(sensorpin2);
//  Serial.print(val);            // prints the value of the sensor to the serial monitor
//  Serial.print(" ");
  Serial.println(secondVal);
  delay(1000);

//  digitalWrite(trig, HIGH); 
//             delayMicroseconds(10); 
//             digitalWrite(trig, LOW); 
//             lecture_echo = pulseIn(echo, HIGH); 
//             cm = lecture_echo / 58; 
//             Serial.println(cm);


//  if (val>500) { 
//    myMotor->run(RELEASE);
//
//    delay(1000);
//    
//    Serial.println(origin);
//
//
//    while (secondVal<origin){
//    myMotor->run(BACKWARD);
//    myMotor->setSpeed(255);
////    Serial.println("True");
//    i=i+1;
//    if (i>100000){
//      break;
//    }
//    }
//    myMotor->run(RELEASE);
//
//  }
}