#include <Arduino.h>
int Y,U,V,W,X;
void setup()
{
	pinMode(13, OUTPUT);
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
}
void loop(){
 	U = digitalRead(2);
	V = digitalRead(3);
	W = digitalRead(4);
	X = digitalRead(5);
	Y = (!U&&V||!V&&U)&&(!W&&!X||W&&!X);
	digitalWrite(13, Y);
	
}
