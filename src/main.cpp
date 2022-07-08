#include <Arduino.h>
#include <Servo.h>

Servo myservo;
#define PIN_ECHO      A0  // Pin ECHO conectado a A0
#define PIN_TRIGGER   A1  // Pin TRIGGER conectado a PIN A1
#define PIN_SERVO     A2  // Pin del servo conectado a PIN A2


volatile int distancia_frontal;
volatile int distancia_izquierda;
volatile int distancia_derecha;

/* Funcion distancia: calcula la distancia del sensor ultrasonico */
float distancia() {
  digitalWrite(PIN_TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER, LOW);
  float distance = pulseIn(PIN_ECHO, HIGH) / 58.00;
  delay(10);
  return distance;
}

/* Funcion detectar la distancia izquierda y derecha */
void detecta_distancia(){

  myservo.write(90);                // Servo al centro
  delay(500);
  distancia_frontal = distancia();  // totamos la distancia del frente
  delay(500);
  Serial.print("centro: ");         // la enviamos a la PC
  Serial.print(distancia_frontal);
  Serial.println(" cm");            // la enviamos a la PC

  myservo.write(0);                 // Servo al centro
  delay(500);
  distancia_derecha = distancia();  // totamos la distancia del frente
  delay(500);
  Serial.print("Derecha: ");        // la enviamos a la PC
  Serial.print(distancia_derecha);
  Serial.println(" cm");            // la enviamos a la PC

  myservo.write(180);               // Servo al centro
  delay(500);
  distancia_izquierda = distancia(); // totamos la distancia del frente
  delay(500);
  Serial.print("Izquierda: ");        // la enviamos a la PC
  Serial.print(distancia_izquierda);
  Serial.println(" cm");              // la enviamos a la PC

}


void setup(){
  myservo.attach(PIN_SERVO);
  Serial.begin(9600);
  distancia_frontal = 0;
  distancia_izquierda = 0;
  distancia_derecha = 0;
  myservo.write(90);
  pinMode(PIN_ECHO, INPUT);      
  pinMode(PIN_TRIGGER, OUTPUT);     
}

void loop(){
  detecta_distancia();
}