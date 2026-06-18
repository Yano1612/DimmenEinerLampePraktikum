#include <Arduino.h>

volatile bool Nulldurchgang = false;

// Pins definieren
const int PIN_TASTER1     = 0;
const int PIN_TASTER2     = 1;
const int PIN_TRIAC       = 3;  
const int PIN_ZC          = 6;  
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

  attachInterrupt(digitalPinToInterrupt(PIN_ZC_INT), RisingEdgeDetected, RISING);

}

void RisingEdgeDetected(){
  Nulldurchgang = true;
}

int durchgaenge = 0;
int lampeAn = 0;

void loop(){
  if (Nulldurchgang){ 
    durchgaenge++;
    Nulldurchgang = false;
  }

  if(lampeAn == 1){
    // Zünde
    digitalWrite(PIN_TRIAC, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIAC, LOW));
  }
  
  if(durchgaenge >= 100){
    lampeAn = (lampeAn+1)%2;
    durchgaenge = 0;
  }  
  
  
}