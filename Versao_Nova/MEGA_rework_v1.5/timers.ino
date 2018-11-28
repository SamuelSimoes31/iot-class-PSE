void iniciarDesligamento()
{
  estadoSala = DESLIGANDO_LAMPADAS;
  id1 = t.after(1000*60*3,desligando1,1);
  //chama a função desligando1 depois de 3 minutos
  #ifdef debug
    Serial.println("Desligamento iniciado!");
  #endif
}

void desligando1()
{
  #ifdef debug
    Serial.println("Sem presença por 3 minutos!");
  #endif
  beepAtivo(3, 100, 200, BUZ);
  id2 = t.after(1000*20,desligando2,1);
  //chama a função desligando2 depois de 20 segundos
}
void desligando2()
{
  #ifdef debug
    Serial.println("Desligando lampadas!");
  #endif
  estadoSala = DESLIGANDO_TOMADAS;
  beepAtivo(1, 1000, 0, BUZ); //avisa que vai desligar
  digitalWrite(lamp,LOW); //desliga as lâmpadas
  BTserial.write("OFF|"); //manda o arduino 2 desligar o ar condicionado
  id3 = t.after(1000*60*5*TT,desligarTomadas,1); 
}
void desligarTomadas(){
  #ifdef debug
    Serial.println("Desligando tomadas!");
  #endif
  beepAtivo(3, 1000, 0, BUZ);
  digitalWrite(tomadas,LOW);
  estadoSala == DESLIGADA;
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
