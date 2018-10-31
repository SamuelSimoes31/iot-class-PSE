#define debug true

//SAÍDAS
#define BUZ 8
#define lamp 13  //MUDAR PARA 4
#define tomadas 5
#define LedIR 11

//PIR
#define PIR1 2
#define PIR2 3
enum estadosSala{LIGADA, DESLIGANDO, DESLIGADA};
volatile bool pres1;
volatile bool pres2;
volatile bool pres1_anterior; //estado anterior
volatile bool pres2_anterior;//estado anteiror
         int estadoSala = DESLIGADA;

//BLUETOOTH
String comandoRecebido;
volatile bool BTstatus = false;

//TIMER
#include <Event.h>
#include <Timer.h>
int id1,id2,id3,id4;
Timer t;

void setup() {
  #ifdef debug
  Serial.begin(9600);
  #endif
  
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  //digitalWrite(lamp,HIGH);
  pinMode(tomadas,OUTPUT);
  //digitalWrite(tomadas,HIGH);
  pinMode(LedIR, OUTPUT);

  pinMode(PIR1,INPUT);
  pinMode(PIR2,INPUT);
  pirISR1();
  pres1_anterior = pres1;
  pirISR2();
  pres2_anterior = pres2;
  attachInterrupt(digitalPinToInterrupt(PIR1), pirISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIR2), pirISR2, CHANGE);

  Serial3.begin(9600);
}

void loop() {
  t.update();
  if( !( pres1 || pres2 ) && estadoSala == LIGADA )
    {
      estadoSala = DESLIGANDO;
      iniciarDesligamento();
    }
    else if( ( pres1 || pres2 ) && estadoSala == LIGADA)
    {
      detachInterrupt(digitalPinToInterrupt(PIR1));
      detachInterrupt(digitalPinToInterrupt(PIR2));
      id4 = desativarDeteccao();
    }
    else if(( pres1 || pres2 ) && estadoSala == DESLIGANDO)
    {
        Serial.println("DESLIGAMENTO CANCELADO!");
        estadoSala = LIGADA;
        t.stop(id1);
        t.stop(id2);
        t.stop(id3);
    }
    else if(( pres1 || pres2 ) && estadoSala == DESLIGADA)
    {
      estadoSala = LIGADA;
      digitalWrite(lamp,HIGH);
      digitalWrite(tomadas,HIGH);
    }

    if(Serial3.available())
    {
      comandoRecebido = Serial3.readStringUntil('|');
      Serial.println(comandoRecebido);
      if ( comandoRecebido == "FOGO" ) beepAtivo(5,750,250,BUZ); //incendio();
    }
     
    Serial.print("estadoSala = ");
    Serial.print(estadoSala);
    Serial.print("  |  pres1 = ");
    Serial.print(pres1);
    Serial.print("  |  pres2 = ");
    Serial.println(pres2);
    //delay(1000);
}

void pirISR1()
{
  pres1 = digitalRead(PIR1);
}

void pirISR2()
{
  pres2 = digitalRead(PIR1);
}
