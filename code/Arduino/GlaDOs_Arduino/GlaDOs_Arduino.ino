/**
 *@author : BLIN Sébastien
 *@date : 2013
 *@brief : programm for GlaDOs
 */

#include <Servo.h> 

Servo servo1Tete;
Servo servo2Tete;
Servo servoCentre;


int posServo1 = 0;
int posServo2 = 0;
int posServoCentre = 0;
int temps = 0;
String newValue = "";
char carlu = 0; 
boolean detect = false;
boolean droite = false;
boolean initialise = false;
boolean modeDanse = false;

int REDPin = 4;    // RED pin of the LED to PWM pin 4
int GREENPin = 5;  // GREEN pin of the LED to PWM pin 5
int BLUEPin = 6;   // BLUE pin of the LED to PWM pin 6
int brightness = 0; // LED brightness
int increment = 5;  // brightness increment 

int pinMotorBase1 = 9;
int pinMotorBase2 = 10;
int pinServoCenter = 11;

boolean haut = false;
boolean bas = false;

void baseGauche()
{
  analogWrite(pinMotorBase1, 0);
  analogWrite(pinMotorBase2, 255);
}

void baseDroite()
{
  analogWrite(pinMotorBase1, 255);
  analogWrite(pinMotorBase2, 0);
}

void baseStop()
{
  analogWrite(pinMotorBase1, 0);
  analogWrite(pinMotorBase2, 0);
}

void droiteGauche()
{
  posServo1 = 20;
  if(posServo2 == 0)
      droite = false;
  if(posServo2 == 180)
      droite = true;
  if(droite)
      --posServo2;
  else
      ++posServo2;
  delay(20);
   
}

void centreHautBas(boolean haut)
{
  if(haut && posServoCentre < 180)
      posServoCentre += 4;
  else if(!haut && posServoCentre > 0)      
      posServoCentre -= 4;
}

void changeColorLed(int R, int G, int B)
{
  analogWrite(REDPin, R); 
  analogWrite(GREENPin, G);
  analogWrite(BLUEPin, B);
}

void danse()
{
  if(temps >= 10)
  {
      servoCentre.write(random(0,180));
      delay(100);
      temps = 0;
  }
  ++temps;
}

void setup() 
{ 
  
  pinMode(REDPin, OUTPUT);
  pinMode(GREENPin, OUTPUT);
  pinMode(BLUEPin, OUTPUT);
  
  servo1Tete.attach(7);
  servo2Tete.attach(8); 
  servoCentre.attach(pinServoCenter);
  
  servo2Tete.write(posServo1);
  servo1Tete.write(posServo2);
  Serial.begin(9600);
  Serial.println(" RORI - GLaDOs 1.0");
} 

void loop()
{  
  /**/
  servo1Tete.write(posServo1);
  servo2Tete.write(posServo2);
  servoCentre.write(posServoCentre);
  int cardispo = 0; //variable contenant le nombre de caractère disponibles dans le buffer
  cardispo = Serial.available();
  if(cardispo > 0) //tant qu'il y a des caractères à lire
  {
    carlu = Serial.read(); //on lit le caractère
    newValue += carlu; //puis on le renvoi à l’expéditeur tel quel    
  } 
  
  if(carlu == 'a' && initialise)
  {
    detect = true;
    if(newValue[0] == '+' && posServo1 < 180)
      ++posServo1;
    else if(newValue[0] == '-' && posServo1 > 0)
      --posServo1;
    newValue = "";
  }
  else if(carlu == 'b' && initialise)
  {
    detect = true;
    if(newValue[0] == '+' && posServo2 < 180)
      ++posServo2;
    else if(newValue[0] == '-' && posServo2 > 0)
      --posServo2;
    else if(newValue[0] == 'g')
       baseGauche();       
    else if(newValue[0] == 'd')
       baseDroite();
    newValue = "";
  }  
  else if(carlu == 'l')
  {
    initialise = true;
    changeColorLed(255,10,0);
  }  
  else if(carlu == 'r' && initialise)
  {
    changeColorLed(255,0,0);
  }  
  else if(carlu == 'v' && initialise)
  {
    changeColorLed(0,255,0);
  }  
  else if(carlu == 'n' && initialise)
  {
    changeColorLed(0,0,255);
  }  
  else if(carlu == 'c' && initialise)
  {
    detect = false;
  }
  else if(carlu == 's' && initialise)
  {
    baseStop(); 
  }
  else if(carlu == 'g' && initialise)
  {
    baseGauche(); 
  }
  else if(carlu == 'd' && initialise)
  {
    baseDroite(); 
  }
  else if(carlu == 'h' && initialise)
  {
    haut = true;
  }
  else if(carlu == 'j' && initialise)
  {
    bas = true;
  }
  else if(carlu == 'k' && initialise)
  {
    haut = false;
    bas = false; 
  }
  else if(carlu == 'z' && initialise)
  {
     modeDanse = modeDanse ? false : true; 
  }
  
  if(haut && initialise)
      centreHautBas(true);
  if(bas && initialise)
      centreHautBas(false);
  
  if(!detect && initialise)
      droiteGauche();
  if(modeDanse && initialise)
      danse();
  /**/
} 
