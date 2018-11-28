#define debug true

//SERIAL + BLUETOOTH
#include <SoftwareSerial.h>
SoftwareSerial BTserial(4, 5); // RX | TX
String comandoRecebido;

//INFRAVERMELHO
#define LedIR 13 //MUDAR PARA 11 DEPOIS

//MQ-2
#define pinMQ2 A2
#define nivel 230
int valor_analogico;

void setup(){
  #ifdef debug
    Serial.begin(9600); //Para fins de debug
  #endif
  digitalWrite(LedIR,LOW);
  BTserial.begin(9600);
}

void loop(){
  if ( BTserial.available() )
  {
    comandoRecebido = BTserial.readStringUntil('|');
    Serial.println(comandoRecebido);
    if ( comandoRecebido == "OFF" ) desligarAr();
  }
  
  valor_analogico = analogRead(pinMQ2);
  if(valor_analogico > nivel) 
  {
    BTserial.write("FOGO|");
    #ifdef debug
      Serial.println("É FOGOOO");
    #endif
  }

  #ifdef debug
    Serial.print("valor_analogico = ");
    Serial.println(valor_analogico);
    //delay(500);
  #endif
}

