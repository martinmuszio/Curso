#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

/* Para Tacometro*/
float contador = 0; // guardamos la cuenta de giros
int rpm = 0;        // el valor de rpm
long ahora = 0;     // tiempo actual
long antes = 0;     // tiempo pasado

// Funcion Interrupcion, solo aumenta la variable contador
// es llamada en caso que attachInterrupt(pin, isr, modo) ocurra
// para nuestro caso, solo aumenta la variable contador
void isr(){ //interrupt service routine
  contador++;
}

void setup(){

 lcd.init();
  
  // ahora habilitaremos la interrupcion, el cuales el pin fisico 2
  // la funcion digitalPinToInterrupt(2) devuelve el numero de interrupcion
  // que pertenece al pin 2, en este caso, devolveria e 0, a su vez
  // pasamos por parametro, la funcion que va a ejecutar cuando la interrupcion
  // ocurra, y el modo en que lo va a hacer, en este caso, puede ser Falling o rising
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr, FALLING);
}

void loop(){

  // guardamos el tiempo actual en la variable ahora
  ahora = millis();
  
  // si el tiempo actual menos el anterior, es mayor a 1000
  // significa que paso 1000 ms, lo cual es 1 segundo.
  if((ahora - antes) >= 1000 ){
    
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
    lcd.clear();
    //lcd.setCursor(0,0);
    lcd.print("TACOMETRO: ");
    //lcd.setCursor(10,0);
    lcd.print(rpm);
    // una vez mostrada la variable, reestablezco el contador
    // para que empiece a contar nuevamente de 0.
    contador = 0;
  }
}