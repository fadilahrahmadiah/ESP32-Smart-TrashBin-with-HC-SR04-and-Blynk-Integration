// Implementation of Internet of Things (IoT) Technology
// Trash Can Capacity Monitoring and Notification
// Automatically Opening and Closing Trash Cans
// Using a Servo
// Using an HC-SR04 Sensor
// Using an IR Sensor

// Connect to Blynk
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6tcRK03O2"
#define BLYNK_TEMPLATE_NAME "project iot"
#define BLYNK_AUTH_TOKEN "tJltuU_KB4QljKQRRmoFCV55oIHDX_xx"

// Header WiFi
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

WidgetLED led(V1);

// WiFi credentials.
char ssid[] = "Wokwi-GUEST";
char pass[] = ""; 

const int trigPin = 19;
const int echoPin = 18;
const int ledMerah = 17; 

int IR = 5;

int servoPin=21;
Servo servo1;

long duration;
int distance;
bool alreadyNotified = false; 
bool modePetugas = false;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledMerah, OUTPUT);
  pinMode(IR, INPUT);
  servo1.attach(servoPin);
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  modePetugas = (value == 1);
}

void loop()
{
  Blynk.run(); 
  int irValue = digitalRead(IR);
  
  if ((irValue == 1 && distance > 5) || (modePetugas && irValue == 1)) {
    servo1.write(120);
  } else {
    servo1.write(0);
    delay(500);
    readheight();
    delay(250);
  }
}

void readheight()
{
  // digitalWrite(trigPin, HIGH);      
  // delayMicroseconds(10);           
  // digitalWrite(trigPin, LOW);      
  digitalWrite(trigPin, LOW);      
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);      
  delayMicroseconds(10);           
  digitalWrite(trigPin, LOW);  

  duration = pulseIn(echoPin, HIGH); 
  distance = duration * 0.034 / 2;

  Serial.print(": ");
  Serial.println(distance);
  Blynk.virtualWrite(V0, distance);

  if (distance <= 5 ) {
    digitalWrite(ledMerah, HIGH);
    led.on();
    if (!alreadyNotified) {
      Blynk.logEvent("trash_bin_full", "Oops, the trash bin is full! Please empty it when you get a chance."); 
      alreadyNotified = true; 
    }
  } else {
    digitalWrite(ledMerah, LOW);
    led.off();
    alreadyNotified = false;
  }
}
