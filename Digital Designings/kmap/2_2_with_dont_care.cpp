#include<Arduino.h>
int D,C,B,A;
int a,b,c,d,e,f,g;

void disp_7447(int a,int b,int c,int d,int e,int f,int g)
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
  a=(A&&!B&&!C&&!D)||(!A&&!B&&C);
  b=(A&&!B&&C)||(!A&&B&&C);
  c=(!A&&B&&!C);
  d=(A&&!B&&!C)||(A&&B&&C)||(!A&&!B&&C);
  e=(A)||(!B&&C);
  f=(B&&!C)||(A&&B)||(A&&!C&&!D);
  g=(A&&B&&C)||(!B&&!C&&!D);

  disp_7447(a,b,c,d,e,f,g);
}

 



 


