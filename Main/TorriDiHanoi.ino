/*
void s(int partenza,int destinazione,int appoggio,int dischi);
int main(){
  int n;
  printf("immetti numero\n");
  scanf("%d",&n);
  s(1,3,2,n);
  system("PAUSE");
  return 0;
}
void s(int partenza,int destinazione,int appoggio,int dischi){
  //caso base
  if(dischi == 1){
    printf("%d --> %d\n",partenza,destinazione);
  }
  //passo ricorsivo
  else{
    s(partenza,appoggio,destinazione,dischi - 1);
    printf("%d --> %d\n",partenza,destinazione);
    s(appoggio,destinazione,partenza,dischi - 1);
  }
}*/ 

void Hanoi(){
  //sposto il disco piccolo da 1 a 3
  VaiA(U,D,HTRE);
  acchiappa();
  VaiA(C,S,HUNO);
  molla();
  //sposto il disco medio da 1 a 2
  VaiA(U,D,HDUE);
  acchiappa();
  VaiA(T,Q,HUNO);
  molla();
  //sposto il disco piccolo da 3 a 2
  VaiA(C,S,HUNO);
  acchiappa();
  VaiA(T,Q,HDUE);
  molla();
  //sposto il disco grande da 1 a 3
  VaiA(U,D,HUNO);
  acchiappa();
  VaiA(C,S,HUNO);
  molla();
  //sposto il disco piccolo da 2 a 1
  VaiA(T,Q,HDUE);
  acchiappa();
  VaiA(U,D,HUNO);
  molla();
  //sposto il disco medio da 2 a 3
  VaiA(T,Q,HUNO);
  acchiappa();
  VaiA(C,S,HDUE);
  molla();
  //sposto il disco piccolo da 1 a 3
  VaiA(U,D,HUNO);
  acchiappa();
  VaiA(C,S,HTRE);
  molla();
  muoviBraccioACasa();
}


