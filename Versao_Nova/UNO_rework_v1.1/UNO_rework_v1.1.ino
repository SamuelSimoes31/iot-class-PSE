#define debug true

//SERIAL
#include <SoftwareSerial.h>
SoftwareSerial BTserial(4, 5); // RX | TX

//BLUETOOTH
String comandoRecebido;
//#define BTstatusPIN 2
//volatile bool BTstatus = false;

//SAÍDAS
#define LedIR 13 //MUDAR PARA 11 DEPOIS

void setup(){
  #ifdef debug
  Serial.begin(9600); //Para fins de debug
  digitalWrite(LedIR,LOW);
  #endif
  BTserial.begin(9600);
}

void loop(){
  if ( BTserial.available() )
  {
    comandoRecebido = BTserial.readStringUntil('|');
    if ( comandoRecebido == "OFF" ) desligarAr();
  }
}

