/**
 *@author : BLIN Sébastien
 *@date : April 2014
 *@brief : GLaDOs demonstration for hackaday Sci-Fi contest
 */

#include <Servo.h> 

Servo servo1;
Servo servo2;
Servo servoCenter;

int pos = 0;

int posServo1 = 180;
int posServo2 = 0;
int posServoCenter = 0;

int pinServo1 = 7;
int pinServo2 = 8;
int pinServoCenter = 11;

String newValue = "";
char carlu = 0; 

int REDPin = 4;    // RED pin of the LED to PWM pin 4
int GREENPin = 5;  // GREEN pin of the LED to PWM pin 5
int BLUEPin = 6;   // BLUE pin of the LED to PWM pin 6

void dance()
{
  for(int i = posServo2; i > 0; --i)
  {
    servo2.write(i);
    delay(5); 
  }
  for(int i = 0; i <= 180; ++i)
  {
    servo2.write(i);
    if(i % 9 == 0)
      servo1.write(i/9);
    delay(10);
  }
  servoCenter.write(60);
  for(int i = 180; i >= 0; --i)
  {
    servo2.write(i);
    if(i % 9 == 0)
      servo1.write(i/9);
    delay(10);
  }
  servoCenter.write(0);
  posServo1 = 150;
  posServo2 = 0;
  posServoCenter = 0;
}

void talk()
{
  servoCenter.write(40);
  servo1.write(20);
  delay(200);
  servoCenter.write(0);
  servo1.write(0);
  delay(200);
  servoCenter.write(30);
  servo1.write(30);
  delay(500);
  servoCenter.write(40);
  servo1.write(20);
  servo2.write(140);
  delay(200);
  servoCenter.write(0);
  servo1.write(0);
  servo2.write(180);
  posServo1 = 0;
  posServo2 = 0;
  posServoCenter = 0;
}

void clignoter()
{
  for(int i = 1; i < 30 ; ++i)
  {
    analogWrite(REDPin, i);
    analogWrite(BLUEPin, 0);
    analogWrite(GREENPin, 0);
    delay(10);
  }
  for(int i = 1; i < 30 ; ++i)
  {
    analogWrite(REDPin, 0);
    analogWrite(BLUEPin, i);
    analogWrite(GREENPin, 0);
    delay(100);
  }
  for(int i = 1; i < 30 ; ++i)
  {
    analogWrite(REDPin, 0);
    analogWrite(BLUEPin, i);
    analogWrite(GREENPin, 0);
    delay(100);
  }
  for(int i = 1; i < 180 ; ++i)
  {
    analogWrite(REDPin, 0);
    analogWrite(BLUEPin, i);
    analogWrite(GREENPin, 0);
    delay(100);
  }
  analogWrite(GREENPin, 10);
}

void stopAll()
{  
  posServo1 = 0;
  posServo2 = 180;
  posServoCenter = 0;
  servo1.write(posServo1);
  servo2.write(posServo2);
  servoCenter.write(posServoCenter);
  analogWrite(REDPin, 255);
  analogWrite(BLUEPin, 0);
  analogWrite(GREENPin, 10);
}

void setup() 
{ 
  
  pinMode(REDPin, OUTPUT);
  pinMode(GREENPin, OUTPUT);
  pinMode(BLUEPin, OUTPUT);
  
  servo1.attach(pinServo1);
  servo2.attach(pinServo2); 
  servoCenter.attach(pinServoCenter);
  
  servo2.write(posServo1);
  servo1.write(posServo2);
  servoCenter.write(posServoCenter);
  Serial.begin(9600);
  Serial.println(" RORI - GLaDOs 1.0");
} 

void loop()
{  
  if(Serial.available()) 
  {
    char ch = Serial.read();
    String number = "0123456789";
    if(number.indexOf(ch) != -1)
    {    
      pos = pos * 10 + (int(ch)-48);
    }
    else if(ch == 'a')
    {
      servo1.write(pos); 
      posServo1 = pos;
      pos = 0;
    }      
    else if(ch == 'c') 
    {
      servo2.write(pos);    
      posServo2= pos;   
      pos = 0;    
    }
    else if(ch == 'e') 
    {
      servoCenter.write(pos);
      posServoCenter = pos;
      pos = 0;
    }
    else if(ch == 's') 
    {
      stopAll();
      pos = 0;
    }
    else if(ch == 't') 
    {
      talk();
      pos = 0;
    }
    else if(ch == 'd') 
    {
      dance();
      pos = 0;
    }
    else if(ch == 'r') 
    {
      analogWrite(REDPin, pos);
      pos = 0;
    }
    else if(ch == 'g') 
    {
      analogWrite(GREENPin, pos);
      pos = 0;
    }
    else if(ch == 'b') 
    {
      analogWrite(BLUEPin, pos);
      pos = 0;
    }
    else if(ch == 'z') 
    {
      clignoter();
    }
    else if(ch == '+')
    {
      ch = Serial.read();
      if(ch == 'a')
      {
        if(posServo1 >= 180)
            posServo1 += 1;
        servo1.write(posServo1);
      }
      if(ch == 'c')
      {
        if(posServo2 >= 180)
            posServo2 += 1;
        servo2.write(posServo2);
      }
      if(ch == 'e')
      {
        if(posServoCenter >= 180)
            posServoCenter += 1;
        servoCenter.write(posServoCenter);
      }
      pos = 0;
    }
    else if(ch == '-')
    {
      ch = Serial.read();
      if(ch == 'a')
      {
        if(posServo1 <= 0)
            posServo1 -= 1;
        servo1.write(posServo1);
      }
      if(ch == 'c')
      {
        if(posServo2 <= 0)
            posServo2 -= 1;
        servo2.write(posServo2);
      }
      if(ch == 'e')
      {
        if(posServoCenter <= 0)
            posServoCenter -= 1;
        servoCenter.write(posServoCenter);
      }
      pos = 0;
    }
  }
} 
