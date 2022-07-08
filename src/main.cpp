#include <Arduino.h>
/* EJERCICIO PRACTICO: Leyendo la temperatura por medio del sensor LM35,
 *  se encendera un led verde conectado al pin 8 en caso que la temperatura
 *  medida este por debajo de una temperatura umbral, la cual la sera de 25°C
 *  en caso de que la temperatura sea igual o superior a 25°C, se debera encender
 *  un led ROJO que estara conectado al pin 8. y en caso de que la temperatura
 *  supere al umbral inicial, por encima de 10°C, se debera encender un ventilador 
 *  que esta accionado mediante el pin 5. utilizar el puerto serie, para saber 
 *  el valor de temperatura del sensor 
 */

/*DESARROLLO DEL PROBLEMA */


/* DEFINICIONES DE PINES */
#define LM35        A0  // LM35 en PIN A0
#define LED_ROJO    7   // Led rojo esta conectado a PIN 7
#define LED_VERDE   8   // Led rojo esta conectado a PIN 8
#define VENTILADOR  5   // control ventilador

/* DEFINICIONES DE VARIABLES Y CONSTANTES */
float temp_umbral = 25.0;   // 1° umbral de temperatura: 25 °C
float temp_umbral2 = 10.0;  // 2° umbral de temperatura: 10 °C  
int adc = 0;                // en esta variable guardare el valor del sensor de temperatura
float temperatura =0;       // en esta variable vamos a guardar el valor de la temperatura leida


/*FUNCION SETUP: configuracion inicial de perifericos */
void setup() {

  /* CONFIGURACION DE PINES DIGITALES */
  pinMode(LED_ROJO, OUTPUT);      // LED_ROJO es salida
  pinMode(LED_VERDE, OUTPUT);     // LED_VERDE es salida
  pinMode(VENTILADOR, OUTPUT);    // VENTILADOR es salida

  /* FORZADO DE VALOR INICIAL DE PINES DIGITALES */
  digitalWrite(LED_ROJO, LOW);    // apago el led rojo
  digitalWrite(LED_VERDE, LOW);   // apago el led verde
  digitalWrite(VENTILADOR, LOW);  // apago el ventildor

  /* CONFIGURACION DEL ADC */
  analogReference(INTERNAL);
  /* Referencia Interna 1100 mV: esta instruccion le establece al conversor analogico digital 
   *  que debera convertir a digital valores entre 0V y 1100mV (1.1V), esto implicara
   *  que en caso que el sensor LM35 lea una temperatura por encima de 110°C, no podremos determinar
   *  si es por ejemplo, 115°C ó 180°C, ya que el conversor mostrara el valor 1023.
   *  Por lo tanto con esta configuracion, el rango util del sensor quedara conprendida en temperaturas
   *  de entre 0°C y 110°C. Sin embargo la resolusion sera mayor debido al menor rango de operacion.*/
   
  /* CONFIGURACION DEL PUERTO SERIE */
  Serial.begin(9600); // avisamos que usaremos el puerto serie y a 9600 baudios
}

/*FUNCION LOOP: Desarrollo del ejercicio*/
void loop() {

  // etapa 1: el primer paso del ejercicio es leer el sensor, para saber que tendremos que hacer:
  adc = analogRead(LM35); // guardamos en la variable adc, la lectura del sensor.
  /* Sin embargo, hemos guardado un valor que se comprende entre 0 y 1023,
   *  por lo cual sera necesario convertirlo a °C, teniendo en cuenta, que nuestro ADC va
   *  a leer valores entre 0V y 1100mV, y que la funcion del sensor es: mV = 10 mV * °C,
   *  asi como ejemplo en caso que la temperatura sea de 23°C, la salida sera de 230 mV
   *  procederemos a calcular el valor en °C en funcion del valor de adc.
   */
  temperatura = 1100 / 1023 * adc / 10 ; // realizamos el calculo
  Serial.println(temperatura);  // y tal cual se nos ordena, enviamos a la PC el valor de la temperatura, 

  /* 2° etapa: deberemos tomar dediciones en funcion de la temperatura, asi que
   * como primer medida, si la temperatura esta por debajo del umbral,
   * deberemos prender el led verde y apagar el rojo y el ventilador
   */
  if (temperatura < temp_umbral){ // si la temperatura es menor que temp_umbral
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
    digitalWrite(VENTILADOR, LOW);
  }
  /* 3° etapa: en caso que la temperatura sea mayor o igual
   *  deberemos apagar el verde, endender el rojo y apagar el
   *  ventilador
   */
  if (temperatura >= temp_umbral){
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(VENTILADOR, LOW);
  }
  /* 4° etapa: en caso que la temperatura sea mayor o igual
   *  que la temperatura elejida + el 2° umbral, se debera
   *  apagar el verde, encender el rojo y ademas el ventidador
   */
  if(temperatura >= temp_umbral + temp_umbral2){
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(VENTILADOR, HIGH);
  }

  /* finalmente, agregaremos una pequeña demora para que no sea caotica la recepcion
   *  a traves de la PC, de la temperatura
   */
  delay(200);
}
