#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Servo.h>
#include "Gyro.h"

// Setting variables
const int resistorPinR = A1; //SET
const int resistorPinL = A0; //SET
//const int laserPinL = 9; //SET
//const int laserPinR = 10; //SET
const int servoPin = 8; //SET
const int trigPin = 9;
const int echoPin = 10;

// US Distance Sensors
float duration, distance;

// Creating Motorshield and Motors and Servo
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *FL = AFMS.getMotor(1);
Adafruit_DCMotor *FR = AFMS.getMotor(2);
Adafruit_DCMotor *BR = AFMS.getMotor(3);
Adafruit_DCMotor *BL = AFMS.getMotor(4);

Servo servo;
int servopos = 0;

// Creating defined speeds for motors
int baseSpeed = 60;
int FLSp = baseSpeed, FRSp = baseSpeed, BRSp = baseSpeed, BLSp = baseSpeed;


// Encoders
int leftCounter, rightCounter;
int flagL = 0, flagR = 0;
int stopFlagL = 0, stopFlagR = 0;

// Blink w/o Delay
unsigned long previousMillis = 0;

const int left = 0;
const int right = 1;

void setup()
{
  servo.write(50);
  // put your setup code here, to run once:
  Serial.begin(9600); // Set up communications
  AFMS.begin(); // Start motor

  initGyroSensor();
  
  //Servo
  servo.attach(servoPin);

  // Encoders
  //  pinMode(laserPinL, OUTPUT);
  //  pinMode(laserPinR, OUTPUT);
  // pinMode(resistorPinL, INPUT);
  // pinMode(resistorPinR, INPUT);

  //  digitalWrite(laserPinL, HIGH);
  // digitalWrite(laserPinR, HIGH);

  // US Distance Sensors
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


}
void repeatForward(int milliseconds) {
  delay(500);
  double currAmp = 0.0; //get value from raspi;
  double maxAmp = currAmp;
  Forward(milliseconds);
  while (0.9 * maxAmp <= currAmp || 1.3 * maxAmp > currAmp) {
    Forward(milliseconds);
    Serial.flush();
    double currAmp = (double)Serial.parseFloat(); //get value from raspi;
    if (currAmp > maxAmp) {
      maxAmp = currAmp;
    }
  }
}

void loop()
{
  double currentHeading = getGroundHeading();
  findDirection(20.0, 2000);
  double newHeading = getGroundHeading();
  if ((newHeading-40)<currentHeading && currentHeading<(newHeading+40)){
    Backward(200);
    Stop(left);
    Stop(right);
    while(true){}
  } else {
    repeatForward(1000); //1 second
  }
               // put your main code here, to run repeatedly:
               //unsigned long currentMillis = millis();
               // if (currentMillis - previousMillis >= 1000)

}

void pointAnt(int angle)
{
  servo.write(angle);
}

void Circle(int direction)
{
  if (direction == left)
  {
    FLSp = 40;
    FRSp = 75;
    BRSp = 75;
    BLSp = 40;
  }

  if (direction == right)
  {
    FLSp = 75;
    FRSp = 40;
    BRSp = 40;
    BLSp = 75;
  }
}
void SpinRight()
{
  FLSp = baseSpeed;
  FRSp = -baseSpeed;
  BRSp = -baseSpeed;
  BLSp = baseSpeed;
}

void SpinLeft()
{
  FLSp = -baseSpeed;
  FRSp = baseSpeed;
  BRSp = baseSpeed;
  BLSp = -baseSpeed;
}

void Backwards()
{
  FLSp = -baseSpeed;
  FRSp = -baseSpeed;
  BRSp = -baseSpeed;
  BLSp = -baseSpeed;
}

void triggerDistSensor()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
//Turns 1 arg in teeth, positive 1 motor, negative other motor, 1 arg direction of turn
//12 teeth (24 spaces)

//Move forward 1 arg, teeth

int encoderFlag = 0;
int leftSpokeCounter = 0;
int rightSpokeCounter = 0;

void RunMotors()
{
  FL->setSpeed(abs(FLSp));
  FR->setSpeed(abs(FRSp));
  BR->setSpeed(abs(BRSp));
  BL->setSpeed(abs(BLSp));

  if (FLSp > 0)
  {
    FL->run(FORWARD);
  }
  else
  {
    FL->run(BACKWARD);
  }
  if (FRSp > 0)
  {
    FR->run(FORWARD);
  }
  else
  {
    FR->run(BACKWARD);
  }
  if (BRSp > 0)
  {
    BR->run(FORWARD);
  }
  else
  {
    BR->run(BACKWARD);
  }
  if (BLSp > 0)
  {
    BL->run(FORWARD);
  }
  else
  {
    BL->run(BACKWARD);
  }

}

/*void Turn(int turnDirection, int teeth) //Changed readEncoders //Teeth = Every time opens up //Added stop function
  {
  if (turnDirection == left)
  {
    FLSp = -baseSpeed;
    FRSp = baseSpeed;
    BLSp = -baseSpeed;
    BRSp = baseSpeed;
    RunMotors();
  }
  else if (turnDirection == right)
  {
    FLSp = baseSpeed;
    FRSp = -baseSpeed;
    BLSp = baseSpeed;
    BRSp = -baseSpeed;
    RunMotors();
  }
  while(encoderFlag == 0)
  {
    readEncoders(teeth);
  }
  }*/
void Turn(double goalAngle)
{
  double currAngle = getGroundHeading();
  if (goalAngle < currAngle) {
    turnLeft(goalAngle);
  } else {
    turnRight(goalAngle);
  }
}
void turnLeft(double degCC) {
  double currAngle = getGroundHeading();
  while (currAngle > degCC) {
    FLSp = -baseSpeed;
    FRSp = baseSpeed;
    BLSp = -baseSpeed;
    BRSp = baseSpeed;
    RunMotors();
    currAngle = getGroundHeading();
  }
  Stop(left);
  Stop(right);
}
void turnRight(double degClockwise) {
  double currAngle = getGroundHeading();
  while (currAngle < degClockwise) {
    FLSp = baseSpeed;
    FRSp = -baseSpeed;
    BLSp = baseSpeed;
    BRSp = -baseSpeed;
    RunMotors();
    currAngle = getGroundHeading();
    Serial.println(currAngle);
  }
  Stop(left);
  Stop(right);
}
void findDirection(double outerAngleCW, int milliseconds) {
  delay(500);
  double currAmp = 0.0;//get value from raspi;
  double maxAmp = currAmp;
  Forward(milliseconds);
  double currAngle = getGroundHeading();
  Turn(((int)outerAngleCW + (int)currAngle) % 360);
  while (0.9 * maxAmp <= currAmp) {
    delay(500);
    double currAmp = 0.0; //get value from raspi;
    if (currAmp > maxAmp) {
      maxAmp = currAmp;
    }
    Forward(milliseconds);
    delay(500);
    Turn(((int)outerAngleCW + (int)currAngle) % 360);
  }
  Backward(milliseconds);
  Turn(((int)currAngle + (720 - ((180 - (int)outerAngleCW) / 2) - (int)outerAngleCW)) % 360);
}

void Stop(int turnDirection)
{
  if (turnDirection == left)
  {
    FLSp = 0;
    BLSp = 0;
  }
  else if (turnDirection == right)
  {
    FRSp = 0;
    BRSp = 0;
  }
  RunMotors();
}

void readEncoders(int teeth)
{
  int readLeft = analogRead(resistorPinL);
  int readRight = analogRead(resistorPinR);

  if (flagL == 0 && readLeft < 550)
  {
    //  Serial.println("Left Blocked!");
    flagL = 1;
  }
  else if (flagL == 1 && readLeft > 670)
  {
    //  Serial.println("Left Open!");
    flagL = 0;
    leftSpokeCounter++;
    //  Serial.print("Left: ");
    //  Serial.println(leftSpokeCounter);
  }

  if (flagR == 0 && readRight < 550)
  {
    //   Serial.println("Right Blocked!");
    flagR = 1;
  }
  else if (flagR == 1 && readRight > 670)
  {
    //   Serial.println("Right Open!");
    flagR = 0;
    rightSpokeCounter++;
    //  Serial.print("Right: ");
    //   Serial.println(rightSpokeCounter);
    // }
  }

  if ( leftSpokeCounter == teeth )
  {
    Stop(left);
    stopFlagL = 1;
  }
  if ( rightSpokeCounter == teeth )
  {
    Stop(right);
    stopFlagR = 1;
  }

  if ( (stopFlagL && stopFlagR) == 1 )
  {
    encoderFlag = 0;
    stopFlagL = 0;
    stopFlagR = 0;
    leftSpokeCounter = 0;
    rightSpokeCounter = 0;
  }
  // Serial.println("************************");
  // Serial.println(readLeft);
  // Serial.println(readRight);
}




unsigned long currentMillis;
int timeFlag = 0;

void Forward(int milliseconds)
{
  previousMillis = currentMillis = millis();
  FLSp = baseSpeed;
  FRSp = baseSpeed;
  BRSp = baseSpeed;
  BLSp = baseSpeed;
  RunMotors();
  while (timeFlag == 0) {
    timer(milliseconds);
  }
  timeFlag = 0;
  Stop(left);
  Stop(right);
  delay(1000);
}
void Backward(int milliseconds)
{
  previousMillis = currentMillis = millis();
  FLSp = -baseSpeed;
  FRSp = -baseSpeed;
  BRSp = -baseSpeed;
  BLSp = -baseSpeed;
  RunMotors();
  while (timeFlag == 0) {
    timer(milliseconds);
  }
  timeFlag = 0;
  Stop(left);
  Stop(right);
  delay(1000);
}


void timer(unsigned long interval)
{
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    timeFlag = 1;
  }
  currentMillis = millis();
}
