#include  <Wire.h> //library I2C
#include  <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
const int ledPin = 3; //set led pada pin 13 arduino
const int buzzerPin = 2; //set buzzer pada pin 2 arduino
const int ldrPin = A2; //set ldr pada pin A2arduino
int nilai;

void setup () {
  Serial.begin(9600); //kecepatan komunikasi serial arduino
lcd.begin(16,2); //LCD 16x2
  pinMode(ledPin, OUTPUT); //set led sebagai output
  pinMode(buzzerPin, OUTPUT); //set buzzer sebagai output
  pinMode(ldrPin, INPUT); //set ldr sebagai input
}

void loop() {
  int ldrStatus = analogRead(ldrPin);  //read the state of the LDR value
   nilai= analogRead(ldrPin);
  Serial.print("Nilai LDR: "); //menampilkan pada serial monitor
  Serial.println(nilai);
  
  if (ldrStatus <= 400) { //jika status ldr kurang atau sama dengan 400
    lcd.setCursor(0,0); //
    lcd.print("mendung"); //menampilkan di lcd
    delay(100); //jeda 100 milisecond 
    lcd.setCursor(0,1);
    lcd.print("Nilai LDR: ");//menampilkan pada serial monitor
  lcd.print(nilai);
    delay(100);
    

    tone(buzzerPin, HIGH); //buzzer mati selama jeda 100 milisecond
    digitalWrite(ledPin, HIGH); //led mati
    delay(100); //jeda 100 milisecond
    Serial.println("ALARM AKTIF"); //menampilkan di serial monitor
  noTone(buzzerPin);
  digitalWrite(ledPin, LOW);
  }
  
  else {
    lcd.setCursor(0,0);
lcd.print("cerah  "); //menampilkan di lcd
delay(100); // jeda 100 milisecond
 lcd.setCursor(0,1);
    lcd.print("Nilai LDR: ");
  lcd.print(nilai);
    delay(100);
    noTone(buzzerPin); //buzzer mati s
    digitalWrite(ledPin, LOW); //led mati
    Serial.println("ALARM NONAKTIF"); //menampilkan di serial monitor
  }
 }
