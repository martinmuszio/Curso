#include <Arduino.h>

/* CONTROL DE SERVO */
#define PIN_SERVO A2  // PIN SIGNAL DEL SERVO a A2


void setup(){ // Configuración inicial
  
  pinMode(PIN_SERVO, OUTPUT);   // Declara que PIN_SERVO será la Salida digital A2
  digitalWrite(PIN_SERVO, LOW); // como de costumbre, incializamos en bajo
}

void loop(){

  /* SERVO AL MEDIO (0°): debemos segun datasheet, enviar
   * un pulso de 1.5 ms para que el servo quede en posicion
   * de 0°
  */
  digitalWrite(PIN_SERVO, HIGH);  // subimos el pin 
  delayMicroseconds(1500);        // durante 1500 us
  digitalWrite(PIN_SERVO, LOW);   // bajamos el pin
  delayMicroseconds(18500);       // durante el 18500 us
  delay(2000);  //Esperamos 2 segundos para ver el movimiento

  /* SERVO Derecha (90)°: debemos, segun datasheet, enviar
   * un pulso de 2.0 ms para que el servo quede en posicion
   * de 0°
  */
  digitalWrite(PIN_SERVO, HIGH);  // subimos el pin 
  delayMicroseconds(2000);        // durante 1500 us
  digitalWrite(PIN_SERVO, LOW);   // bajamos el pin
  delayMicroseconds(18000);       // durante el 18000 us
  delay(2000);  //Esperamos 2 segundos para ver el movimiento
  
  /* SERVO izquierda (-90°): debemos, segun datasheet, enviar
   * un pulso de 1.0 ms para que el servo quede en posicion
   * de 0°
  */
  digitalWrite(PIN_SERVO, HIGH);  // subimos el pin 
  delayMicroseconds(1000);        // durante 2000 us
  digitalWrite(PIN_SERVO, LOW);   // bajamos el pin
  delayMicroseconds(19000);       // durante el 19000 us
  delay(2000);  //Esperamos 2 segundos para ver el movimiento
}
