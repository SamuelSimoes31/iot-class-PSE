#define debug true

//SAÍDAS
#define BUZ 6
#define lamp 9
#define tomadas 8

//PIR
#define PIR1 3
#define PIR2 2
enum estadosSala{LIGADA, DESLIGANDO_LAMPADAS, DESLIGANDO_TOMADAS, DESLIGADA};
volatile bool pres1; //variável de presença do PIR 1
volatile bool pres2; //variável de presença do PIR 2
volatile bool pres1_anterior; //estado anterior do PIR 1
volatile bool pres2_anterior; //estado anteiror do PIR 2
         int estadoSala = DESLIGADA; //variável que define em qual estado a sala está
         
//TIMER
#include <Event.h>
#include <Timer.h>
int id1,id2,id3; //IDs dos timers utilizados na aba "timers"
Timer t; //o objeto Timer

//BOTÃO
#define botao 12 //pino do botão de adicionar mais tempo para desligar as bancadas
bool flagBotao; //flag para identificar se o botão foi pressionado
int TT=1;       //variável para incrementar o tempo para desligar as bancadas


//SERIAL + BLUETOOTH
#include <SoftwareSerial.h>
SoftwareSerial BTserial(11, 10); // RX | TX
String comandoRecebido;

void setup() {
  #ifdef debug
    Serial.begin(9600);
  #endif
  //definindo as saídas
  pinMode(BUZ,OUTPUT);
  pinMode(lamp,OUTPUT);
  //digitalWrite(lamp,HIGH);
  pinMode(tomadas,OUTPUT);
  //digitalWrite(tomadas,HIGH);

  //definindo as entradas
  pinMode(botao,INPUT_PULLUP);
  pinMode(PIR1,INPUT);
  pinMode(PIR2,INPUT);
  pirISR1(); //atualizando o estado atual do PIR 1
  pres1_anterior = pres1;
  pirISR2(); //atualizando o estado atual do PIR 2
  pres2_anterior = pres2;
  attachInterrupt(digitalPinToInterrupt(PIR1), pirISR1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIR2), pirISR2, CHANGE);

  BTserial.begin(9600);
}

void loop() {
  t.update(); //função para atualizar os timers
  
  //BLOCO DE ANÁLISE DE PRESENÇA E CONTROLE DA ENERGIZAÇÃO
  if( !( pres1 || pres2 ) && estadoSala == LIGADA )
  {
    iniciarDesligamento(); //inicia o tempo para desligamneto da sala
  }
  else if(( pres1 || pres2 ) && (estadoSala == DESLIGANDO_LAMPADAS ||
        estadoSala == DESLIGANDO_TOMADAS || estadoSala == DESLIGADA) )
  {
    #ifdef debug
      switch(estadoSala)
      {
        case DESLIGADA: Serial.println("RELIGANDO!");break;
        case DESLIGANDO_LAMPADAS: Serial.println("DESLIGAMENTO DAS LÂMPADAS CANCELADO!");break;
        case DESLIGANDO_TOMADAS: Serial.println("DESLIGAMENTO DAS TOMADAS CANCELADO!");break;
      }
    #endif
    estadoSala = LIGADA;
    TT = 1; //reinicia TT
    t.stop(id1); //para os timers
    t.stop(id2);
    t.stop(id3);
    digitalWrite(lamp,HIGH); //liga o que tiver sido desligado
    digitalWrite(tomadas,HIGH);
  }

  //BLOCO DO BLUETOOTH PARA RECEBER AVISO DO INCÊNDIO
  if(BTserial.available())
  {
    comandoRecebido = BTserial.readStringUntil('|');
    #ifdef debug
      Serial.println(comandoRecebido);
    #endif
    if ( comandoRecebido == "FOGO" ) beepAtivo(5,750,250,BUZ);
  }

  //BLOCO DE TRATAMENTO DO BOTÃO
  if(digitalRead(botao)) flagBotao = 0;
  if(!digitalRead(botao) && flagBotao == 0)
  {
    flagBotao = 1;
    TT++;
    beepAtivo(TT, 200, 200, BUZ);
  }
  
  #ifdef debug
    Serial.print("estadoSala = ");
    Serial.print(estadoSala);
    Serial.print("  |  pres1 = ");
    Serial.print(pres1);
    Serial.print("  |  pres2 = ");
    Serial.print(pres2);
    Serial.print("  |  TT = ");
    Serial.println(TT);
    //delay(1000);
  #endif
}

//ROTINAS DE TRATAMENTO DE INTERRUPÇÃO(ISRs)
void pirISR1()
{
  pres1 = digitalRead(PIR1);
}

void pirISR2()
{
  pres2 = digitalRead(PIR2);
}
