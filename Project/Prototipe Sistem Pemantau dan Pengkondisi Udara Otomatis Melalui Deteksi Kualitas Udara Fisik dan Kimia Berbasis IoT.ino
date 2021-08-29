//pin-pin yang digunakan dalam penelitian ini adalah
//BME280 Vin (3,3V), GND, SDA (21), SCL (22)
//MQ135 Vcc (5V), GND, Do (NC), Ao (res 10k||32||res 20k)
//DS18B20 Vcc (3.3V), GND, Data (Vcc||res 4.3k||23)
//PMS5003 Vcc (5V), GND, RX (17), TX (16)
//Relay Panas (14)
//Relay Dingin (15)
//Relay Blower (4)
//=====Library=====
#include <WiFi.h>//Library yang memungkinkan mikrokontroler terhubung secara jaringan internet library ini mendukung pada WEP dan WPA2 Personal encryption
#include <DallasTemperature.h>//Library sensor suhu DS18B20
#include <Adafruit_BME280.h>//Library BME280
#include <MQUnifiedsensor.h>//Library sensor MQ-135
#include <HardwareSerial.h>//mengaktifkan port serial tambahan untuk komunikasi UART
HardwareSerial pmsSerial(1);
/*
 U0UXD is used to communicate with the ESP32 for programming and during reset/boot. U1UXD is unused and can be used for your projects. Some boards use this port
for SPI Flash access though  U2UXD is unused and can be used for your projects. UART adalah singkatan dari Universal Asynchronous Receiver / Transmitter
 Tujuan utama UART adalah untuk mengirim dan menerima data serial. Pengantar Komunikasi UART Dalam komunikasi UART, dua UART berkomunikasi secara langsung satu sama lain.
UART pemancar mengubah data paralel dari perangkat pengontrol seperti CPU menjadi bentuk serial, mengirimkannya secara serial ke UART penerima, yang kemudian mengubah data serial kembali menjadi data paralel untuk perangkat penerima. Hanya dua kabel yang dibutuhkan untuk mengirimkan data antara dua UART.
Data mengalir dari pin Tx dari transmisi UART ke pin Rx dari UART penerima Ketika UART penerima mendeteksi bit awal, ia mulai membaca bit yang masuk pada frekuensi tertentu yang dikenal sebagai baud rate.
 Baud rate adalah ukuran kecepatan transfer data, dinyatakan dalam bits per second (bps).
*/
// Note the format for setting a serial port is as follows: Serial2.begin(baud-rate,
protocol, RX pin, TX pin);
//Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
//=====Deklarasi Pin=====
#define DS18B20 23//Deklarasi Pin 23 sebagai pin data DS18B20
#define mikro "ESP32 Dev Module"//tipe board untuk MQ135
#define tegangan 3.3//tegangan referensi MQ135
#define pin_analog 33//deklarasi pin 33sebagai pin analog MQ135
#define tipe "MQ-135"//tipe sensor gas yang digunakan
#define ADC_Bit_Resolusi 12//bit ADC RESOLUSI Nodemcu ESP32 yaitu 4096 atau 0-4095
#define Rasio_udara 3.6//RS / R0 = 3.6 PPM
//====Parameter Network dan Tipe data====
const char* ssid = "KAPOTHA"; //ssid yang digunakan untuk terhubung secara iot||deklarasi konstan characther
const char* password = "9876543210"; //password ssid||deklarasi konstan characther
String api_key = "IIYS1HTVPZO4BSRD";//kode write API (Application Programming Interface) pada Thingspeak
/*memungkinkan developer untuk mengintegrasikan dua bagian dari aplikasi atau
dengan aplikasi yang berbeda secara bersamaan.
API terdiri dari berbagai elemen seperti function, protocols, dan tools lainnya
yang memungkinkan developers untuk membuat aplikasi.
 Tujuan penggunaan API adalah untuk mempercepat proses development dengan
menyediakan function secara terpisah sehingga developer tidak perlu membuat
fitur yang serupa.*/
char* server = "api.thingspeak.com";//situs alamat server
float kelembapan = 0; //tipe data variabel yaitu integer
float suhu = 0; //tipe data variabel yaitu integer
float CO2 = 0; //tipe data variabel yaitu integer
float R0 = 0; //tipe data variabel yaitu integer
int pm10 = 0; //tipe data variabel yaitu integer
const int pendingin = 15;//deklarasi konstan pin 15 untuk relay aktuator pendingin
const int pemanas = 14;//deklarasi konstan pin 14 untuk relay aktuator pemanas
const int exhaust = 4;//deklarasi konstan pin 4 untuk relay aktuator exhaust
float ADC_MQ135 = 0;//tipe data variabel yaitu integer
float tegangan_kalkulasi_MQ135 = 0; //tipe data variabel yaitu integer
Adafruit_BME280 bme;//menggunakan alamat I2C
OneWire ourWire(DS18B20); //menggunakan protokol komunikasi 1-wire (onewire) DS18B20
DallasTemperature sensor(&ourWire);//komunikasi perangkat Dallas untuk DS18B20
WiFiClient client; // Initialize the client library||Membuat klien yang dapat terhubung ke alamat
MQUnifiedsensor MQ135(mikro, tegangan, ADC_Bit_Resolusi, pin_analog,
tipe);//fungsi library yang menungkinkan membaca sensor MQ melalui board
void setup() // put your setup code here, to run once:
{
 Serial.begin(115200);//kecepatan komunikasi ke serial monitor
 pmsSerial.begin(9600, SERIAL_8N1, 16, 17);//(baud-rate, protocol, RX pin, TX
pin)
 pinMode(pendingin, OUTPUT);//fungsi pin pendingin sebagai keluaran
  pinMode(pemanas, OUTPUT);//fungsi pin pemanas sebagai keluaran
 pinMode(exhaust, OUTPUT);//fungsi pin exhaust sebagai keluaran
 sensor.begin();//Start up the library DS18B20
 Wire.begin(21, 22);//memulai I2C dan pengalamatan pin SDA & SCL pada BME280
 MQ135.setRegressionMethod(1);//_PPM = a*ratio^b untuk MQ-135
 MQ135.init(); //Init() menginisialisasi penghitung waktu, konverter AD dan
komunikasi serial board
 for (int i = 1; i <= 10; i ++)
 {
 MQ135.update();// Update data, the board will be read the voltage on the analog pin
 R0 += MQ135.calibrate(Rasio_udara);
 }
 MQ135.setR0(R0 / 10);
  Serial.println(" done!.");
 if (isinf(R0)) ////pengecekan jalur sensor||Determines if the given floating point number arg is positive or negative infinity
 {
 Serial.println("Koneksi bermasalah R0 bernilai tak hingga MQ-135 (Open Circuit) periksa jalur");
 while (1);
 }
 if (R0 == 0) //pengecekan jalur sensor
 {
 Serial.println("Koneksi bermasalah R0 bernilai nol (pin analog short circuit ke ground) periksa jalur");
 while (1);
 }
 if (!bme.begin(0x76))//inisialisasi I2C address||pengecakan jalur sensor
 {
 Serial.println("Koneksi BME280 bermasalah periksa jalur");
  while (1);
 }
 if (suhu == -127)//pengecakan jalur sensor
 {
 Serial.println("Koneksi bermasalah DS18B20 periksa jalur");
 while (1);
 }
 koneksi_wifi();//eksesusi
 delay(100);
}
struct pms5003data//Struct adalah tipe data bentukan yang berisi kumpulan variabel-variabel yang berada dalam satu nama yang sama dan memiliki kaitan satu sama lain.
{
 uint16_t framelen; //Jadi kita tidak akan bisa memiliki nilai 256 dalam satu char (atau byte).
 //uint16_t sama dengan 2 byte atau 16 bit. Karena (2 ^ 8) - 1 = 256 - 1 = 255. Memang nilai pertama kita 0 dan bukan 1.
 uint16_t pm10_standard, pm25_standard, pm100_standard;
 uint16_t pm10_env, pm25_env, pm100_env;
 uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
 uint16_t unused;
 uint16_t checksum;
};
struct pms5003data data;
void loop() // put your main code here, to run repeatedly:
{
 lembapBME280();//eksekusi
 gasMQ135();//eksekusi
 suhuDS18B20();//eksekusi
  pm10PMS5003();//eksekusi
 kirim_data();//eksekusi
 aktuator();//eksekusi
}
void koneksi_wifi()
{
 WiFi.begin( ssid, password );//koneksi ke wifi
 while (WiFi.status() != WL_CONNECTED)//menunggu hingga wifi terkoneksi
 {
 delay(100);
 Serial.print(".");
 }
 Serial.println( "Connected" );
}
void lembapBME280()
{
 kelembapan = bme.readHumidity(); //membaca nilai kelembapan dari sensor BME280
}
void gasMQ135()
{
 MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin
 MQ135.setA(110.47); MQ135.setB(-2.862); // Configurate the ecuation values to get CO2 concentration
 CO2 = MQ135.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
 ADC_MQ135 = analogRead(pin_analog);//membaca ADC pada pin analog sensor MQ135
tegangan_kalkulasi_MQ135 = ( ( ADC_MQ135 * 3.3 ) / 4095);//mengkalkulasi sinyal analog menjadi tegangan
}
void suhuDS18B20()
{
 sensor.requestTemperatures();
 suhu = sensor.getTempCByIndex(0);
}
void pm10PMS5003()
{
 if (readPMSdata(&pmsSerial)) //bandingkan mereun
 {
 //Serial.println();
 //Serial.println("---------------------------------------");
 //Serial.println("Concentration Units (standard)");
 //Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
 //Serial.println("---------------------------------------");
  //Serial.println("Concentration Units (environmental)");
 //Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
 //Serial.println("---------------------------------------");
 pm10 = data.particles_100um;
 //Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(pm10);
 //Serial.println("---------------------------------------");
 //delay(1000);
 }
}
boolean readPMSdata(Stream *s)
{
 if (! s->available())
 {
 return false;
 }
 if (s->peek() != 0x42)
 {
 s->read();
 return false;
 }
 if (s->available() < 32)
 {
 return false;
 }
 uint8_t buffer[32];
 uint16_t sum = 0;
 s->readBytes(buffer, 32);
 for (uint8_t i = 0; i < 30; i++)
 {
 sum += buffer[i];
 }
 for (uint8_t i = 2; i < 32; i++)
 {
 //Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
 }
 Serial.println();
 uint16_t buffer_u16[15];
 for (uint8_t i = 0; i < 15; i++)
 {
 buffer_u16[i] = buffer[2 + i * 2 + 1];
 buffer_u16[i] += (buffer[2 + i * 2] << 8);
 }
 memcpy((void *)&data, (void *)buffer_u16, 30);
 if (sum != data.checksum)
 {
 Serial.println("Checksum failure");
 return false;
 }
 return true;
}
void kirim_data()
{
 Serial.println("| SUHU | KELEMBAPAN | CO2 | PM10 | ADC MQ135 | TEGANGAN MQ135 |");
 Serial.print("| "); Serial.print(suhu);
 Serial.print(" | "); Serial.print(kelembapan);
 Serial.print(" | "); Serial.print(CO2);
 Serial.print(" | "); Serial.print(pm10);
 Serial.print(" | "); Serial.print(ADC_MQ135);
 Serial.print(" | "); Serial.print(tegangan_kalkulasi_MQ135);
 Serial.println(" |");
 if (client.connect(server, 80)) //jika koneksi terhubung
 {
 String sendData = api_key;
 sendData += "&field1=" + String(suhu); //pengiriman data suhu untuk dituliskan pada thingspeak field 1
 sendData += "&field2=" + String(kelembapan); //pengiriman data kelembapan untuk dituliskan pada thingspeak field 1
 sendData += "&field3=" + String(pm10); //pengiriman data pm10 untuk dituliskan pada thingspeak field 1
 sendData += "&field4=" + String(CO2); //pengiriman data CO2 untuk dituliskan pada thingspeak field 1
 sendData += "&field5=" + String(ADC_MQ135); //pengiriman data ADC MQ135 untuk dituliskan pada thingspeak field 1
sendData += "&field6=" + String(tegangan_kalkulasi_MQ135); //pengiriman data tegangan kalkulasi MQ135 untuk dituliskan pada thingspeak field 1
/* if (suhu < 18) { sendData += "&status=" + String("Suhu Dingin"); }
else if (suhu > 30) { sendData += "&status=" + String("Suhu Panas"); }
else { sendData += "&status=" + String("Suhu Normal"); }
if ((pm10 < 20) && (CO2 < 250)) { sendData += "&status=" +
String("PM10||CO2 Normal"); }
else if ((pm10 >= 20 && pm10 <= 70) && (CO2 < 250)) { sendData +=
"&status=" + String("PM10||CO2 Normal"); }
else if ((pm10 < 20) && (CO2 >= 250 && CO2 <= 1000)) { sendData +=
"&status=" + String("PM10||CO2 Normal"); }
else if ((pm10 >= 20 && pm10 <= 70) && (CO2 >= 250 && CO2 <= 1000)) {
sendData += "&status=" + String("PM10||CO2 Normal"); }
else if ((pm10 < 20) && (CO2 > 1000)) { sendData += "&status=" +
String("CO2 Terpolusi"); }
else if ((pm10 >= 20 && pm10 <= 70) && (CO2 > 1000)) { sendData +=
"&status=" + String("CO2 Terpolusi"); }
else if ((pm10 > 70) && (CO2 > 1000)) { sendData += "&status=" +
String("PM10||CO2 Terpolusi"); }
else if ((pm10 > 70) && (CO2 >= 250 && CO2 <= 1000)) { sendData +=
"&status=" + String("PM10 Terpolusi"); }
else if ((pm10 > 70) && (CO2 < 250)){ sendData += "&status=" + String("PM10
Terpolusi"); }*/
 sendData += "\r\n\r\n";
 client.print("POST /update HTTP/1.1\n");//client post
 client.print("Host: api.thingspeak.com\n");//client host
 client.print("Connection: close\n");//client stop
 client.print("X-THINGSPEAKAPIKEY: " + api_key + "\n");
 client.print("Content-Type: application/x-www-form-urlencoded\n");
 client.print("Content-Length: ");
 client.print(sendData.length());
 client.print("\n\n");
 client.print(sendData);
 }
 Serial.println("Sending....");
 delay(16000);
 Serial.println();
}
void aktuator()
//======SUHU======
{
 if (suhu < 18)
 {
 digitalWrite(pendingin, LOW);
 Serial.print("DINGIN : ");
 digitalWrite(pemanas, HIGH);
 Serial.println(suhu);
 }
 else if (suhu > 30)
  {
 digitalWrite(pendingin, HIGH);
 Serial.print("PANAS : ");
 digitalWrite(pemanas, LOW);
 Serial.println(suhu);
 }
 else
 {
 digitalWrite(pendingin, LOW);
 Serial.print("HANGAT : ");
 digitalWrite(pemanas, LOW);
 Serial.println(suhu);
 }
 //======EXHAUST FAN======
  if ((pm10 < 20) && (CO2 < 250)) 
  {
 Serial.println ("=====A LOW=====");
 digitalWrite(exhaust, LOW);
 }
 else if ((pm10 >= 20 && pm10 <= 70) && (CO2 < 250))
 {
 Serial.println ("=====D LOW=====");
 digitalWrite(exhaust, LOW);
 }
 else if ((pm10 < 20) && (CO2 >= 250 && CO2 <= 1000))
 {
 Serial.println ("=====B LOW=====");
 digitalWrite(exhaust, LOW);
 }
 else if ((pm10 >= 20 && pm10 <= 70) && (CO2 >= 250 && CO2 <= 1000))
 {
 Serial.println ("=====E LOW=====");
  digitalWrite(exhaust, LOW);
 }
 else if ((pm10 < 20) && (CO2 > 1000)) //Bahaya
 {
 Serial.println ("=====C HIGH=====");
 digitalWrite(exhaust, HIGH);
 Serial.println("Blower NYALA");
 }
 else if ((pm10 >= 20 && pm10 <= 70) && (CO2 > 1000))
 {
 Serial.println ("=====F HIGH=====");
 digitalWrite(exhaust, HIGH);
 Serial.println("Blower NYALA");
 }
 else if ((pm10 > 70) && (CO2 > 1000)) 
  {
 Serial.println ("=====I HIGH=====");
 digitalWrite(exhaust, HIGH);
 Serial.println("Blower NYALA");
 }
 else if ((pm10 > 70) && (CO2 >= 250 && CO2 <= 1000))
 {
 Serial.println ("=====H HIGH=====");
 digitalWrite(exhaust, HIGH);
 Serial.println("Blower NYALA");
 }
 else if ((pm10 > 70) && (CO2 < 250))
 {
 Serial.println ("=====G HIGH=====");
 digitalWrite(exhaust, HIGH);
 Serial.println("Blower NYALA");
  }
}
