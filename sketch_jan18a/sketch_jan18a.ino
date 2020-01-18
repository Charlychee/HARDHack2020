#include <Adafruit_MotorShield.h>
#include <Wire.h>

// Creating Motorshield and Motors
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);
Adafruit_DCMotor *Motor4 = AFMS.getMotor(4);

// Creating defined speeds for motors
int M1Sp = 50, M2Sp = 50, M3Sp = 50, M4Sp = 50;

// Blink w/o Delay
unsigned long previousMillis = 0;

int flag = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Set up communications
  AFMS.begin(); // Start motor
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (flag == 1){
      Forward();
    }
    else {
      Backwards();
    }
    RunMotors();
    /*Backwards();
    RunMotors();
    */
  }
}

void Forward()
{
  M1Sp = 50;
  M2Sp = 50;
  M3Sp = 50;
  M4Sp = 50;
  flag = 0;
}

void Backwards()
{
  M1Sp = -50;
  M2Sp = -50;
  M3Sp = -50;
  M4Sp = -50;
  flag = 1;
}

void RunMotors()
{
  Motor1->setSpeed(abs(M1Sp));
  Motor2->setSpeed(abs(M2Sp));
  Motor3->setSpeed(abs(M3Sp));
  Motor4->setSpeed(abs(M4Sp));

  if (M1Sp > 0)
  {
    Motor1->run(FORWARD);
  }
  else
  {
    Motor1->run(BACKWARD);
  }
  if (M2Sp > 0)
  {
    Motor2->run(FORWARD);
  }
  else
  {
    Motor2->run(BACKWARD);
  }
  if (M3Sp > 0)
  {
    Motor3->run(FORWARD);
  }
  else
  {
    Motor3->run(BACKWARD);
  }
  if (M4Sp > 0)
  {
    Motor4->run(FORWARD);
  }
  else
  {
    Motor4->run(BACKWARD);
  }
  
}
