#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Servo.h>

// Setting variables
const int resistorPinR = A0; //SET
const int resistorPinL = A1; //SET
const int laserPinL = 9; //SET
const int laserPinR = 10; //SET
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
int FLSp = 50, FRSp = 50, BRSp = 50, BLSp = 50;
const int left = 0;
const int right = 1;

// Encoders
int leftCounter, rightCounter;
int flagL, flagR;

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
  pinMode(laserPinL, OUTPUT);
  pinMode(laserPinR, OUTPUT);
 // pinMode(resistorPinL, INPUT);
 // pinMode(resistorPinR, INPUT);

  digitalWrite(laserPinL, HIGH);
  digitalWrite(laserPinR, HIGH);

  // US Distance Sensors
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  readEncoders();
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

void readEncoders()
{
  if(flagL == 0 && analogRead(resistorPinL) < 600)
  {
    Serial.println("Left Blocked!");
    flagL = 1;
  }
  else if (flagL == 1 && analogRead(resistorPinL) > 700)
  {
    Serial.println("Left Open!");
    flagL = 0;
  }

  if(analogRead(flagR == 0 && resistorPinR) < 600)
  {
    Serial.println("Right Blocked!");
    flagR = 1;
  }
  else if (flagR == 1 && analogRead(resistorPinR) > 700)
  {
    Serial.println("Right Open!");
    flagR = 0;
  }
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
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
}
