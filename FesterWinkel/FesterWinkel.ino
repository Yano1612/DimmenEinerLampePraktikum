
#include <Arduino.h>


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

  
  attachInterrupt(digitalPinToInterrupt(PIN_ZC_INT), RisingEdgeDetected, RISING)
}

bool Nulldurchgang = false;
void RisingEdgeDetected(){
  Nulldurchgang = true;
}

float Zündwinkel = 90.0;
float Periodendauer = 20.0;

void loop(){
  if(Nulldurchgang){
    digitalWrite(PIN_TRIAC, LOW);
    delay(Zündwinkel/360.0 * Periodendauer); // /360 * Periodendauer --> /360 entspricht prozentuellen Anteil der periodendauer * Periodendauer
    digitalWrite(PIN_TRIAC, HIGH);
    delayMicroseconds(10) // Nur kurz anmachen, damit der TRIAC einrasten kann
    digitalWrite(PIN_TRIAC, LOW);
    Nulldurchgang = false;
  }
}