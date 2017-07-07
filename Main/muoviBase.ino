//programma con lo scopo di pilotare il motore di base e rispettare una data tolleranza di errore E
//questa versione differisce dalla standard perché non modula la potenza in base a i punti di inceppo ma invece cerca di compensare una spinta elevata con un algoritmo di frenata.
//il rischio di questa versione è che solleciti troppo le giunture del braccio che subiscono la leva creata dall'inerzia dello spostamento


/*

Ho un angolo e devo fare in modo che il braccio si orienti in quella posizione.


Per prima cosa controllo che l'angolo target sia entro il raggio di azione del braccio: OK
Se non è nel raggio di azione stampo errore e esco dalla funzione  OK
Se è nel raggio di azione valuto in che direzione mi devo girare:  OK
Imposto i pin del ponte h per muovere in quella direzione  OK
leggo il potenziometro di base fino a quando non leggo il valore voluto  OK
Imposto i pin del ponte h per muovere nella direzione opposta  OK
disabilito movimento al ponte H (pin enable LOW)  OK
controllo se lo spostamento eseguito rispetti la tolleranza  OK 
in caso favorevole fine funzione  OK
in caso contrario richiamare la funzione con gli stessi argomenti  OK  

NB: angoloAttuale è una funzione quindi si aggiorna tutte le volte che viene letta quindi è compatibile con un ciclo while di questo tipo



*/
//funzione che porta il braccio all'angolo target ang con una tolleranza E di errore
void servoBase(const int ang){
  #ifdef FUN 
  Serial.print(" servoBase " );  
  #endif  //comando di debug selettivo 
  if(ang<MIN_BASE || ang>MAX_BASE){//se l'angolo è minore di quello minimo e maggiore di quello massimo 
     Serial.println("Errore angolo Delta immesso fuori dominio !!!");//stampo errore
     return;//esco dalla funzione
  }
  if(ang-E<angoloAttuale() && angoloAttuale()<ang+E){//se mi trovo gia posizionato correttamente (angolo attuale compreso tra i due errori consentiti)
    kerr=-1*(ang-angoloAttuale());//se voglio andare a 30 ma vado a trentuno l'errore k è 1;se fossi andato a 29 era -1
    return;//termina funzione
  }
  if(ang<angoloAttuale()){//se l'angolo target è minore di quello attuale
    while(ang+E<angoloAttuale())//fintanto che rimane minore l'angolo target piu l'errore
      decrementa_ang_base(VEL_M_BASE);//continuo a decrementare
    ferma_decremento();//quando raggiungo l'angolo necessario fermo i motori
  }
  else{
    while(ang-E>angoloAttuale())//fintanto che l'angolo target più l'errore sono maggiori dell'angolo attuale 
      incrementa_ang_base(VEL_M_BASE);//continuo a incrementare
    ferma_incremento();//quando raggiungo l'angolo necessario fermo i motori
  }
  if(!(ang-E<angoloAttuale() && angoloAttuale()<ang+E))//se non rispetta la tolleranza l'angolo attuale
    servoBase(ang);//richiamo la funzione e ricomincio
}

//funzioni secondarie

//funzione che legge il valore di POTBASE e lo converte nel angolo in cui è girato il braccio 
int angoloAttuale(){
 #ifdef FUN_BASE 
 Serial.print(" Leggi_base/angoloAttuale " );  
 #endif  //comando di debug selettivo 
 int lettura=map(analogRead(POTBASE),0,1023,0,270);//ottengo lettura del potenziometro sotto forma di angolo
 #ifdef LEGGI_BASE 
 Serial.print(" Lettura base:"); Serial.print(lettura); 
 #endif  //comando di debug selettivo
 return lettura;//restituisco angolo
}



//funzioni di movimento basilari

//guardando il braccio da dietro si sposterà a destra
void incrementa_ang_base(const int velocita){
  #ifdef FUN_BASE 
  Serial.print(" incrementa/destra " );  
  #endif  //comando di debug selettivo
  analogWrite(GAS,velocita);
  digitalWrite(DIR2,HIGH); 
  digitalWrite(DIR1,LOW);
}

//guardando il braccio da dietro si sposterà a sinistra
void decrementa_ang_base(const int velocita){
  #ifdef FUN_BASE 
  Serial.print(" decrementa/sinistra " );  
  #endif  //comando di debug selettivo
  analogWrite(GAS,velocita);
  digitalWrite(DIR1,HIGH);
  digitalWrite(DIR2,LOW);
}

//funzione che ferma il braccio dopo aver incrementato
void ferma_incremento(){
  #ifdef FUN_BASE 
  Serial.print(" ferma_incremento " );  
  #endif  //comando di debug selettivo
  #ifdef FRENO
  decrementa_ang_base(VEL_STOP);//resisto all'inerzia
  delay(TEMP_STOP);//aspetto che si blocchi
  #endif   //comando di debug selettivo
  digitalWrite(GAS,LOW);//disattivo ponte H
}


//funzione che ferma il braccio dopo aver decrementato
void ferma_decremento(){
  #ifdef FUN_BASE 
  Serial.print(" ferma_decremento " );  
  #endif  //comando di debug selettivo
  #ifdef FRENO
  incrementa_ang_base(VEL_STOP);//resisto all'inerzia
  delay(TEMP_STOP);//aspetto che si blocchi
  #endif //comando di debug selettivo
  digitalWrite(GAS,LOW);//disattivo ponte H
}




































/*
int ang_prec=MIN_BASE-100;//inizializzo a un' angolo fuori range l'angolo precedente perchè la prima volta devo necessariamente muovere il braccio

void servoBase(int ang) {
  int esci=0;//finche non diventa 1 non posso uscire dall loop (vuol dire che non ho ancora raggiunto la precisione voluta)
  #ifdef DEBASE
	  Serial.print("  angolo virtuale=");Serial.print(ang);//debug  //stampo angolo richiesto con etichetta
    Serial.print("  angolo reale=");Serial.println(leggi_base());//debug //stampo angolo motore con etichetta
  #endif
  while(esci==0){//fintanto che non soddisfo la precisione imposta
    if((leggi_base()+E<ang)||(leggi_base()-E>ang))//se la lettura reale non soddisfa la precisione imposta dall'errore E
      ang_prec=MIN_BASE-10;//faccio in modo che il braccio si sistemi nuovamente per soddisfare la tolleranza imposta (E)
    else
      esci=1;//se la precisione è soddisfatta esco dall loop
    if(ang >= ang_prec+E || ang <= ang_prec-E){//muovo il braccio solamente se è realmente necessario (cioè se la precisione non è soddisfatta)
      #ifdef DEBASE
        Serial.print(leggi_base());Serial.print("  muovo  ");//debug
      #endif
		  muoviBase(ang,VELOCITA_M_BASE);//muovo all'angolo selezionato
		  ang_prec=ang;//aggiorno angolo precedente
    }
	}
}

//funzione che muove il motore di base in funzione di un angolo e di una velocità
void muoviBase(int angolo,int velocita){
  #ifdef MUOVI
    Serial.print("  muoviBase");//debug
    Serial.print(angolo);
  #endif
  if(angolo>=MAX_BASE || angolo<=MIN_BASE){ //se l'angolo immesso non è accettabile stampo errore ed esco
    #ifdef MUOVI
      Serial.println("  Errore: nella funzione muoviBase e' stato immesso un'angolo non accettabile");
    #endif
    return;
  }
  int val=leggi_base();//ottengo posizione attuale
  if(val<angolo){//se la posizione attuale è minore dell'angolo target 
    while(val<angolo){
      if( val<20 || (val>120&&val<170) || (val>205&&val<216) )//intervalli dove è necessaria la velocità 150
        destra(180);
      else if(val>=170 && val<=205)//intervallo dove occorre torsione massima
        destra(255);
      else//altri casi non speciali
        destra(velocita);//velocità a discrezione
      delay(25);
      val=leggi_base();//ottengo la nuova posizione attuale
    }
  }
  else{//se invece la posizione attuale è maggiore dell'angolo target
    while(val>angolo){
      if( val<20 || (val>120&&val<170) || (val>205&&val<=211) )//intervalli dove è necessaria la velocità 150
        sinistra(180);
      else if((val>=170&&val<=205)||(val>211&&val<217))//intervallo dove occorre torsione massima
        sinistra(255);
      else
        sinistra(velocita);//decremento l'angolo attuale
      delay(25);
      val=leggi_base();//ottengo la nuova posizione attuale
    }
  }
  fermo();//fermo i motori
}

//ferma il motore di base
void fermo(){
  //Serial.print("  fermo");//debug
  digitalWrite(GAS,LOW);
  digitalWrite(DIR2,LOW);
  digitalWrite(DIR1,LOW);  
}

//guardando il braccio da dietro si sposterà a sinistra
void sinistra(int velocita){
      //Serial.print("  sinistra");//debug
      analogWrite(GAS,velocita);
      digitalWrite(DIR1,HIGH);
      digitalWrite(DIR2,LOW);
}

//guardando il braccio da dietro si sposterà a destra
void destra(int velocita){
      //Serial.print("  destra");//debug
      analogWrite(GAS,velocita);
      digitalWrite(DIR2,HIGH);
      digitalWrite(DIR1,LOW);
}

//funzione che restituisce la lettura del potenziometro e ne stampa il valore
int leggi_base(){
  int lettura=map(analogRead(POTBASE),0,1023,0,270);//ottengo lettura del potenziometro sotto forma di angolo
  #ifdef LEGGI_BASE
    Serial.print("lettura base");Serial.println(lettura);//debug stampo la lettura per il debug
  #endif
  return lettura;  
}
 
*/
