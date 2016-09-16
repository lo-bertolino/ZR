//Begin page Distanza
float distanza(float A[], float B[]){
    mathVecSubtract(vectorBetween, A, B, 3);
    return mathVecMagnitude(vectorBetween, 3);
}
//End page Distanza
//Begin page Foto
void Foto(){
    if(game.getMemoryFilled()<2)
        if(!game.posInDark(otherPos))
            if(game.isFacingOther())
                if(game.isCameraOn())
                    if(distanza(myPos, otherPos)>PHOTO_MIN_DISTANCE)
                        if(game.getEnergy()>=PICDEN)
                            if(game.getMirrorTimeRemaining()==0)
                                if(game.getPicPoints()>0.001){  //-0.1 E
                                    game.takePic(); //-1E
                                    DEBUG(("PUNTIIII :*"));
                                }
}
//End page Foto
//Begin page Muovi
void spostamento(){
    if(step==0){
       mathVecSubtract(vectorBetween,targetPoint,myPos,3);
       if (mathVecMagnitude(vectorBetween,3)>0.75)
           api.setVelocityTarget(vectorBetween);
       else api.setPositionTarget(targetPoint);
    }
    else api.setPositionTarget(targetPoint);
}
//End page Muovi
//Begin page PuntaAvversario
void puntaAvversario(){
    if(!upLoad){
        mathVecSubtract(vectorBetween,otherPos,myPos,3);
        mathVecNormalize(vectorBetween,3);
        api.setAttitudeTarget(vectorBetween);
    }
}
//End page PuntaAvversario
//Begin page Upload
void upload() {//Quasi OK
    if(game.getEnergy()>PICUP_end&&(game.getMemoryFilled()==2||(game.getCurrentTime()>170 &&game.getMemoryFilled()==1))){
        upLoad=true;
        //mi oriento verso terra (z direction>0, 0,25rad)
        api.setAttitudeTarget(ground);
        
        if(myAtt[2]>0.85)
            if(mathVecMagnitude(myAttSpeed,3)<0.1)
            {
                if(game.getEnergy()>=PICUP){
                    game.uploadPics();
                }
                else if(game.getEnergy()>=PICUP_end && game.getCurrentTime()>170)
                    game.uploadPics();
            }
    }
    if(game.getMemoryFilled()==0)
        upLoad=false;
}
//End page Upload
//Begin page checkOutOfBounds
bool outOfBounds (float posToCheck[]){//OK
    if (fabsf(posToCheck[0])>7.5)
        return true;
    if (fabsf(posToCheck[1])>0.6)
        return true;
    if (fabsf(posToCheck[2])>0.6)
        return true;
    return false;
}
//End page checkOutOfBounds
//Begin page main
float PICDEN,PICUP,PICUP_end;//Energia minima x fare foto in zona (L) luce e (D) buio
float ground[3];
float myAtt[3];
float myAttSpeed[3];	     //usate nella funzione carica foto
float myPos[3];
float myForces[3];
float myState[12];	         //stato del satellite
float otherPos[3];          
float otherState[12];        //stato del satellite avversario
float vectorBetween[3];      //vettore utilizzato per spostarsi
float distance;              //distanza tra il satellite e l'obbiettivo
float targetPoint[3];
float oggetto[3];
float oggetto1[3];
int step;      				 //variabile che scandisce la strategia
int tempo;                   //variabile per contare il tempo
int cont;
bool upLoad;
float vector[3];
int strategia[2];
int Time_of_Mirror;

void init(){
for(int i=0;i<3;i++)
ground[i]=EARTH[i];
step=0;
tempo=0;
cont=0;
PICDEN=2.18f;
PICUP=1.6f;
Time_of_Mirror=0;
PICUP_end=1.0f;
upLoad=false;
    strategia[0]=6;
    strategia[1]=7;
}

void loop(){
    ogniSecondo();
    if(game.getCurrentTime()==0){
        game.getItemLoc(oggetto,7);
        game.getItemLoc(oggetto1,8);
        if(distanza(myPos,oggetto)>distanza(myPos,oggetto1))
            strategia[1]=8;  
    }
    if (step<2) {
        game.getItemLoc(oggetto,strategia[step]);
        if (game.hasItem(strategia[step])==-1) 
            game.getItemLoc(targetPoint,strategia[step]);
        else      
            step++;
    } 
    else {
        if(step==2){
             if(game.getLightSwitchTime()==0)
                  Time_of_Mirror=Time_of_Mirror+1;//Quando siamo verso la fine del gioco lo usiamo non prima
             if(game.posInDark(otherPos) && Time_of_Mirror==2){
                 targetPoint[0]=0;
                 targetPoint[1]=-0.2f;     
                 targetPoint[2]=-0.5f;
             }
             else{
                   if(strategia[0]==8){
                       targetPoint[0]=myPos[0];
                       targetPoint[1]=myPos[1];
                       targetPoint[2]=targetPoint[2]+0.1;
                   }
                    
 //__________________________Strategie diverse____________________________________________
                   if(strategia[0]==7){
                       if(myPos[0]<-0.45 || myPos[1]<0.55){
                           targetPoint[0]=-0.5f;
                           targetPoint[1]=0.6f;
                           targetPoint[2]=-0.58f;
                       }
                       else{
                           if(otherPos[1]>0.5 && otherPos[2]>0)
                               myPos[1]=myPos[1]-0.5f;
                           if(otherPos[0]>-0.4 && otherPos[2]>0)
                               myPos[0]=myPos[0]-0.4f;
                       }
                   }
             }
        }
    }
    if(game.getScore()>(game.getOtherScore()+4.5) && Time_of_Mirror==2)//punteggio nostro maggiore di almeno 4.5 rispetto all'avversario
        game.useMirror();

    if (outOfBounds(myPos)){
        targetPoint[0]=0;
        targetPoint[1]=0.2f;
        targetPoint[2]=-0.5f;
    }
    
    if(game.posInDark(myPos) && game.getEnergy()<1.1 && game.getCurrentTime()<150)//con >30 va outofbounds se luce e buio cambiano troppo presto
      api.setPositionTarget(myPos);
    else
      spostamento();
    cont++;
}

//End page main
//Begin page ogniSecondo
void ogniSecondo(){
    api.getMyZRState(myState);
    api.getOtherZRState(otherState);
    for(int i=0;i<3;i++)//posizione satellite 
	    myPos[i]=myState[i];
    for(int i=0;i<3;i++)//posizione avversario
	    otherPos[i]=otherState[i];
    for(int i=0;i<3;i++)//rotazione satellite 
        myAtt[i]=myState[i+6];
    for(int i=0;i<3;i++)//velocita' rotazione
        myAttSpeed[i]=myState[i+9]-0.035;
    puntaAvversario();
    Foto();
    upload();
}
//End page ogniSecondo
