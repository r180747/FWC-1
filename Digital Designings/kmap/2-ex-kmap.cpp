#include<Arduino.h>
int D,C,B,A;
int a,b,c,d,e,f,g;

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
  a=(A&&!B&&!C&&!D)||(!A&&!B&&C&&!D);
  b=(A&&!B&&C&&!D)||(!A&&B&&C&&!D);
  c=(!A&&B&&!C&&!D);
  d=(A&&!B&&!C&&!D)||(!A&&!B&&C&&!D)||(A&&B&&C&&!D)||(A&&!B&&!C&&D);
  e=(A&&!B&&!C&&!D)||(A&&B&&!C&&!D)||(!A&&!B&&C&&!D)||(A&&!B&&C&&!D)||(A&&B&&C&&!D)||(A&&!B&&!C&&D);
  f=(A&&!B&&!C&&!D)||(!A&&B&&!C&&!D)||(A&&B&&!C&&!D)||(A&&B&&C&&!D);
  g=(!A&&!B&&!C&&!D)||(A&&!B&&!C&&!D)||(A&&B&&C&&!D);

  disp_7447(D,C,B,A);
}

 



 


