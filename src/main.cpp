#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo f_servo;
LiquidCrystal_I2C lcd(0x27,16,2);

#define PIN_ECHO    A0  // Pin ECHO conectado a A0
#define PIN_TRIGGER A1  // Pin TRIGGER conectado a PIN A1
#define PIN_SERVO   A2  // Pin del servo conectado a PIN A2
#define PIN_298ENA  5   // ENA (enable A) conectado a PIN 5
#define PIN_298ENB  6   // ENB (enable B) conectado a PIN 6
#define PIN_298IN1  9   // IN1 del L298, conectado a PIN 9
#define PIN_298IN2  4   // IN2 del L298, conectado a PIN 4
#define PIN_298IN3  7   // IN3 del L298, conectado a PIN 7 
#define PIN_298IN4  8   // IN4 del L298, conectado a PIN 8


volatile float D_min = 30;
volatile float distancia_frontal;
volatile float distancia_izquierda;
volatile float distancia_derecha;

volatile int vel_izq = 145;
volatile int vel_der = 125;


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


/*funcion ir hacia adelante: enciende ambos motores para ir hacia adelante
* se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void ir_adelante(unsigned char vel_izq, unsigned char vel_der){    // speed_val：0~255
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,LOW);
  digitalWrite(PIN_298IN3,HIGH); 
  digitalWrite(PIN_298IN4,LOW);
  analogWrite(PIN_298ENA,vel_izq);
  analogWrite(PIN_298ENB,vel_der);
}

/*funcion ir hacia atras: enciende ambos motores para ir hacia atras
* se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void ir_atras(unsigned char vel_izq, unsigned char vel_der){    // speed_val：0~255 
  digitalWrite(PIN_298IN1,LOW);  
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,LOW);  
  digitalWrite(PIN_298IN4,HIGH);
  analogWrite(PIN_298ENA,vel_izq);
  analogWrite(PIN_298ENB,vel_der);
}

/*funcion girar a la izquierda: motor izquierdo para atras y motor derecho para adelante, 
*se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void gira_izquierda(unsigned char vel_izq, unsigned char vel_der){        // speed_val：0~255
  digitalWrite(PIN_298IN1,LOW); 
  digitalWrite(PIN_298IN2,HIGH);
  digitalWrite(PIN_298IN3,HIGH);
  digitalWrite(PIN_298IN4,LOW);
  analogWrite(PIN_298ENA,vel_izq);
  analogWrite(PIN_298ENB,vel_der);
}

/*funcion girar a la derecha: motor izquierdo para adelante y motor derecho para atras, 
*se le pasa por parametro el valor PWM para la velocidad de los motores
*/
void gira_derecha(unsigned char vel_izq, unsigned char vel_der){    // speed_val：0~255
  digitalWrite(PIN_298IN1,HIGH);
  digitalWrite(PIN_298IN2,LOW);  
  digitalWrite(PIN_298IN3,LOW);  
  digitalWrite(PIN_298IN4,HIGH);
  analogWrite(PIN_298ENA,vel_izq);
  analogWrite(PIN_298ENB,vel_der);
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

/* Funcion deteccion de colision: en caso que el sensor de distancia,
* encuentre un obstaculo entre entre 0 y 20 cm, gira a la izquierda,
* mide la distancia izquierda, gira a la derecha, mide la distancia derecha
* y la distancia a un obstaculo mayor sera el destino de avance
*/
void deteccion_colision(){

  distancia_frontal=distancia(); // calcula la distancia
  if((distancia_frontal < D_min) && (distancia_frontal > 0)){ // si la distancia frontal esta entre 0 y D_min 
    stopp();  //frena los motores
    delay(100);
    f_servo.write(180); // mira a la izquierda, gira el servo 180°
    delay(500);
    distancia_izquierda = distancia();  //mide la distancia izquierda
    delay(100);
    f_servo.write(0); // mira a la derecha, gira el servo 180°
    delay(500);
    distancia_derecha = distancia();  //mide la distancia Derecha
    delay(100);
    if(distancia_izquierda > distancia_derecha){  //si la distancia izquierda es mayor a la derecha
      gira_izquierda(vel_izq, vel_der);   // Gira hacia la izquierda
      f_servo.write(90);  // vuelva a poner el ultrasonico en el centro
      delay(150); 
    }else{ // si la distancia izquierda es menor o igual a la derecha,
      gira_derecha(vel_izq, vel_der);  // gira a la derecha
      f_servo.write(90);  // vuelva a poner el ultrasonico en el centro
      delay(150);
    }
  }else{  // si la distancia no esta entre 0 y 20...
    ir_adelante(vel_izq, vel_der);  //va hacia adelante
  }

  lcd.setCursor(0, 0);
  lcd.print("L:");
  lcd.print(distancia_izquierda);
  lcd.print(" R:");
  lcd.print(distancia_derecha);
  lcd.setCursor(0, 1);
  lcd.print("   F:");
  lcd.print(distancia_frontal);
}

void setup(){
  f_servo.attach(PIN_SERVO);
  Serial.begin(9600);
  distancia_frontal = 0;
  distancia_izquierda = 0;
  distancia_derecha = 0;
  f_servo.write(90);
  pinMode(PIN_ECHO, INPUT);      
  pinMode(PIN_TRIGGER, OUTPUT);    
  pinMode(PIN_298IN1,OUTPUT);   //pin 9
  pinMode(PIN_298IN2,OUTPUT);   // pin 4
  pinMode(PIN_298IN3,OUTPUT);   // pin 7
  pinMode(PIN_298IN4,OUTPUT);   // pin 8
  pinMode(PIN_298ENA,OUTPUT);   // pin 5 (PWM) 
  pinMode(PIN_298ENB,OUTPUT);   // pin 6(PWM)

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("CURSO ROBOTICA");
  lcd.setCursor(3,1);
  lcd.print("   CLASE 13    ");
  
  delay(3000);
}

void loop(){
  deteccion_colision();
}