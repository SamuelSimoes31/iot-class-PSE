#define debug true

//SERIAL + BLUETOOTH
#include <SoftwareSerial.h>
SoftwareSerial BTserial(12, 11); // RX | TX
String comandoRecebido;

//INFRAVERMELHO
#define LedIR 3 

//MQ-2
#define pinMQ2 A2
#define nivel 280
int valor_analogico;

//BUZZER
#define BUZ 6

void setup(){
  #ifdef debug
    Serial.begin(9600); //Para fins de debug
  #endif
  digitalWrite(LedIR,LOW);
  BTserial.begin(9600);
  pinMode(LedIR,OUTPUT);
}

void loop(){
  if ( BTserial.available() )
  {
    comandoRecebido = BTserial.readStringUntil('|');
    Serial.println(comandoRecebido);
    if ( comandoRecebido == "OFF" ) 
    {
      for(int i=0;i<5;i++)
      {
        desligarAr();
        delay(500);
      }
    }
  }
  
  valor_analogico = analogRead(pinMQ2);
  if(valor_analogico > nivel) 
  {
    Serial.print("valor_analogico = ");
    Serial.println(valor_analogico);
    BTserial.write("FOGO|");
    #ifdef debug
      Serial.println("É FOGOOO");
    #endif
    beepAtivo(5,750,250,BUZ);
  }

  #ifdef debug
    Serial.print("valor_analogico = ");
    Serial.println(valor_analogico);
    //delay(500);
  #endif
}

void beepAtivo(int beeps, int tempoON, int tempoOFF, int pin)
{
  for(int i=0; i<beeps; i++)
  {
    digitalWrite(pin,HIGH);
    delay(tempoON);
    digitalWrite(pin,LOW);
    delay(tempoOFF);
  }
}

