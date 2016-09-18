int BoR;// 0 se sfera rossa, 1 se blu
int fase,sottofase;
float stato[12];
float statoAvv[12];
float vai[3];
float punta[3];
float posAvv[3];
float pos[3];
float facing[3];
float vel[3];//velocità della sfera
float debug[7];//7 variabili di debug, da usare
float zona[4];//posizione e dimensione drop zone

float dist(float* a,float* b){
    //return sqrt(mathSquare(a[0]-b[0])+mathSquare(a[1]-b[1])+mathSquare(a[2]-b[2]));
    float* v;
    mathVecSubtract(v, a, b, 3);
    return mathVecMagnitude(v, 3);
}

//imposta più facilmente le direzioni (meno errori)
void setVai(float x,float y,float z){
    vai[0]=x;
    vai[1]=y;
    vai[2]=z;
}
void setVai(float* v){
    vai[0]=v[0];
    vai[1]=v[1];
    vai[2]=v[2];
}
void setPunta(float x,float y,float z){
    punta[0]=x;
    punta[1]=y;
    punta[2]=z;
}
void setPunta(float* v){
    punta[0]=v[0];
    punta[1]=v[1];
    punta[2]=v[2];
}
void setVec(float *v,float x,float y,float z){
    v[0]=x;
    v[1]=y;
    v[2]=z;
}

void muovi(){
    if (oOB(pos))setVai(0,0,0);
    float d=dist(vai,pos);
    if (d<0.1)
        frena(d);
    else{
        float vec; mathVecSubtract(vec,vai,pos,3);
        if (d>0.2) api.setVelocityTarget(vec);
        else api.setPositionTarget(vai);
    }
}

void frena(float p){
    float f[3];
    p*=10;
    setVec(f,0-vel[0]*p,0-vel[1]*p,0-vel[2]*p);
    api.setAttitudeTarget(f);
}

void ruota(){
    api.setAttRateTarget(punta);
}

bool oOB (float* ptc){//outOfBounds
    //fabsf(ptc[0])>0.75||fabsf(ptc[1])>0.75||fabsf(ptc[2])>0.75
    if (!(fabsf(ptc[0])<0.75&&fabsf(ptc[1])<0.75&&fabsf(ptc[2])<0.75))
        return true;
    return false;
}

void init(){
	for(int i=0;i<7;i++)debug[i]=0;
	for(int i=0;i<3;i++)vai[i]=punta[i]=0;
	api.getMyZRState(stato);
	BoR=stato[1]>0?0:1;//capisco quale sfera siamo
	fase=sottofase=0;
	game.getItemLoc(vai,0);//temporaneo
}

void inizio(){
    api.getMyZRState(stato);//ottengo dati attuali
    api.getOtherZRState(statoAvv);//ottengo dati avversario
    for(int i=0;i<3;i++){
        pos[i]=stato[i];
        vel[i]=stato[i+3];
        facing[i]=stato[i+6];
        posAvv[i]=statoAvv[i];
    }//ottengo posizione e direzione miei e posizione avversario (magari poi anche direzione avversario)
}
void fine(){
    api.setDebug(debug);
    ruota();//ordine di rotazione verso punta[]
    muovi();//ordine di spostamento verso vai[]
}
void loop(){
    inizio();
    switch(fase){
        case 0://drop SPSs, i cosi per sapere dove si trova la zhohona
        if (sottofase==0 && dist(vai,pos)<0.1){
            game.dropSPS();
            sottofase++;
            game.getItemLoc(vai,1);
        }
        else if (sottofase==1 && dist(vai,pos)<0.1){
            game.dropSPS();
            sottofase++;
            game.getItemLoc(vai,2);
        }
        else if(sottofase==2 && dist(vai,pos)<0.1){
            game.dropSPS();
            sottofase=0;
            fase++;
        }
        break;
        case 1://getZone
        if(!game.getZone(zona)){//se true, carica anche la posizione della sfera in zona
            fase--;
        }else {
            setVai(zona);
        }
        break;
        default:
        DEBUG(("ERROR"));
    }
    fine();
}