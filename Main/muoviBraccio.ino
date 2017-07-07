//set di funzioni finalizzate al corretto movimento del braccio e della mano. Infatti è sconsigliabile muovere il braccio senza utilizzare l'intermedio perchè potrebbe comportare 
//forzature sulle giunture. Il punto intermedio è stato studiato per consentire al braccio di estendersi al massimo e poi di potersi abbassare successivamente arrivando a qualsiasi target.


//funzione che partendo da casa va e a seconda dell argomento (immettere solo 0 e 114) prende o lascia un oggetto e ritorna a casa
void muoviBraccioA(const int m1, const int m2){
  #ifdef FUN 
  Serial.print(" muoviBraccioA "); 
  #endif //comando di debug selettivo
  M1(INTERMEDIO_M1);
  M2(INTERMEDIO_M2);
  delay(25);//vado a intermedio
  M2(m2);
  M1(m1);
  delay(25);//vado a target
}


//porta a casa il braccio (ritrae il braccio)
void muoviBraccioACasa(){
  #ifdef FUN 
  Serial.print(" muoviBraccioACasa "); 
  #endif //comando di debug selettivo
  M1(INTERMEDIO_M1);
  M2(INTERMEDIO_M2);
  delay(25);//vado a intermedio
  M2(CASA2);
  M1(CASA1);
  delay(25);//vado a casa
}

//porta il braccio all'intermedio e si ferma così
void muoviBraccioAIntermedio(){
  #ifdef FUN 
  Serial.print(" muoviBraccioAIntermedio "); 
  #endif //comando di debug selettivo
  M1(INTERMEDIO_M1);
  M2(INTERMEDIO_M2);
  delay(25);//vado a intermedio
}



//funzione che muove la mano per prendere un oggetto metallico
void acchiappa(){
 kerrEn=1;//attivo correzione errore di acchiappo
 mano.write(50);
 delay(75);
}

//funzione che muove la mano per lasciare un oggetto metallico
void molla(){
 kerrEn=0;//disattivo correzione errore di acchiappo
 mano.write(160);
 delay(75); 
}
