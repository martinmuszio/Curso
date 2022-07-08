#include <Arduino.h>

// SENSOR DE DISTANCIA 
#define TRIGGER 5 //Pin digital 2 para el TRIGGER del sensor
#define ECHO    6 //Pin digital 3 para el ECHO del sensor


void setup(){
  Serial.begin (9600);
  pinMode(TRIGGER, OUTPUT); //
  pinMode(ECHO, INPUT); 
}

void loop(){   
  long duracion, distancia ;
  digitalWrite(TRIGGER, LOW);       // Nos aseguramos de que el trigger está desactivado
  delayMicroseconds(2);             // Para asegurarnos de que el trigger esta LOW
  digitalWrite(TRIGGER, HIGH);      // Activamos el pulso de salida
  delayMicroseconds(10);            // Esperamos 10µs. El pulso sigue active este tiempo
  digitalWrite(TRIGGER, LOW);       // Cortamos el pulso y a esperar el echo
  duracion = pulseIn(ECHO, HIGH) ;  // guardamos la duracion del pulso
  distancia = duracion / 58.2;      // Realizamos el calculo
  Serial.print(distancia);          // enviamos la variable distancia
  Serial.println(" cm.");           // y seguido los cm.
  delay (500) ;                     // Para limitar el número de mediciones
}
