#include <Adafruit_NeoPixel.h>
#include <MSE2202_Lib.h>

#define BUTTON 0

#define BASE_ARM  42
#define FRONT_ARM 41
#define TAIL      40

#define F_TRIG_PIN 2
#define F_ECHO_PIN 39
#define B_TRIG_PIN 10
#define B_ECHO_PIN 9

#define MOTOR_LA 35
#define MOTOR_LB 36
#define MOTOR_RA 37
#define MOTOR_RB 38

#define FMOTOR_LA 5
#define FMOTOR_LB 4
#define FMOTOR_RA 6
#define FMOTOR_RB 7

#define SMART_LED       21
#define SMART_LED_COUNT 1

Motion Bot = Motion();

int mode = 0;
const int LEDBrightnessLevel = 200;

int baseStart = 2000;
int baseMid = 1325;
int baseFinal = 750;

int frontStart = 2000;
int frontMid = 1325;
int frontFinal = 1325;

int tailStart = 300;
int tailFinal = 2150;

long fduration;
long fdistance;
long bduration;
long bdistance;

Adafruit_NeoPixel SmartLEDs(SMART_LED_COUNT, SMART_LED, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  Bot.driveBegin("D1", MOTOR_LA, MOTOR_LB, MOTOR_RA, MOTOR_RB);
  Bot.servoBegin("S1", BASE_ARM);
  Bot.servoBegin("S2", FRONT_ARM);
  Bot.servoBegin("S3", TAIL);
  
  SmartLEDs.begin();
  SmartLEDs.setBrightness(LEDBrightnessLevel);
  SmartLEDs.show();

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(F_TRIG_PIN, OUTPUT);
  pinMode(F_ECHO_PIN, INPUT);
  pinMode(B_TRIG_PIN, OUTPUT);
  pinMode(B_ECHO_PIN, INPUT);
  pinMode(FMOTOR_LA,OUTPUT);
  pinMode(FMOTOR_LB,OUTPUT);
  pinMode(FMOTOR_RA,OUTPUT);
  pinMode(FMOTOR_RB,OUTPUT);
}

void loop() {
  if (digitalRead(BUTTON) == LOW) 
  {
    delay(50);
    mode = 1;
  }

  switch(mode)
  {
    case 0:
    {
      SmartLEDs.setPixelColor(0,255,0,0);
      SmartLEDs.show();

      digitalWrite(F_TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(F_TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(F_TRIG_PIN, LOW);
 
      fduration = pulseIn(F_ECHO_PIN, HIGH);
      fdistance = fduration / 58.2;

      Serial.print("Front Distance: ");
      Serial.print(fdistance);
      Serial.print(" cm");

      digitalWrite(B_TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(B_TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(B_TRIG_PIN, LOW);
 
      bduration = pulseIn(B_ECHO_PIN, HIGH);
      bdistance = bduration / 58.2;

      Serial.print("      Back Distance: ");
      Serial.print(bdistance);
      Serial.println(" cm");

      delay(100);

      Bot.Stop("D1");
      digitalWrite(FMOTOR_LA,LOW);
      digitalWrite(FMOTOR_LB,LOW);
      digitalWrite(FMOTOR_RA,LOW);
      digitalWrite(FMOTOR_RB,LOW);
      break;
    }
    
    case 1:
    {
      SmartLEDs.setPixelColor(0,0,255,0);
      SmartLEDs.show();

      Bot.ToPosition("S1", baseStart);
      Bot.ToPosition("S2", frontStart);
      Bot.ToPosition("S3", tailStart);

      Bot.Forward("D1", 220);

      digitalWrite(F_TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(F_TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(F_TRIG_PIN, LOW);
 
      fduration = pulseIn(F_ECHO_PIN, HIGH);
      fdistance = fduration / 58.2;

      delay(50);

      if(fdistance > 6)
      {
        Bot.Reverse("D1", 255);
        delay(100);
        Bot.Stop("D1");
        delay(2000);

        for(int i = baseStart; i >= baseMid; i -= 25){
          Bot.ToPosition("S1", i);
          Bot.ToPosition("S2", i);
          delay(50);
        }      

        for(int i = baseMid; i >= baseFinal; i -= 25){
          Bot.ToPosition("S1", i);
          delay(50);
        }
        delay(2000);

        for(int i = tailStart; i <= tailFinal; i += 25){
          Bot.ToPosition("S3", i);
          delay(50);
        }
        delay(4000);

        digitalWrite(FMOTOR_LA,LOW);
        digitalWrite(FMOTOR_LB,HIGH);
        digitalWrite(FMOTOR_RA,LOW);
        digitalWrite(FMOTOR_RB,HIGH);

        digitalWrite(F_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(F_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(F_TRIG_PIN, LOW);
 
        fduration = pulseIn(F_ECHO_PIN, HIGH);
        fdistance = fduration / 58.2;

        while(fdistance > 5)
        {
          digitalWrite(F_TRIG_PIN, LOW);
          delayMicroseconds(2);
          digitalWrite(F_TRIG_PIN, HIGH);
          delayMicroseconds(10);
          digitalWrite(F_TRIG_PIN, LOW);
 
          fduration = pulseIn(F_ECHO_PIN, HIGH);
          fdistance = fduration / 58.2;

          Serial.print("Front Distance: ");
          Serial.print(fdistance);
          Serial.println(" cm");
        }

        mode = 2;
      }
      
      break;
    }

    case 2:
    {
      SmartLEDs.setPixelColor(0,0,0,255);
      SmartLEDs.show();

      Bot.Forward("D1", 255);
      digitalWrite(FMOTOR_LA,LOW);
      digitalWrite(FMOTOR_LB,HIGH);
      digitalWrite(FMOTOR_RA,LOW);
      digitalWrite(FMOTOR_RB,HIGH);

      digitalWrite(B_TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(B_TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(B_TRIG_PIN, LOW);
 
      bduration = pulseIn(B_ECHO_PIN, HIGH);
      bdistance = bduration / 58.2;

      while(bdistance > 5)
      {
        Bot.Forward("D1", 255);
        digitalWrite(FMOTOR_LA,LOW);
        digitalWrite(FMOTOR_LB,HIGH);
        digitalWrite(FMOTOR_RA,LOW);
        digitalWrite(FMOTOR_RB,HIGH);        
    
        digitalWrite(B_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(B_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(B_TRIG_PIN, LOW);
 
        bduration = pulseIn(B_ECHO_PIN, HIGH);
        bdistance = bduration / 58.2;

        Serial.print("Back Distance: ");
        Serial.print(bdistance);
        Serial.println(" cm");
      }

      Bot.Stop("D1");
      digitalWrite(FMOTOR_LA,LOW);
      digitalWrite(FMOTOR_LB,LOW);
      digitalWrite(FMOTOR_RA,LOW);
      digitalWrite(FMOTOR_RB,LOW);
      
      for(int i = tailFinal; i >= tailStart; i -= 25){
        Bot.ToPosition("S3", i);
        delay(50);
      }
      delay(2000);

      for(int i = frontMid; i <= frontStart; i += 25){
        Bot.ToPosition("S2", i);
        delay(50);
      }      

      for(int i = baseFinal; i <= baseStart; i += 25){
        Bot.ToPosition("S1", i);
        delay(50);
      }
      delay(2000);

      Bot.Forward("D1", 220);

      digitalWrite(F_TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(F_TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(F_TRIG_PIN, LOW);
 
      fduration = pulseIn(F_ECHO_PIN, HIGH);
      fdistance = fduration / 58.2;

      while(fdistance < 7)
      {
        digitalWrite(F_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(F_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(F_TRIG_PIN, LOW);
 
        fduration = pulseIn(F_ECHO_PIN, HIGH);
        fdistance = fduration / 58.2;
      }

      mode = 3;
      break;
    }

    case 3:
    {
      SmartLEDs.setPixelColor(0,255,255,255);
      SmartLEDs.show();

      Bot.Reverse("D1",255);
      delay(3000);
      Bot.Stop("D1");
      delay(500);
      
      mode = 4;
      break;
    }

    case 4:
    {
      digitalWrite(F_TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(F_TRIG_PIN, HIGH);
      delayMicroseconds(10);
      digitalWrite(F_TRIG_PIN, LOW);
 
      fduration = pulseIn(F_ECHO_PIN, HIGH);
      fdistance = fduration / 58.2;

      while(fdistance < 6)
      {
        SmartLEDs.setPixelColor(0,255,255,255);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,255,0,0);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,157,5,5);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,0,155,55);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,25,5,255);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,2,255,255);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,255,255,255);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,130,130,13);
        SmartLEDs.show();
        delay(100);
        SmartLEDs.setPixelColor(0,67,200,56);
        SmartLEDs.show();
        delay(100);

        digitalWrite(F_TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(F_TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(F_TRIG_PIN, LOW);
 
        fduration = pulseIn(F_ECHO_PIN, HIGH);
        fdistance = fduration / 58.2;
      }

      mode = 0;
      break;
    }

  }
}