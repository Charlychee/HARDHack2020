#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Servo.h>

// Setting variables
int resistPin1 = 1;
int resistPin2 = 2;
int servoPin = 3;

// Creating Motorshield and Motors and Servo
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *FL = AFMS.getMotor(1);
Adafruit_DCMotor *FR = AFMS.getMotor(2);
Adafruit_DCMotor *BR = AFMS.getMotor(3);
Adafruit_DCMotor *BL = AFMS.getMotor(4);

Servo servo;
int servopos = 0;

// Creating defined speeds for motors
int FLSp = 50, FRSp = 50, BRSp = 50, BLSp = 50;
const int left = 0;
const int right = 1;

// Blink w/o Delay
unsigned long previousMillis = 0;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200); // Set up communications
  AFMS.begin(); // Start motor

  //Servo
  servo.attach(servoPin);

  // Encoders
  pinMode(resistPin1, INPUT);
  pinMode(resistPin2, INPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    SpinRight();
    RunMotors();
  }
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
  FLSp = 50;
  FRSp = -50; 
  BRSp = -50; 
  BLSp = 50; 
}

void SpinLeft()
{
  FLSp = -50;
  FRSp = 50;
  BRSp = 50;
  BLSp = -50;
}

void Forward()
{
  FLSp = 50;
  FRSp = 50;
  BRSp = 50;
  BLSp = 50;
  flag = 0;
}

void Backwards()
{
  FLSp = -50;
  FRSp = -50;
  BRSp = -50;
  BLSp = -50;
  flag = 1;
}

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
