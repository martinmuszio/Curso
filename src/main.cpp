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

#define servo_izquierda f_servo.write(180)  // Servo Izquierda
#define servo_derecha   f_servo.write(0);   // Servo Derecha
#define servo_frontal   f_servo.write(90);  // Servo Frontal

const int D_min = 30; // distancia minima
const int delay_giro = 400;
const int delay_atras = 500;
const int delay_servo = 400;
const int delay_wat = 0;
float distancia_frontal;
float distancia_izquierda;
float distancia_derecha;

long ahora = 0;     // tiempo actual
long antes = 0;     // tiempo pasado

int vel_izq = 110;
int vel_der = 100;

volatile int ac_izquierdo;
volatile int ac_derecho;



/* Interrupcion Externa, ENCODER IZQUIERDO PIN3*/
void isr_motor_izquierdo(){ //interrupt service routine
  ac_izquierdo ++;
}

/* Interrupcion Externa, ENCODER DERECHO PIN2*/
void isr_motor_derecho(){ //interrupt service routine
  ac_derecho ++;
}

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

  distancia_frontal = distancia(); // calcula la distancia
  if((distancia_frontal > 0) && (distancia_frontal < D_min)){ // si la distancia frontal esta entre 0 y D_min 
    stopp();  //frena los motores
    delay(delay_wat);
    servo_izquierda; // mira a la izquierda, gira el servo 180°
    delay(delay_servo);
    distancia_izquierda = distancia();  //mide la distancia izquierda
    delay(delay_wat);
    servo_derecha; // mira a la derecha, gira el servo 180°
    delay(delay_servo);
    distancia_derecha = distancia();  //mide la distancia Derecha
    delay(delay_wat);
    
    if((distancia_derecha < D_min) && (distancia_izquierda < D_min)){
      // en este caso, ambos extremos estan demasiado cerca... debera ir para atras.
      ir_atras(vel_izq, vel_der);
      delay(delay_atras);
    }else{
      if(distancia_izquierda > distancia_derecha){  //si la distancia izquierda es mayor a la derecha
        gira_izquierda(vel_izq, vel_der);   // Gira hacia la izquierda
        servo_frontal;  // vuelva a poner el ultrasonico en el centro
        delay(delay_giro); 
      }else{ // si la distancia izquierda es menor o igual a la derecha,
        gira_derecha(vel_izq, vel_der);  // gira a la derecha
        servo_frontal;  // vuelva a poner el ultrasonico en el centro
        delay(delay_giro);
      }
    }
  }else{  // si la distancia no esta entre 0 y 20...
    ir_adelante(vel_izq, vel_der);  //va hacia adelante
  }
}

void setup(){

  pinMode(PIN_ECHO, INPUT);      
  pinMode(PIN_TRIGGER, OUTPUT);    
  pinMode(PIN_298IN1,OUTPUT);   //pin 9
  pinMode(PIN_298IN2,OUTPUT);   // pin 4
  pinMode(PIN_298IN3,OUTPUT);   // pin 7
  pinMode(PIN_298IN4,OUTPUT);   // pin 8
  pinMode(PIN_298ENA,OUTPUT);   // pin 5 (PWM) 
  pinMode(PIN_298ENB,OUTPUT);   // pin 6(PWM)
  pinMode(2,INPUT_PULLUP);      // PIN 2 Interrpcion Rueda Derecha
  pinMode(3,INPUT_PULLUP);      // PIN 3 Interrpcion Rueda Izquierda
  f_servo.attach(PIN_SERVO);
  servo_frontal;
  lcd.init();
  lcd.backlight();

  attachInterrupt(digitalPinToInterrupt(2), isr_motor_derecho, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), isr_motor_izquierdo, FALLING);
}

void loop(){
  deteccion_colision();
  // guardamos el tiempo actual en la variable ahora
  ahora = millis();
  if((ahora - antes) >= 1000 ){
    antes = ahora;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(ac_izquierdo);
    lcd.setCursor(0, 1);
    lcd.print(distancia_izquierda);

    lcd.setCursor(5, 0);
    lcd.print(distancia_frontal);

    lcd.setCursor(14, 0);
    lcd.print(ac_izquierdo);
    lcd.setCursor(10, 1);
    lcd.print(distancia_derecha);

    ac_izquierdo = 0;  //  reset counter to zero
    ac_derecho = 0;  //  reset counter to zero
  }
}