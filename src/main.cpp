#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

/* Para Tacometro*/
volatile float value=0;
volatile float count=0;
volatile int rpm;
volatile int oldtime=0;
volatile long ahora=0;
volatile long antes =0;

// Funcion Interrpcion, solo aumenta la variable count
void isr(){ //interrupt service routine
  count++;
}

void setup(){

  Serial.begin(9600);
  attachInterrupt(0, isr, RISING); //habilitamos la interrupcion 0 (pin 2) la cual llama a la funcion isr e interrumpe por cambio de estado
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print(" CURSO ROBOTICA");
  lcd.setCursor(3,1);
  lcd.print("PRACTICA 11");
  delay(3000);
  count = 0;
  
}

void loop(){

  ahora = millis();             // guardo el tiempo...
  
  if((ahora - antes) > 1000 ){  // cuando pase 1 segundo...
    antes = ahora;              // actualizo antes...
    detachInterrupt(0);         // apago la interrpcion
    rpm = count * 60 / 2;       // calculo las rpm
    count = 0;                  // borro la variable contador

    // y muestro en la pantalla
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TACOMETRO");
    lcd.setCursor(0,1);
    lcd.print(     rpm);
    lcd.print(" RPM");
    lcd.print("   ");
    // una vez, mostrado los datos, activo nuevamente la interrupcion
    attachInterrupt(0,isr,RISING);
  }
}