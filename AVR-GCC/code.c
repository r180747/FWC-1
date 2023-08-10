#include <LiquidCrystal.h>
LiquidCrystal lcd(7,8,9,10,11,12);
void setup() {
  lcd.begin(16,2);
}
void loop() {
  int a[32],i;
  float v,v1;
  a[0]=1;
  a[1]=0;
  a[2]=1;
  v=a[0]+a[2]+2*a[1];
  v1=v/4;
 lcd.setCursor(0,0);
  
  i=(v1*1000)/50;
 lcd.print(i);
  lcd.print("mA");
}
