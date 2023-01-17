#define BLYNK_TEMPLATE_ID "TMPLGlshk-f3"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "KBWTyWctwmJJCKRgHyqViiIO-tD0ocHe"

char auth[] = BLYNK_AUTH_TOKEN;
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>


#define Echo 15
#define Trig 4
#define Merah 14
#define Kuning 12
#define Hijau 13
#define pinBuzzer 27

char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

float distance = 0;
float ketinggian = 0;

Servo myservo;
int rotasi = 90;
int interval = 0;

void ukur_jarak(){
  digitalWrite(Trig,LOW);
  delay(2);
  digitalWrite(Trig, HIGH);
  delay(10);
  digitalWrite(Trig, LOW);

  int duration = pulseIn(Echo, HIGH);
  distance = duration * 0.034 / 2;
  ketinggian = 10 - distance ;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" CM");

  Serial.print("Ketinggian Pakan : ");
  Serial.print(ketinggian);
  Serial.println(" CM");
}

  void soundBuzzer(){
    if(interval < 500){
    tone(pinBuzzer, 250);
    }else
    if(interval > 500){
    tone(pinBuzzer, 100,800);
    };
  }
void kirim_blynk(){
  Blynk.virtualWrite(V0, ketinggian);
  Blynk.virtualWrite(V4, ketinggian);
    if (distance > 7 && distance < 10){
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, 1);
  }
  else if (distance > 3 && distance < 7){
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 1);
    Blynk.virtualWrite(V3, 0);
  }
  else if (distance > 0 && distance < 4){
    Blynk.virtualWrite(V1, 1);
    Blynk.virtualWrite(V2, 0);
    Blynk.virtualWrite(V3, 0);
  }
}

 
BLYNK_WRITE(V5) {
  myservo.write(param.asInt());
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Merah, OUTPUT);
  pinMode(Kuning, OUTPUT);
  pinMode(Hijau, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
  myservo.attach(2);
  myservo.write(90);
  timer.setInterval(2000L,kirim_blynk);
}

void loop() {
   
  digitalWrite(Merah, LOW);
  digitalWrite(Hijau, LOW);
  digitalWrite(Kuning, LOW);
 
  ukur_jarak();
  int ketinggian = 10 - distance ;
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" CM");

  if( distance > 3 && distance < 7)
  {
    Serial.println("Cukup");
    digitalWrite(Kuning, HIGH);
    tutup();
    noTone(pinBuzzer);
  }
  else if (distance > 7 && distance < 10)
  {
    Serial.println("Habis");
    digitalWrite(Merah, HIGH);
    soundBuzzer();
    buka();
  }
  else if (distance > 0 && distance < 4)
  {
    Serial.println("Banyak");
    digitalWrite(Hijau, HIGH);
    noTone(pinBuzzer);
    tutup();
  }
  else 
  {
    Serial.println("Pakan Melebihi Batas Tempat ");
  }
  Blynk.run();
  timer.run();
  delay(500);
}

void tutup(){
    if(rotasi <= 90){
    myservo.write(rotasi);
    delay(10);
    rotasi+=30;
  }
  if(rotasi <= 0 ) rotasi = 0;
}

  void buka(){
    if(rotasi <= 90){
    myservo.write(rotasi);
    delay(10);
    rotasi-=30;
  }
  if(rotasi >= 90 ) rotasi = 90;
}