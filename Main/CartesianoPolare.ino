//Programma per commutare le coordinate cartesiane del Target (x,y,z) in coordinate polari (Alpha,Beta,Delta) 


//funzione che trasforma coordinate spaziali in coordinate polari
void VaiA(const float x,const float y,float z){
  z+=KH;
  int Delta;
  double m,DeltaG;
  if(x==0){//se sto a PigrecoMezzi+k Pigreco 
    Delta=90;
  }
  else if(y==0){//se sto a k Pigreco
    if(x>0)//se sono a zero
      Delta=0;
    else//se sono a 180
      Delta=180;
  }
  else{
    m=y/x;//ottengo coefficiente angolare
    DeltaG=atan(m)*180/3.14;//ottengo e converto da radianti a gradi DeltaG grezzo 
    DeltaG=floor(DeltaG+0.5);//approssimo il dato grezzo
    if(x>0 && y<0)//se è nel quarto quadrante
      Delta=-DeltaG;
    if(x<0 && y<0)//se è nel terzo quadrante
      Delta=180-DeltaG;
    if(x<0 && y>0)//se è nel secondo quadrante
      Delta=180-DeltaG;
  }
//Qui ho ottenuto Delta
  float d=sqrt(pow(x,2)+pow(y,2))-K;//eseguo pitagora per sapere di quanto mi devo spostare r=d+k 
  double Alpha_primo,Alpha_secondo,ipotenusa;
  int Alpha;
  Alpha_secondo=atan(z/d)*180/3.14;//ottengo e converto da radianti a gradi Alpha secondo con il secondo teorema dei triangoli rettangoli 
  ipotenusa=sqrt(pow(z,2)+pow(d,2));//ottengo l'ipotenusa del triangolo 1 con il teorema di pitagora
  Alpha_primo=acos((pow(ipotenusa,2)+pow(BBASSO,2)-pow(BALTO,2))/(2*ipotenusa*BBASSO));//ottengo alpha primo con il teorema del coseno
  Alpha_primo=Alpha_primo*180/3.14; //converto da radianti a gradi
  Alpha=floor((Alpha_primo+Alpha_secondo)+0.5);
//Qui ho ottenuto Alpha
  double Beta;
  Beta=asin(sin(Alpha_primo*3.14/180)*ipotenusa/BALTO)*180/3.14;//ottengo Beta con il teorema dei seni e lo converto in gradi da radianti (Alpha lo converto in radianti)
  Beta=floor(Beta+0.5);//approssimo Beta
//Qui ho ottenuto Beta
  #ifdef ANGOLI
  Serial.print(Alpha);
  Serial.print(" <Alpha Beta> ");
  Serial.print(Beta);
  Serial.print(" ");
  Serial.print(sqrt(pow(x,2)+pow(y,2)));
  Serial.print(" <Raggio Delta> ");
  Serial.println(Delta);
  #endif
  //inizio conversione per Beta
  Beta=Alpha+Beta;
//controllo se non rompo nulla
  if(Beta-Alpha<40||Beta-Alpha>125){//se l'angolo beta reale è minore o maggiore di quello che meccanicamente può essere
    Serial.print("Errore, Beta reale troppo piccolo/grande");
    return;
  }
//converto angoli teorici in angoli pwm
   Alpha=map(Alpha,45,120,172,97);//Tolleranza molto lasca (dieci gradi di errore per raggiungere 100 gradi e cinque per raggiungere 50)
   Beta=map(Beta,115,190,100,175);
   //Serial.println(Alpha);Serial.print(Beta);
//muovo i motori
  if(!(attuale1==CASA1 && attuale2==CASA2)){//se il braccio non è a casa ce lo porto
    muoviBraccioACasa();
  }
  if(kerrEn==1)//se ho attivato l'errore k con acchiappa();
  servoBase(Delta+kerr);//muovo prima la base tenendo conto del fatto che se ho acchiappato co un errore mi devo spostare di un angolo diverso
  else
  servoBase(Delta);//muovo normalmente se non ho acchiappato nulla
  muoviBraccioA(Beta,Alpha);//poi allungo il braccio
}

