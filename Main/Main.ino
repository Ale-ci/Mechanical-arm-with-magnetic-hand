/*V2.3 derivata da V2.2
 
Obbiettivo:
In questa versione si utilizzano a pieno le 4 configurazioni degli interruttori del braccio

Problemi:
La precisione a volte lascia un po' a desiderare

Modifiche:
Comandi if quà e là nel loop
*/

//definizione costanti letterali di tutto il programma--------------------------------------
//definizioni generali
#define NERO 6  //interruttore 1 (nero)
#define GIALLO 8  //interruttore 2 (giallo)
#define SU 0 //cosa restituiscono gli interruttori se stanno sù
#define GIU 1 //cosa restituiscono gli interruttori se stanno giù
#define POT  A1  //potenziometro

//definizioni CartesianoPolare
#define K 6.5       //distanza tra attaccatura mano (bullone) e punto di aggancio effettivo
#define KH 5.8      //distanza tra carta di sgancio e mano
#define BBASSO 13.8 //parte del braccio inferiore
#define BALTO 14.8 //parte del braccio superiore

//definizioni TorriDiHanoi
#define U -23 //x pilastro 1
#define D -9  //y pilastro 1
#define T -14 //x pilastro 2
#define Q -21  //y pilastro 2
#define C  4  //x pilastro 3
#define S -25 //y pilastro 3
#define HUNO -5 //altezza di un disco
#define HDUE -3.5 //altezza di due dischi
#define HTRE -2 //altezza di tre dischi

//definizioni muoviBase
#define GAS 3        //attivatore ponte H
#define DIR1 4       //ponte H verso 1
#define DIR2 5       //ponte H verso 2
#define POTBASE  A5  //potenziometro che rende servomotore la base
#define MAX_BASE 250     //angolo massimo motore base max 270
#define MIN_BASE 25      //angolo minimo motore base min 0
#define VEL_M_BASE 90 //modulatore della velocità di base (a 100 si inceppa molto facilmente) max 255
#define E 2 //gradi di errore tra l'angolo richiesto e quello reale
#define FRENO 3       //commentare per eliminare resistenza all'inerzia
#define VEL_STOP 100 //velocità con cui oppongo resistenza all'inerzia max 255
#define TEMP_STOP 50 //tempo in cui applico la resistenza all'inerzia min 1

//definizioni rallentatore
#define MAXM_UNO 179 //valore pwm massimo che può essere inviato a M1 (126 gradi sessagesimali)
#define MINM_UNO 90  //valore pwm minimo che può essere inviato a M1 (63 gradi sessagesimali)
#define MAXM_DUE 172 //valore pwm massimo che può essere inviato a M2 (121 gradi sessagesimali)
#define MINM_DUE 97  //valore pwm minimo che può essere inviato a M2 (68 gradi sessagesimali)
#define VEL 45 //attesa tra un cambiamento di angolo e un altro 

//definizioni muoviBraccio
#define CASA1 150  //il pezzo di sopra è adagiato su quello sotto (posizione casa)
#define CASA2 110 //il pezzo di sotto è quasi tutto indietro (posizione base)
#define INTERMEDIO_M1 160 //posizione intermedia negli spostamenti (casa, intermedio, target) 
#define INTERMEDIO_M2 145

//definizioni temporanee di debug--------------------------------------------------------------
#define DEBUG "Debug" //commentare questa riga per eliminare totalmente il debug
#ifdef DEBUG
//globali--------------------------
  //#define FUN 1 //commentare per eliminare stampe dei nomi delle funzioni principali chiamate 
//CartesianoPolare
  #define ANGOLI 5 //commentare per eliminare stampe seriali degli angoli ottenuti da VaiA()
//muoviBase------------------------
  //#define FUN_BASE 1 //commentare per eliminare stampe dei nomi di funzioni secondarie dichiarate in muoviBase
  //#define LEGGI_BASE //commentare per eliminare stampe dell'angolo attuale del motore di base
//rallentatore---------------------
  //#define DERALLE 1 //commentare per eliminare stampe di debug dalle funzioni secondarie dichiarate in rallentatore
#endif//-----------------------------------------------------------------------------------------

//Programma---------------------------------------------------------------------------------
#include <Servo.h>
Servo m1,m2,mano; //dichiaro tre servomotori (perché quello di base lo controllo manualmente)
//variabili di controllo dei Servomotori
int attuale1=CASA1;//inizializzo la posizione attuale (avvio journaling)
int attuale2=CASA2;//inizializzo la posizione attuale (avvio journaling)
//correggo acchiappate
boolean kerrEn=0;//inizializzo la variabile di attivazione errore di acchiappo
int kerr=0;//inizializzo l'errore di acchiappo a 0;

void setup() {
  #ifdef DEBUG
  Serial.begin(9600); 
  #endif  //avvio porta seriale per il debug
//setup muoviBase
  pinMode(DIR1,OUTPUT);
  digitalWrite(DIR1,LOW);
  pinMode(DIR2,OUTPUT);
  digitalWrite(DIR2,LOW);
  pinMode(GAS,OUTPUT);
  digitalWrite(GAS,LOW);//inizio a motore spento
//setup servomotori
  mano.attach(9);
  m1.attach(10);
  m2.attach(11);
//setup generale
  pinMode(NERO,INPUT);
  pinMode(GIALLO,INPUT);
  pinMode(13,OUTPUT);//leddino
  digitalWrite(13,LOW);//spento
//
  m1.write(CASA1);//porto a posizione base i due servomotori di movimento per 
  m2.write(CASA2);//consentire al journaling di funzionare correttamente
}

int attiva=1;
void loop(){
  int lettura=map(analogRead(POT),0,1023,0,270);
  int g=digitalRead(GIALLO);
  int n=digitalRead(NERO);
  if(g==GIU && n==GIU){//stan by
    if(attiva!=1){//vai a posizione di stazionamento e rimani li
      muoviBraccioACasa();
      servoBase(25);
    }
    attiva=1;
  }
  if(g==SU && n==GIU){//hanoi
    if(attiva!=2)
      Hanoi();//esegui un solo gioco
    attiva=2;
  }
  if(g==GIU && n==SU){//profilo M1
    if(attiva!=3){
      muoviBraccioACasa();
      servoBase(225);
      muoviBraccioAIntermedio();
    }
    attiva=3;
  }
  if(g==SU && n==SU){//avanti e indietro Alpha
    if(attiva%2==0)
      M2(155);
    else   //vado avanti quando è pari e indietro quando è dispari
      M2(135);
    attiva++;
  }
  #ifdef DEBUG
    Serial.println(" ");
  #endif
}


