#include <Wire.h>
#include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
const int s0 = 8;  
const int s1 = 9;  
const int s2 = 12;  
const int s3 = 11;  
const int out = 10;   
// Variables  
int red = 0;  
int green = 0;  
int blue = 0;  
    void setup()   
{  
  Serial.begin(9600); 
  lcd.begin(16, 2);
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
 digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);  
}  
    void loop() 
{  
   lcd.setCursor(0, 0);
  lcd.print(" Color Detected "); 
  color(); 
  Serial.print("R Intensity:");  
  Serial.print(red, DEC);  
  Serial.print(" G Intensity: ");  
  Serial.print(green, DEC);  
  Serial.print(" B Intensity : ");  
  Serial.print(blue, DEC);  
   if (green - blue >= 10 && green - blue <= 25 && green - ( 2 * red ) >= 8 )
  {  
   Serial.println(" - (Red Color)");
    lcd.setCursor(0, 1);  
   lcd.print("    RED Color   ");
   delay(500);
  }  
  else if ((red > green &&  blue < green) && blue < 25 && red > 40) 
  {  
   Serial.println(" - (Blue Color)"); 
   lcd.setCursor(0, 1);
   lcd.print("   Blue Color   ") ;
   delay(500);
  }  
  else if (green < red && green < blue && green < 25)
  {  
   Serial.println(" - (Green Color)"); 
   lcd.setCursor(0, 1); 
   lcd.print("   Green Color   ");
   delay(500);
   }  
    else{
  Serial.println();
  lcd.setCursor(0, 1); 
  lcd.print(" Can't Identify"); 
  delay (500); 
  }   
 }  
    void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
   red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
    blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
    green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}
