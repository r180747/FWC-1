#include<Arduino.h>
void disp_7447(int D, int C, int B, int A)
{
  digitalWrite(2, A); 
  digitalWrite(3, B);
  digitalWrite(4, C);
  digitalWrite(5, D); 

}

void setup() {
    pinMode(2, OUTPUT);  
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
}

void loop() {
disp_7447(0,1,1,0);  
}

