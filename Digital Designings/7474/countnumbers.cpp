#include<Arduino.h>
int d_1=2,d_2=3,d_3=4,d_4=5,q_1=6,q_2=7,q_3=8,q_4=9,clock_pin=13;
void setup()
{	
	pinMode(d_1,OUTPUT);
	pinMode(d_2,OUTPUT);
	pinMode(d_3,OUTPUT);
	pinMode(d_4,OUTPUT);
	pinMode(clock_pin,OUTPUT);
	pinMode(q_1,INPUT);
	pinMode(q_2,INPUT);
	pinMode(q_3,INPUT);
	pinMode(q_4,INPUT);
	Serial.begin(9600);
}
void loop()
{
	digitalWrite(clock_pin,HIGH);
	bool a=digitalRead(q_4);
	bool b=digitalRead(q_3);
	bool c=digitalRead(q_2);
	bool d=digitalRead(q_1);

	bool D=(!d);
	bool C=((c&&(!d))||((!a)&&(!c)&&(d)));
	bool B=((b&&(!d))||(b&&(!c))||((!b)&&c&&d));
	bool A=((a&&(!d))||(b&&c&&d));
	Serial.println(a);
	Serial.println(b);
	Serial.println(c);
	Serial.println(d);
	Serial.println("\n");
	Serial.println(A);
	Serial.println(B);
	Serial.println(C);
	Serial.println(D);
	digitalWrite(d_1,D);
	digitalWrite(d_2,C);
	digitalWrite(d_3,B);
	digitalWrite(d_4,A);
	digitalWrite(clock_pin,LOW);
	delay(1000);
}

