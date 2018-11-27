/*
void aguardarPresUNObreak()
{
  while (1)
  {
        #ifdef debug
        Serial.println("aguardarPresUNObreak();");
        #endif
    
    if ( Serial3.available() )
    {
      comandoRecebido = Serial3.readStringUntil('|');
      //if( comandoRecebido == "PU1" || comandoRecebido == "PU0" )Serial3.write("PUOK|");
      if ( comandoRecebido == "PU1" ) { presUNO = true; Serial3.write("PUOK|");}
      else if ( comandoRecebido == "PU0" ) { presUNO = false; Serial3.write("PUOK|");}
          #ifdef debug
          Serial.println(comandoRecebido);
          #endif
      break;
    }
  }
}
*/
