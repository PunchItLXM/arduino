// reflexion : au lieu de choisir le niveau via blth appui sur un bouton
// code pour la fonction aléatoire nécessaire plus loin
long math_random_int(int a,int b) {
  if (a > b) {
    int c = a;
    a = b;
    b = c;
  }
  return random(a,b);
}

// bibliothèque pour communication blth
#include <SoftwareSerial.h>
SoftwareSerial blth(10, 11); // (RX, TX) (pin Rx BT, pin Tx BT)

int M1dirpin = 7;  //Moteur 1 sens de rotation
int M1steppin = 6; //Moteur 1 pas pin
int M1en=8;  //Moteur 1 allumage pin

int pinPot=0; //variable pour définir le CAN où est connecté le potentiomètre
int valPot=0; //variable pour récupérer la tension aux bornes du potentiomètre traduite par le CAN . On l’initialise à 0.
int maxPot=0;
int tempoActivation=0; 
long int tempodepart=0;
int tempsdereaction=0;
int coupreussi=0;                                     
int moytpsreac=0;
int meilleurtps=2000;
int piretps=0;
int forcemoy=0;
int plusfort=0;
int nbrdecouptotal=0;
int mposition=1;//position initiale
int MPosition=1;
int res=0;
long int tpsround=30000;
const int buzzer = 9; //buzzer to arduino pin 9

void setup(){
  Serial.begin(9600); // ouvetrure de la communication
   blth.begin(9600);
  randomSeed(analogRead(0));

  pinMode(M1dirpin,OUTPUT);
  pinMode(M1steppin,OUTPUT);
  pinMode(M1en,OUTPUT);
  digitalWrite(M1en,LOW);
  
  pinMode(buzzer, OUTPUT); // bip sur la broche 9
 
 // bip départ
  tone(buzzer, 8000); // envoie un signal sonore de 8KHz
  delay(800);        // pendant 0.8s
  noTone(buzzer);     // arrete le son
  delay(800);       // pendant 0.8s
  tone(buzzer, 8000); 
  delay(800);        
  noTone(buzzer);     
  delay(800);        
  tone(buzzer, 8000); 
  delay(800);        
  noTone(buzzer);     
 
}
void loop(){
  
  while(millis()<=tpsround){ // temps de la scéance
  
  int tourne;
  int j;  
  int  tpsattente=2000; //temps pdt le quel le boxeur doit frapper ( à modifier suivant le niveau)
  valPot=analogRead(A0); //lit la tension, la convertit en valeur numérique et la stocke dans valPot

//bip avant coup
  tone(buzzer, 8000); 
  delay(300);        
  noTone(buzzer);     

   tempoActivation=1;
   tempodepart=millis();

  //mesure de la force
  if(tempoActivation==1){
    while(millis()-tempodepart<=tpsattente){
    valPot=analogRead(A0);
    if(valPot>maxPot){
      maxPot=valPot;
    tempsdereaction=millis()-tempodepart;
    }
   }
  }
// incrémentation des différentes variables pour les résultats
Serial.println(maxPot);
 nbrdecouptotal=nbrdecouptotal+1;

 if (maxPot>50){
  coupreussi=coupreussi+1;
  moytpsreac=moytpsreac+tempsdereaction;
 }
 if (maxPot>plusfort){
    plusfort=maxPot;
 }
 if (tempsdereaction<meilleurtps){
  meilleurtps=tempsdereaction;
 }
   
    forcemoy=forcemoy+maxPot;
    tempoActivation=0;
    maxPot=0;

 //choix de la position, nbr de pas     
  if (mposition==0){
      long A=random(0,2);
        if (A==0){
          tourne=50;
        MPosition=1; //centre
        }
        if (A==1){
           tourne=100;
        MPosition=2;  //droite
        }
  }
   else if (mposition==1){
      long A=random(0,2);
        if (A==0){
          tourne=-50;
        MPosition=0; //gauche
        }
        if (A==1){
           tourne=50;
        MPosition=2;  //droite
        }
  }

     else if (mposition==2){
      long A=random(0,2);
        if (A==0){
          tourne=-50;
        MPosition=1; //centre
        }
        if (A==1){
           tourne=-100;
        MPosition=0;  //gauche
        }
     }
mposition=MPosition; // pour ne pas que le if se fasse deux fois 

//sens de rotation du moteur
   if (tourne<0){
        digitalWrite(M1dirpin,LOW);
  }
   if (tourne>0){
        digitalWrite(M1dirpin,HIGH);
  }
  // pilotage moteur
  for(j=0;j<=abs(tourne);j++){
    digitalWrite(M1steppin,HIGH);
    delayMicroseconds(10);    // à modifier suivant le niveau 
    digitalWrite(M1steppin,LOW); //Rising step
    delay(10);                  // à modifier suivant le niveau
    }
  }
//fin de la scéance

//mise en position initiale
int tourne;
int j;

  if (mposition==0){
        tourne=150;
        MPosition=1; //centre
        }

  if (mposition==2){
        tourne=-150;
        MPosition=1; //centre
        }


   digitalWrite(M1en,HIGH);
   if (tourne<0){
        digitalWrite(M1dirpin,LOW);
  }
   if (tourne>0){
        digitalWrite(M1dirpin,HIGH);
  }
 if (mposition!=1){ //si position différente du centre
  for(j=0;j<=abs(tourne);j++){
    digitalWrite(M1steppin,HIGH);
    delayMicroseconds(10);
    digitalWrite(M1steppin,LOW); //Rising step
    delay(10);
    }
  } 
mposition=MPosition;
// conversion resistance en N

 forcemoy=forcemoy*0.06-3;
 plusfort=plusfort*0.06-3;

  moytpsreac=moytpsreac/coupreussi;
  forcemoy=forcemoy/coupreussi;
 
  if (res!=1){
//bip de fin
  tone(buzzer, 8000); // envoie un signal sonore de 8KHz
  delay(800);        // pendant 0.8s
  noTone(buzzer);     // arrete le son
  delay(800);       // pendant 0.8s
  tone(buzzer, 8000); 
  delay(800);        
  noTone(buzzer);     
  delay(800);        
  tone(buzzer, 8000); 
  delay(800);        
  noTone(buzzer);     

//envoie des résultats
  blth.write("resultats:");
  blth.write("\n"); // saut de ligne

// conversion des variables en caractère

 blth.write("temps de reaction moyen:");
    char moytps[12]="";
    itoa ( moytpsreac, moytps, 10);
    blth.write(moytps );
    blth.write("ms");
  blth.write("\n ");
   
  blth.write("nombre de coups total: ");
    char nbrcouptt[12]="";
    itoa (nbrdecouptotal, nbrcouptt, 10);
   blth.write(nbrcouptt);
  blth.write("\n");
  
  blth.write("nombres de coups reussi:");
    char nbrcoupr[12]="";
    itoa (coupreussi, nbrcoupr, 10);
    blth.write(nbrcoupr);
  blth.write("\n");
  
  blth.write("moyenne de force par coups:");
    char moyf[12]="";
    itoa (forcemoy, moyf, 10);
    blth.write(moyf);
    blth.write("N");
  blth.write("\n");
  
  blth.write("meilleurs temps:");
    char mtps[12]="";
    itoa (meilleurtps, mtps, 10);
    blth.write(mtps);
    blth.write("ms");
  blth.write("\n");

  blth.write("coup le plus fort:");
    char coupfort[12]="";
   itoa (plusfort, coupfort, 10);
    blth.write(coupfort);
    blth.write("N");
    blth.write("\n");
    blth.write("\n");
    res=1;
  }
  while(res=1){
  digitalWrite(M1en,LOW);}
  }
 
 //fin du programme
