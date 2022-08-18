#include <Arduino.h>

// liberia LCD
#include <LiquidCrystal_I2C.h>

// Clase LCD en direccion 0x27 de 16 caracteres x 2 renglones
LiquidCrystal_I2C lcd(0x27,16,2);

/* Para Tacometro*/
float contador = 0; // guardamos la cuenta de giros
int rpm = 0;        // el valor de rpm
long ahora = 0;     // tiempo actual
long antes = 0;     // tiempo pasado

// Funcion Interrpcion, solo aumenta la variable contador
// es llamada en caso que attachInterrupt(pin, isr, modo) ocurra
// para nuestro caso, solo aumenta la variable contador
void isr(){ //interrupt service routine
  contador++;
}

void setup(){

  // inicializamos puerto serie
  Serial.begin(9600);

  // Inicializacion de LCD
  lcd.init();                   // inicio
  lcd.backlight();              // prendemos el fondo
  lcd.setCursor(0,0);           // posicionamos el cursor x=0 e y=0
  lcd.print(" CURSO ROBOTICA"); // muestra en LCD
  lcd.setCursor(3,1);           // posicionamos en x=3 e y y=1
  lcd.print("PRACTICA 11");     // muestra LCD
  delay(3000);

  // ahora habilitaremos la interrupcion, el cuales el pin fisico 2
  // la funcion digitalPinToInterrupt(2) devuelve el numero de interrupcion
  // que pertenece al pin 2, en este caso, devolveria e 0, a su vez
  // pasamos por parametro, la funcion que va a ejecutar cuando la interrupcion
  // ocurra, y el modo en que lo va a hacer, en este caso, puede ser Falling o rising
  attachInterrupt(digitalPinToInterrupt(2), isr, RISING); 
}

void loop(){

  // guardamos el tiempo actual en la variable ahora
  ahora = millis();
  
  // si el tiempo actual menos el anterior, es mayor a 1000
  // significa que paso 1000 ms, lo cual es 1 segundo.
  if((ahora - antes) > 1000 ){
    
    // en ese caso, actualizo el tiempo:
    antes = ahora;
    // calculo las RPM, la cual sera el producto de 
    // contador por 60 segundos, y en el caso de nuestro
    // ventilador, el fabricante nos indica que por cada
    // revolucion, la salida son 2 pulsos, por lo que se
    // debera dividir por 2. 
    rpm = contador * 60 / 2;

    // a continuacion, simplemente mostraremos las RPM
    // en la pantalla
    lcd.clear();  // borra toda la pantalla
    lcd.setCursor(0,0);
    lcd.print("TACOMETRO");
    lcd.setCursor(0,1);
    lcd.print(     rpm);
    lcd.print(" RPM");
    lcd.print("   ");
    // una vez mostrada la variable, reestablezco el contador
    // para que empiece a contar nuevamente de 0.
    contador = 0;
  }
}