
#include <Arduino.h>
//TODO Zeiten zu unsinged long, Maximal und minimalwert richtig legen (maybe 5° bis 35°)

// Pins initialisieren
const int PIN_TASTER1     = 0;
const int PIN_TASTER2     = 1;
const int PIN_TRIAC       = 3;  
const int PIN_ZC_INT      = 9;  
const int PIN_LED1        = 14;
const int PIN_LED2        = 15;


void setup(){
  // Pins konfigurieren
  pinMode(PIN_TASTER1, INPUT);
  pinMode(PIN_TASTER2, INPUT);
  pinMode(PIN_TRIAC, OUTPUT);
  pinMode(PIN_ZC_INT, INPUT);
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  digitalWrite(PIN_TRIAC, LOW);
  digitalWrite(PIN_LED1, LOW);
  digitalWrite(PIN_LED2, LOW);

  //Interrupt für den PIN
  attachInterrupt(digitalPinToInterrupt(PIN_ZC_INT), RisingEdgeDetected, RISING);
}

// Nulldurchgang erkennen
volatile bool Nulldurchgang = false;
int Nullzeit = 0;

int lastpress = 0; // zeit des letzten knopfdrucks

void RisingEdgeDetected(){
  Nulldurchgang = true;
}


float Zuendwinkel = 90.0;
float Periodendauer = 20000.0; // 50 hz in Microsekunden

// Ob Diese Periode aktiviert wurde
bool activated = false;

//Fürs Blinken
bool blink = true;
int durchgaenge = 0;
int lampeAn = 1;

void loop(){
  // speichere Zeit des Nulldurchgangs
  if(Nulldurchgang) {
    Nullzeit = micros();
    durchgaenge++;
    Nulldurchgang = false;
    activated = false;
  }
                                
  if(micros()-Nullzeit>= Zuendwinkel/360.0 * Periodendauer && !activated && lampeAn == 1){ // Wenn seit dem letzten nulldurchgang Zuendwinkel/360 % der periodendauer vergangen sind und diese Periode noch nicth aktiviert wurde:
    digitalWrite(PIN_TRIAC, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIAC, LOW);
    activated = true;
  }

                                    // Sichern, dass ein Press nicht mehrmals registriert wird
  if (digitalRead(PIN_TASTER2) == 1 && millis()- lastpress > 50){
    if (Zuendwinkel < 175){ // Maximalwert
      Zuendwinkel += 5.0;
    }
    lastpress = millis();
  }
  if (digitalRead(PIN_TASTER1) == 1 && millis()- lastpress > 50){
    if (Zuendwinkel > 5){ // Minimalwert
      Zuendwinkel -= 5.0;
    }
    lastpress = millis();
  }

  if(blink){

    if(durchgaenge >= 100){
      lampeAn = (lampeAn+1)%2;
      durchgaenge = 0;
    }  
  }
}