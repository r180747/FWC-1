#include<Arduino.h>
int D,C,B,A;
int Z,Y,X,W;

void disp_7447(int D,int C,int B,int A)
{
  digitalWrite(2,A);
  digitalWrite(3,B);
  digitalWrite(4,C);
  digitalWrite(5,D);
}
void setup()
{
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
}
void loop()
{
         A=(!W&&!Z)||(!W&&!X&&!Y);
	 B=(W&&!X&&!Z)||(!W&&X&&!Z);
	 C=(W&&X&&!Y&&!Z)||(!X&&Y&&!Z)||(!W&&Y&&!Z);
	 D=(W&&X&&Y&&!Z)||(!W&&!X&&!Y&&Z);

  disp_7447(D,C,B,A);
}
