#include <Arduino.h>

#define PIN_298ENA  5   // ENA (enable A) conectado a PIN 5
#define PIN_298ENB  6   // ENB (enable B) conectado a PIN 6
#define PIN_298IN1  9   // IN1 del L298, conectado a PIN 9
#define PIN_298IN2  4   // IN2 del L298, conectado a PIN 4
#define PIN_298IN3  7   // IN3 del L298, conectado a PIN 7 
#define PIN_298IN4  8   // IN4 del L298, Rigth Forward, conectado a PIN 8
#define PIN_GLOBAL  2   // Boton Conectado a Pin 2

void stopp(void);
// Funcion Interrpcion, solo aumenta la variable count
void isr(){ //interrupt service routine
  stopp();
}

/*funcion ir hacia adelante: enciende ambos motores para ir hacia adelante
* se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void ir_adelante(unsigned char speed_val){    // speed_val：0~255
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,LOW);
  digitalWrite(PIN_298IN3,HIGH); 
  digitalWrite(PIN_298IN4,LOW);
  analogWrite(PIN_298ENA,speed_val);
  analogWrite(PIN_298ENB,speed_val);
}

/*funcion ir hacia atras: enciende ambos motores para ir hacia atras
* se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void ir_atras(unsigned char speed_val){    // speed_val：0~255 
  digitalWrite(PIN_298IN1,LOW);  
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,LOW);  
  digitalWrite(PIN_298IN4,HIGH);
  analogWrite(PIN_298ENA,speed_val);
  analogWrite(PIN_298ENB,speed_val);
}

/*funcion girar a la izquierda: motor izquierdo para atras y motor derecho para adelante, 
*se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void gira_izquierda(unsigned char speed_val){        // speed_val：0~255
  digitalWrite(PIN_298IN1,LOW); 
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,HIGH);
  digitalWrite(PIN_298IN4,LOW);
  analogWrite(PIN_298ENA,speed_val);
  analogWrite(PIN_298ENB,speed_val);
}

/*funcion girar a la derecha: motor izquierdo para adelante y motor derecho para atras, 
*se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void gira_derecha(unsigned char speed_val){    // speed_val：0~255
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,LOW);  
  digitalWrite(PIN_298IN3,LOW);  
  digitalWrite(PIN_298IN4,HIGH);
  analogWrite(PIN_298ENA,speed_val);
  analogWrite(PIN_298ENB,speed_val);
}

/*funcion detener motores: motor izquierdo y derecho para apagados
* NOTA: no es necesario pasarle por parametro ningun valor
*/
void stopp(){
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,HIGH);
  digitalWrite(PIN_298IN4,HIGH);
}


void setup(){

  Serial.begin(9600);
  pinMode(PIN_298IN1,OUTPUT);   // pin 9
  pinMode(PIN_298IN2,OUTPUT);   // pin 4
  pinMode(PIN_298IN3,OUTPUT);   // pin 7
  pinMode(PIN_298IN4,OUTPUT);   // pin 8
  pinMode(PIN_298ENA,OUTPUT);   // pin 5 (PWM) 
  pinMode(PIN_298ENB,OUTPUT);   // pin 6(PWM)
  attachInterrupt(0, isr, LOW); //habilitamos la interrupcion 0 (pin 2) la cual llama a la funcion isr e interrumpe por Bajo
}

void loop(){
  // va a adelante 2 Segundos
  ir_adelante(100);
  delay(1000);
  // gira Izquierda 2 Segundos
  gira_izquierda(100);
  delay(1000);
  // gira Derecha 2 Segundos
  gira_derecha(100);
  delay(1000);
  // Marcha atras 2 Segundos
  ir_atras(100);
  delay(1000);
}
