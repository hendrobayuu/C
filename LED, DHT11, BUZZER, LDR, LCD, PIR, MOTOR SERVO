//KOMPONEN YANG DIGUNAKAN : LED, DHT11, BUZZER, LDR, LCD, PIR, MOTOR SERVO

#include <Servo.h>
Servo myservo;
#include <LiquidCrystal.h>
#include <DHT.h>
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int calibrationTime = 20;  //?
int pos = 0;  //?
int keadaan = 0;
int pirState = LOW;
long unsigned int lowIn;       //?
long unsigned int pause = 5000;  //?
boolean lockLow = true;  //?
boolean takeLowTime;  //?
int pirPin = 7;
const int ledPin = 6;
int pienzoPin = 9;
const int hijau = A1;   
const int merah = A2;
const int kuning = A3;
const int ldrPin = A0;  

void setup() {
  Serial.begin(9600);
lcd.begin(16,2);
pinMode(ledPin, OUTPUT);
  pinMode(pienzoPin, OUTPUT);
  pinMode(kuning, OUTPUT);
  pinMode(merah, OUTPUT);
  pinMode(hijau, OUTPUT);  //inisiasi LED pin sebagai output
  pinMode(ldrPin, INPUT);   //inisiasi LDR pin sebagai output
myservo.attach(13);
pinMode(pirPin, INPUT);
Serial.println("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(calibrationTime - i);
    Serial.print("-");
    delay(1000);
}
  Serial.println();
  Serial.println("done");
//while (digitalRead(pirPin) == HIGH) {
  //  delay(500);
    //Serial.print(".");     
  }
//  Serial.print("SENSOR ACTIVE");
//}

void loop() 

{ //LDR
int ldrStatus = analogRead(ldrPin);   //read the status of the LDR value
 if (ldrStatus <=400) {
lcd.setCursor(0,0);
   digitalWrite(hijau, HIGH);
  digitalWrite(merah, HIGH);
  digitalWrite(kuning, HIGH);
lcd.print("Lampu Nyala "); // lcd.setCursor(0,0)
 Serial.println("LDR is DARK, LED is ON");
 delay(500);//turn LED on // tambahn lcd.clar()
     }
 else {
lcd.setCursor(0,0);
digitalWrite(hijau,LOW);
 digitalWrite(merah, LOW);
  digitalWrite(kuning, LOW);      //turn LED off
lcd.print("Lampu Mati "); //lcd.setCursor(0,0)
  Serial.println("LED PIN OFF");
  delay(10);// tambahn lcd.clar()
  }
{

keadaan = digitalRead(pirPin);
  if(keadaan == HIGH){  //if the PIR output is HIGH, turn servo

    /*turns servo from 0 to 180 degrees and back
    it does this by increasing the variable "pos" by 1 every 5 milliseconds until it hits 180
    and setting the servo's position in degrees to "pos" every 5 milliseconds
    it then does it in reverse to have it go back
    to learn more about this, google "for loops"
    to change the amount of degrees the servo turns, change the number 180 to the number of degrees you want it to turn
    **/
    for(pos = 0; pos < 180; pos += 1)  //goes from 0 to 180 degrees
    {                                                 //in steps of one degree
      myservo.write(pos);                   //tells servo to go to position in variable "pos"
      delay(5);                                   //waits for the servo to reach the position
    }
    for(pos = 180; pos>=1; pos-=1)    //goes from 180 to 0 degrees
    {                               
      myservo.write(pos);                  //to make the servo go faster, decrease the time in delays for
      delay(5);                                  //to make it go slower, increase the number.
    }
    if (pirState == LOW){
      Serial.println("Motion detected!");
  // nandain di serial monitor
  pirState = HIGH;
    }
   
    if(lockLow){ 
      //makes sure we wait for a transition to LOW before further output is made
      lockLow = false;           
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec");
      delay(50);
    }        
    takeLowTime = true;
  }

  if (keadaan == LOW){      

    if(takeLowTime){
      lowIn = millis();             //save the time of the transition from HIGH to LOW
      takeLowTime = false;    //make sure this is only done at the start of a LOW phase
    }
   
    //if the sensor is low for more than the given pause,
    //we can assume the motion has stopped
    if(!lockLow && millis() - lowIn > pause){
      //makes sure this block of code is only executed again after
      //a new motion sequence has been detected
      lockLow = true;                       
      Serial.print("motion ended at "); //output
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      delay(50);
    }
      if (pirState == HIGH){
    // uda di matiin nih
      Serial.println("Motion ended!");
    // nandain di SM
    pirState = LOW;
    delay(500);
  }
}
}
//  }


//DHT
lcd.setCursor(0,0);
int t = dht.readTemperature();

if (t >25)

{
digitalWrite(ledPin, HIGH);
digitalWrite(pienzoPin, HIGH);
lcd.print ("Panas "); 
{
tone(9, 3047, 500);
delay(100);
noTone(8);
delay(10);//tambahn lcd.clar()
}
}
else 
{
lcd.setCursor(0,0);
digitalWrite(pienzoPin, LOW);
digitalWrite(ledPin, LOW); // tambahn lcd.clar()
}
}
