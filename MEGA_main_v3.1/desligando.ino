void iniciarDesligamento()
{
  //t.after(1000*60*5,desligando1,1); //chama a função desligando1 depois de 5 minutos
  id1 = t.after(1000*10,desligando1,1);
  #ifdef debug
  Serial.println("Desligamento iniciado!");
  #endif
}

void desligando1()
{
  #ifdef debug
  Serial.println("Sem presença por 5 miuntos!");
  #endif
  beepAtivo(5, 100, 200, BUZ);
  //t.after(1000*60*1,desligando2,1); //chama a função desligando2 depois de 1 minuto
  id2 = t.after(1000*5,desligando2,1);
}
void desligando2()
{
  #ifdef debug
  Serial.println("Passou mais 1 minuto. Desligando!");
  #endif
  estadoSala = DESLIGADA;
  beepAtivo(1, 1000, 0, BUZ);
  
  digitalWrite(lamp,LOW);
  desligarAr();
  desligarProjetor();
  id3 = t.after(1000*30,desligarTomadas,1);
}
void desligarTomadas(){
  digitalWrite(tomadas,LOW);
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
