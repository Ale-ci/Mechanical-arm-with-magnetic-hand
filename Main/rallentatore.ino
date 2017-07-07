//set di funzioni create per rallentare la velocità di spostamento dei servomotori controllati dalla libreria Servo.h
//le funzioni MX hanno l'inizializzazione a CASAX che poi cambia come se fosse un journaling
//immettere in queste funzioni solo i valori di angoli pwm che variano da 0 255!!!
 
//funzione che permette ai servomotori di muoversi più lentamente
void M1(const int ang_pwm){
  #ifdef FUN 
  Serial.print(" M1 "); 
  #endif //comando di debug selettivo
  if(ang_pwm>MAXM_UNO || ang_pwm<MINM_UNO){//se l'angolo che invio a M1 non può essere raggiunto meccanicamente
    Serial.print("Errore, inviato angolo fuori dominio a M1");//stampo errore
    return;//esco
  }
  if(attuale1>ang_pwm){//se l'angolo attuale è maggiore di quello target
    #ifdef DERALLE 
    Serial.print(" m1_attualeMaggiore_decremento ");
    #endif //comando di debug selettivo
    for(int x=attuale1;x>ang_pwm;x--){//diminuisco gradualmente
      m1.write(x);
      delay(VEL);//aspetto tot prima di diminuire ancora
    }
  }
  if(attuale1<ang_pwm){//se invece è minore
    #ifdef DERALLE 
    Serial.print(" m1_attualeMinore_incremento "); 
    #endif //comando di debug selettivo
    for(int x=attuale1;x<ang_pwm;x++){//incremento gradualmente
      m1.write(x);
      delay(VEL);//aspetto tot prima di incrementare ancora
    }
  }
    attuale1=ang_pwm;//aggiorno la posizione attuale (journaling)
}

//funzione che permette ai servomotori di muoversi più lentamente
void M2(const int ang_pwm){ 
  #ifdef FUN 
  Serial.print(" M2 "); 
  #endif //comando di debug selettivo
  if(ang_pwm>MAXM_DUE || ang_pwm<MINM_DUE){//se l'angolo che invio a M2 non può essere raggiunto meccanicamente
    Serial.print("Errore, inviato angolo fuori dominio a M2");//stampo errore
    return;//esco
  }
  if(attuale2>ang_pwm){//se l'angolo attuale è maggiore di quello target 
    #ifdef DERALLE 
    Serial.print("m2_attualeMaggiore_decremento"); 
    #endif //comando di debug selettivo
    for(int x=attuale2;x>ang_pwm;x--){//diminuisco gradualmente
      m2.write(x);
      delay(VEL);//aspetto tot prima di decrementare nuovamente
    }
  }
  if(attuale2<ang_pwm){//se invece è minore
    #ifdef DERALLE 
    Serial.print(" m2_attualeMinore_incremento "); 
    #endif //comando di debug selettivo
    for(int x=attuale2;x<ang_pwm;x++){//incremento gradualmente
      m2.write(x);
      delay(VEL);//aspetto tot prima di incrementare nuovamente
    }
  }
    attuale2=ang_pwm;//aggiorno posizione attuale (journaling)
}

