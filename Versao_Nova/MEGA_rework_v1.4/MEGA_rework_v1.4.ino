#define debug true

//SAÍDAS
#define BUZ 6
#define lamp 11  //MUDAR PARA 4
#define tomadas 10

//PIR
#define PIR1 3
#define PIR2 2
enum estadosSala{LIGADA, DESLIGANDO_LAMPADAS, DESLIGANDO_TOMADAS, DESLIGADA};
volatile bool pres1;
volatile bool pres2;
volatile bool pres1_anterior; //estado anterior
volatile bool pres2_anterior;//estado anteiror
         int estadoSala = DESLIGADA;

//BLUETOOTH
String comandoRecebido;

//TIMER
#include <Event.h>
#include <Timer.h>
int id1,id2,id3;
Timer t;

//BOTÃO
#define botao 8
bool flagBotao;
int TT=1;


//SERIAL
#include <SoftwareSerial.h>


void setup() {
  #ifdef debug
  Serial.begin(9600);
  #endif
  
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  //digitalWrite(lamp,HIGH);
  pinMode(tomadas,OUTPUT);
  //digitalWrite(tomadas,HIGH);

  pinMode(botao,INPUT_PULLUP);
  
  pinMode(PIR1,INPUT);
  pinMode(PIR2,INPUT);
  pirISR1();
  pres1_anterior = pres1;
  pirISR2();
  pres2_anterior = pres2;
  attachInterrupt(digitalPinToInterrupt(PIR1), pirISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIR2), pirISR2, CHANGE);

  //Serial3.begin(9600);
}

void loop() {
  t.update();
  if( !( pres1 || pres2 ) && estadoSala == LIGADA )
    {
      estadoSala = DESLIGANDO_LAMPADAS;
      iniciarDesligamento();
    }
    else if(( pres1 || pres2 ) && (estadoSala == DESLIGANDO_LAMPADAS || estadoSala == DESLIGANDO_TOMADAS || estadoSala == DESLIGADA) )
    {
        switch(estadoSala)
        {
          case DESLIGADA: Serial.println("RELIGANDO!");break;
          case DESLIGANDO_LAMPADAS: Serial.println("DESLIGAMENTO DAS LÂMPADAS CANCELADO!");break;
          case DESLIGANDO_TOMADAS: Serial.println("DESLIGAMENTO DAS TOMADAS CANCELADO!");break;
        }
        estadoSala = LIGADA;
        TT = 1;
        t.stop(id1);
        t.stop(id2);
        t.stop(id3);
        digitalWrite(lamp,HIGH);
        digitalWrite(tomadas,HIGH);
        
    }
    else if(( pres1 || pres2 ) && estadoSala == DESLIGADA)
    {
      estadoSala = LIGADA;
      digitalWrite(lamp,HIGH);
      digitalWrite(tomadas,HIGH);
    }

    //if(Serial3.available())
    //{
    //  comandoRecebido = Serial3.readStringUntil('|');
    //  Serial.println(comandoRecebido);
    //  if ( comandoRecebido == "FOGO" ) beepAtivo(5,750,250,BUZ); //incendio();
    //}
    if(digitalRead(botao)) flagBotao = 0;
    if(!digitalRead(botao) && flagBotao == 0)
    {
      flagBotao = 1;
      TT++;
      beepAtivo(TT, 200, 200, BUZ);
      Serial.println(TT);
    }
    
    Serial.print("estadoSala = ");
    Serial.print(estadoSala);
    Serial.print("  |  pres1 = ");
    Serial.print(pres1);
    Serial.print("  |  pres2 = ");
    Serial.print(pres2);
    Serial.print("  |  TT = ");
    Serial.println(TT);
    //delay(1000);
}

void pirISR1()
{
  pres1 = digitalRead(PIR1);
}

void pirISR2()
{
  pres2 = digitalRead(PIR2);
}





