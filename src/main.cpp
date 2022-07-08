#include <Arduino.h>

#define PIN_LED     5   // Led conectado a pin 5 que es salida PWM  
#define PIN_ANALOG  A0  // el potenciometro lo conectamos al pin 


int val = 0;    // variable para almacenar el analogico

void setup() {
  // no es necesario nada, porque no son ni entrada, ni salida digital.
}

void loop() {
  val = analogRead(PIN_ANALOG);   // lee el potenciometro y el valor lo guarda el la variable val
  val = val / 4;                  // Como el valor analogico es de 1024, y el PWM es de 256, dividimos el valor para fondo de escala 
  analogWrite(PIN_LED, val);      // escribo PWM el valor val
}
