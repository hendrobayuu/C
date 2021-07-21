#include <NewPing.h> //Library untuk Sensor Ultrasonic
#include <Servo.h> //Library untuk Servo
#define trigPin 6 //Set Trigger HCSR04 di Pin digital 12
#define echoPin 5 //Set Echo HCSR04 di Pin digital 13
//#define MAX_DISTANCE 1000 //Set jarak maksimal
NewPing sonar(trigPin, echoPin);
int LEDhijau = 9; 
int LEDmerah = 8; //Set Pin LEDhijau dan LEDmerah
Servo myservo; //Buat object 1 buah motor servo

void setup() {
 Serial.begin (9600); //Kecepatan komunikasi serial
 pinMode(trigPin, OUTPUT); //Set pin Trigger sebagai output
 pinMode(echoPin, INPUT); //Set pin Echo sebagai input
 pinMode(LEDmerah, OUTPUT); //Set LEDmerah sebagai output
 pinMode(LEDhijau, OUTPUT); //Set LEDhijau sebagai output
 myservo.attach(4); //Set servo pada pin PWM 4
}

void loop() {
int duration, jarak=0;
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW); 
 duration = pulseIn(echoPin, HIGH);
 jarak = (duration/2) / 29.1; 
  Serial.print(jarak);
  Serial.println(" cm");

if(jarak<=15) // Jarak (Cm) dapat anda sesuaikan

 {
   digitalWrite(LEDhijau, LOW); //LEDhijau mati
   digitalWrite(LEDmerah, HIGH); //LEDmerah hidup
   myservo.write(180); //Posisi servo pada 180 derajat
   delay(450); //Delay
   digitalWrite(LEDmerah, LOW); //LEDmerah mati
   myservo.write(90); //Posisi servo pada 90 derajat
   delay(450); //Delay
   digitalWrite(LEDmerah, HIGH); //LEDmerah hidup
   myservo.write(0); //Posisi servo pada 0 derajat
   delay(450); //Delay
   digitalWrite(LEDmerah, LOW); //LEDmerah mati
   myservo.write(90); //Posisi servo pada 90 derajat
}

 else{ //Jika jarak lebih dari yang ditentukan
   digitalWrite(LEDmerah, LOW); //LEDmerah mati
   digitalWrite(LEDhijau, HIGH); //LEDhijau hidup
   myservo.write(90); //Posisi servo pada 90 derajat
 }
delay(450); //Delay
}
