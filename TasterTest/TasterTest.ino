
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

}

int pressTime1 = 0;
int pressTime2 = 0;

void loop(){
  
  if(digitalRead(PIN_TASTER1) == 1){
    digitalWrite(PIN_LED1, HIGH);
    pressTime1 = millis();
  }

  if(digitalRead(PIN_TASTER2) == 2){
    digitalWrite(PIN_LED2, HIGH);
    pressTime2 = millis();
  }

  if(millis()-pressTime1 > 1000){
    digitalWrite(PIN_LED1, LOW);
  }

  if(millis()-pressTime2 > 1000){
    digitalWrite(PIN_LED2, LOW);
  }

}