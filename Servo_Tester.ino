#include <Adafruit_NeoPixel.h>
#include <MSE2202_Lib.h>
 
int minPos = 300;
int maxPos = 2150;
unsigned int servo;
Motion Bot = Motion();
void setup() {
  Serial.begin(9600);
  Bot.servoBegin("S1", 42);
 
}
 
void loop() {
  servo = map(analogRead(1), 0, 4096, minPos, maxPos);
  Bot.ToPosition("S1", servo);
  Serial.print(F("Servo : Pot R1 = "));
  Serial.print(analogRead(1));
  Serial.print(F(", mapped = "));
  Serial.println(servo);
 
}