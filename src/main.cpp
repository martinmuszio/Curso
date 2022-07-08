#include <Arduino.h>

#define PIN_298ENA  5   // ENA (enable A) conectado a PIN 5
#define PIN_298ENB  6   // ENB (enable B) conectado a PIN 6
#define PIN_298IN1  9   // IN1 del L298, conectado a PIN 9
#define PIN_298IN2  4   // IN2 del L298, conectado a PIN 4
#define PIN_298IN3  7   // IN3 del L298, conectado a PIN 7 
#define PIN_298IN4  8   // IN4 del L298, Rigth Forward, conectado a PIN 8

/*  FUNCION PWM entre 0 y 100
*   la siguiente funcion, convierte valores entre 0 100
*   que puede ser float, a valores tipo byte, que puede
*   ser util por ejemplo para utilizar con analogWrite()
*   de esta forma, podriamos pasar por parametro de la
*   siguiente forma: analogWrite(pwm_0a100(variable_float))
*   la funcion devuelve un byte (0-255) dependiendo el valor
*   float de entrada. Si variable_float es menor o igual a 0 
*   devuelve 0, si es mayor o igual que 100, devuelve 255
*   y si esta entre esos valores, multiplica por 2.55*/
byte pwm_0a100(float valor){
  if(valor >= 100){
    return 255;                   // si valor es mayor o igual que 100, simplemente devuelve 255
  }else{
    if(valor <= 0){
      return 0;                   // si valor es menor o igual a 0, devuelve 0
    }else{
      return byte(valor * 2.55);  // sino devuelve el byte de la multiplicacion de valor x 2.55
    }
  }
}

/*funcion ir hacia adelante: enciende ambos motores para ir hacia adelante
* se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void ir_adelante(float speed_val){    // speed_val：0~255
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,LOW);
  digitalWrite(PIN_298IN3,HIGH); 
  digitalWrite(PIN_298IN4,LOW);
  analogWrite(PIN_298ENA, pwm_0a100(speed_val));
  analogWrite(PIN_298ENB, pwm_0a100(speed_val));
}

/*funcion ir hacia atras: enciende ambos motores para ir hacia atras
* se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void ir_atras(float speed_val){    // speed_val：0~255 
  digitalWrite(PIN_298IN1,LOW);  
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,LOW);  
  digitalWrite(PIN_298IN4,HIGH);
  analogWrite(PIN_298ENA, pwm_0a100(speed_val));
  analogWrite(PIN_298ENB, pwm_0a100(speed_val));
}

/*funcion girar a la izquierda: motor izquierdo para atras y motor derecho para adelante, 
*se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void gira_izquierda(float speed_val){        // speed_val：0~255
  digitalWrite(PIN_298IN1,LOW); 
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,HIGH);
  digitalWrite(PIN_298IN4,LOW);
  analogWrite(PIN_298ENA, pwm_0a100(speed_val));
  analogWrite(PIN_298ENB, pwm_0a100(speed_val));
}

/*funcion girar a la derecha: motor izquierdo para adelante y motor derecho para atras, 
*se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void gira_derecha(float speed_val){    // speed_val：0~255
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,LOW);  
  digitalWrite(PIN_298IN3,LOW);  
  digitalWrite(PIN_298IN4,HIGH);
  analogWrite(PIN_298ENA, pwm_0a100(speed_val));
  analogWrite(PIN_298ENB, pwm_0a100(speed_val));
}

/*funcion detener motores: motor izquierdo y derecho para apagados
* NOTA: no es necesario pasarle por parametro ningun valor
*/
void stopp(){        //stop
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
}

void loop(){

  // va a adelante a fondo durante 2 Segundos
  ir_adelante(80);
  delay(1000);
  // gira Izquierda a fondo durante 2 Segundos
  gira_izquierda(80);
  delay(1000);
  // gira Derecha a fondo durante 2 Segundos
  gira_derecha(80);
  delay(1000);
  // Marcha atras a fondo durante 2 Segundos
  ir_atras(80);
  delay(1000);
  // Para 5 Segundos
  stopp();
  delay(5000);
}