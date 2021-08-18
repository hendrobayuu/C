#include <SoftwareSerial.h> //sebagai library untuk mengubah pin digital menjadi rx dan tx
SoftwareSerial SIM800A(9, 10); // RX | TX
int relay = 8; //lokasi pin untuk menyalakan led
String response; //variable response sebagai tipe data string
int lastStringLength = response.length(); //variable sebagai tipe data integer
void setup()
{
    pinMode(relay, OUTPUT);//seabagai output
    Serial.begin(9600); //kecepatan komunikasi arduino
    while(!Serial);
    Serial.println("Arduino with SIM800L is ready");//tertulis pada serial monitor
    SIM800A.begin(9600); //kecepatan komunikasi tx rx
    SIM800A.println("AT+CMGF=1"); //memilih pesan
    Serial.println("SIM800L started at 9600"); //tertulis pada serial monitor
    delay(1000); //jeda 1000 milisecond
    Serial.println("Setup Complete! SIM800L is Ready!"); //tertulis pada serial monitor
    SIM800A.println("AT+CNMI=2,2,0,0,0"); //indikasi sms masuk
}
void loop()
{
      // Keep reading from Arduino Serial Monitor and send to SIM800L
    if (Serial.available()){ //kondisi jika tersedia
      SIM800A.write(Serial.read()); //membaca tulisan 
    }
     if (SIM800A.available()>0){ //kondisi jika sms yang diterima
      response = SIM800A.readStringUntil('\n'); //tersimpan pada valiable respon
    }
    if(lastStringLength != response.length()){ //ukuran data
      Serial.println(response);
      Serial.println(response.indexOf("ON"));
      if(response.indexOf("ON") == 6){
        digitalWrite(relay,HIGH); //led menyala
        KirimHidup(); //eksekusi bagian void kirim hidup
        Serial.println("LAMPU ON");//sms yang diterima bertuliskan LAMPU ON
      }
      if(response.indexOf("OFF") == 6){
        digitalWrite(relay,LOW);//led mati
        KirimMati(); //eksekusi bagian void kirim mati
        Serial.println("LAMPU OFF");//sms yang diterima bertuliskan LAMPU OFF
      }
      lastStringLength = response.length(); //ukuran data 
    }
}
 void KirimHidup(){ 
  SIM800A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM800A.println("AT+CMGS=\"+6282126828084\"\r"); // Replace x with mobile number
  delay(1000);
  SIM800A.println("Lampu Hidup");// The SMS text you want to send
  delay(100);
  SIM800A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void KirimMati(){
  SIM800A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM800A.println("AT+CMGS=\"+6282126828084\"\r"); // Replace x with mobile number
  delay(1000);
  SIM800A.println("Lampu Mati");// The SMS text you want to send
  delay(100);
  SIM800A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
