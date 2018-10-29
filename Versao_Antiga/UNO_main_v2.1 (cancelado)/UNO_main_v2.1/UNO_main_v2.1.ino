#define debug true

#include <SoftwareSerial.h>

#define PIR 3

//-------INFRARED----------------
#define LedIR 11

volatile bool presUNO;
volatile bool presUNO_anterior; //estado anterior
         bool presUNOok = false;

String comandoRecebido;

SoftwareSerial BTserial(4, 5); // RX | TX

void setup() {
  #ifdef debug
  Serial.begin(9600); //Para fins de debug
  #endif
  BTserial.begin(9600);
  presUNO = digitalRead(PIR);
  presUNO_anterior = presUNO;
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);

  while(!presUNOok)
  {
    if(presUNO)  BTserial.write("PU1|"); //Presença Uno = true
    else         BTserial.write("PU0|"); //Presença Uno = false

    if( BTserial.available() )
    {
        comandoRecebido = BTserial.readStringUntil('|');
        if( comandoRecebido == "PUOK" )  presUNOok = true;
        #ifdef debug
        Serial.println(comandoRecebido);
        #endif
    }
    delay(250);
  }
}

void loop() {
  //se estado mudar, enviar isso para o MEGA
  if(presUNO != presUNO_anterior)
  {
    if(presUNO)  BTserial.write("PU1|"); //Presença Uno = true
    else         BTserial.write("PU0|"); //Presença Uno = false
    presUNO_anterior = presUNO;
  }

  #ifdef debug
  Serial.print("   presUNO = ");
  Serial.println(presUNO);
  #endif
}

void pirISR()
{
  presUNO = digitalRead(PIR);
}

