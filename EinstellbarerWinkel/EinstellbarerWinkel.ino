
#include <Arduino.h>

// Pins initialisieren
const int PIN_TASTER1     = 0;
const int PIN_TASTER2     = 1;
const int PIN_TRIAC       = 3;  
const int PIN_ZC_INT      = 9;  
const int PIN_LED1        = 14;
const int PIN_LED2        = 15;




float Zuendwinkel = 90.0;
float Periodendauer = 20000.0; // 50 hz in Microsekunden

// Ob Diese Periode aktiviert wurde
bool activated = false;


// Nulldurchgang erkennen
volatile bool Nulldurchgang = false;
unsigned long Nullzeit = 0;

unsigned long lastpress = 0; // zeit des letzten knopfdrucks

bool blink = true;
int durchgaenge = 0;
int lampeAn = 1;


void RisingEdgeDetected(){
  Nulldurchgang = true;
}

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

void loop(){
  // speichere Zeit des Nulldurchgangs
  if(Nulldurchgang) {
    Nullzeit = micros();
    if (blink){
      durchgaenge++; // Ein neuer durchgang erkannt
    }
    Nulldurchgang = false;
    activated = false;
  }

  

  // warte zündwinkel ab und Zünde dann                              
  if(micros()-Nullzeit>= Zuendwinkel/360.0 * Periodendauer && !activated && lampeAn == 1){ // Wenn seit dem letzten nulldurchgang Zuendwinkel/360 % der periodendauer vergangen sind und diese Periode noch nicth aktiviert wurde:
    // Zündung
    digitalWrite(PIN_TRIAC, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIAC, LOW);
    // Verhindern des doppelten Zündens innerhalb einer Halbwelle
    activated = true;
  }


  // Zündwinkel mit Taster steuern
                                    // Sichern, dass ein Press nicht mehrmals registriert wird
  if (digitalRead(PIN_TASTER2) == 1 && millis()- lastpress > 500){
    if (Zuendwinkel < 175){ // Maximalwert
      Zuendwinkel += 5.0;
    }
    lastpress = millis();
  }
  if (digitalRead(PIN_TASTER1) == 1 && millis()- lastpress > 500){
    if (Zuendwinkel > 5){ // Minimalwert
      Zuendwinkel -= 5.0;
    }
    lastpress = millis();
  }



  // Steuere das Blinken
  if(blink){
    // alle 50 Perioden einmal den zustand (an / aus) der Lampe wechseln --> triac zündet nur wenn lampeAn == 1 ist
    if(durchgaenge >= 100){
      // Wechseln des Wertes
      lampeAn = (lampeAn+1)%2;
      durchgaenge = 0;
    }  
  }
}