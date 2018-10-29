#define debug true
//-------TIMER-----------
#include <Event.h>
#include <Timer.h>
int id1,id2;
Timer t;
//-----------------------

#define PIR 3
#define BUZ 8
#define lamp 4

volatile bool presUNO = false;
volatile bool presMEGA = true;
volatile bool presUNO_anterior = true; //estado anterior
volatile bool presMEGA_anterior = true;//estado anteiror
         int estadoSala;

enum estadosSala{LIGADA, DESLIGANDO, DESLIGADA};

String comandoRecebido;

void setup() {
  #ifdef debug
  Serial.begin(9600);
  #endif
  Serial3.begin(9600);
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  digitalWrite(lamp,HIGH);
  presMEGA = digitalRead(PIR);
  attachInterrupt(digitalPinToInterrupt(PIR), pirISR, CHANGE);
}

void loop() {
  t.update();
  //se estado mudar, enviar isso para o UNO
  if(presMEGA != presMEGA_anterior)
  {
    if(presMEGA)  Serial3.write("PM1|"); //Presença Mega = true
    else          Serial3.write("PM0|"); //Presença Mega = false
    presMEGA_anterior = presMEGA;
  }

  //recebendo mudança de estado do UNO
  if( Serial3.available() )
  {
    comandoRecebido = Serial3.readStringUntil('|');
    if( comandoRecebido == "PU1" )  presUNO = true;
    else if( comandoRecebido == "PU0" )  presUNO = false;
    #ifdef debug
    Serial.println(comandoRecebido);
    #endif
  }

  if( !( presMEGA || presUNO ) && estadoSala == LIGADA )
  {
    estadoSala = DESLIGANDO;
    Serial3.write("PM0|");
    iniciarDesligamento();
  }
  else if(( presMEGA || presUNO ) && estadoSala == DESLIGANDO)
  {
      estadoSala = LIGADA;
      t.stop(id1);
      t.stop(id2);
  }
  else if(( presMEGA || presUNO ) && estadoSala == DESLIGADA)
  {
    estadoSala = LIGADA;
    digitalWrite(lamp,HIGH);
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
  presMEGA = digitalRead(PIR);
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

