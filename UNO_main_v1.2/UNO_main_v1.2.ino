#define debug true

#include <SoftwareSerial.h>

#define PIR 3

volatile bool presUNO = true;
volatile bool presMEGA = true;
volatile bool presUNO_anterior = true; //estado anterior
volatile bool presMEGA_anterior = true;//estado anteiror

String comandoRecebido;

SoftwareSerial BTserial(4, 5); // RX | TX

void setup() {
  #ifdef debug
  Serial.begin(9600); //Para fins de debug
  #endif
  BTserial.begin(9600);
  presUNO = digitalRead(PIR);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
}

void loop() {
  //se estado mudar, enviar isso para o MEGA
  if(presUNO != presUNO_anterior)
  {
    if(presUNO)  BTserial.write("PU1|"); //Presença Uno = true
    else         BTserial.write("PU0|"); //Presença Uno = false
    presUNO_anterior = presUNO;
  }

  //recebendo mudança de estado do MEGA
  if( BTserial.available() )
  {
    comandoRecebido = BTserial.readStringUntil('|');
    if( comandoRecebido == "PM1" )  presMEGA = true;
    else if( comandoRecebido == "PM0" )  presMEGA = false;
    #ifdef debug
    Serial.println(comandoRecebido);
    #endif
  }
  #ifdef debug
  Serial.print("presMEGA = ");
  Serial.print(presMEGA);
  Serial.print("   presUNO = ");
  Serial.println(presUNO);
  #endif
}

void pirISR()
{
  presUNO = digitalRead(PIR);
}

