#include <Arduino.h>
#include <Servo.h>

#define PIN_SERVO A2
Servo miservo;  // Creamos el objeto, servo, de nombre miservo

void setup() {
  miservo.attach(PIN_SERVO);  // inicializa el servo en el pin especificado
}

void loop() {

  delay(2000);
  miservo.write(90);  // ponemos el servo en el medio
  delay(2000);
  miservo.write(0);   // ponemos a la izquierda
  delay(2000);
  miservo.write(180); // ponemos el derecha
}
