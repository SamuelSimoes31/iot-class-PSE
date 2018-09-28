volatile bool presenca;


void setup() {
  

}

void loop() {


}

void pir_isr()
{
  presenca = digitalRead(PIR);
}

