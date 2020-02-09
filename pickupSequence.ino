#include <Wire.h>
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
Adafruit_DCMotor *clawMotor = AFMS.getMotor(3);

int sensorpin1 = 0;                 // analog pin used to connect the sharp sensor
int sensorpin2 = 1;                 // analog pin used to connect the sharp sensor
int hallsensor = 3;
int origin;
int i = 0;
bool liveMine;


void setup()
{
  Serial.begin(9600);               // starts the serial monitor
  AFMS.begin();
  delay(1000);
  origin = analogRead(sensorpin2);

}
 
void loop()
{
  myMotor1->run(FORWARD);
  myMotor2->run(BACKWARD);
  myMotor1->setSpeed(255);
  myMotor2->setSpeed(255);
  int val = analogRead(sensorpin1);       // reads the value of the sharp sensor
  int secondVal = analogRead(sensorpin2);
  Serial.print(val);            // prints the value of the sensor to the serial monitor
  Serial.print(" ");
  Serial.println(origin);
  if (val>450) { 
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);

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
    delay(500);
    clawMotor->run(RELEASE);
    if (liveMine = true){
      //flashing red
    }
    else {
      //flashing green
    }
    while (secondVal<origin){
    myMotor1->run(BACKWARD);
    myMotor2->run(FORWARD);
    myMotor1->setSpeed(255);
    myMotor2->setSpeed(255);
//    Serial.println("True");
    i=i+1;
    if (i>100000){
      break;
    }
    }
    myMotor1->run(RELEASE);
    myMotor2->run(RELEASE);

  }
}
