//Begin page Appunti
/*note eccetera

preferibilmente usare asana o github

*/
//End page Appunti
//Begin page Definitive (Distanza ecc.)
float dist(float* a,float* b){
    return sqrt(mathSquare(a[0]-b[0])+mathSquare(a[1]-b[1])+mathSquare(a[2]-b[2]));
}
//End page Definitive (Distanza ecc.)
//Begin page Muovi
void muovi(){
    api.setPositionTarget(vai);
}
//End page Muovi
//Begin page Ruota
void ruota(){
    api.setAttRateTarget(punta);
}
//End page Ruota
//Begin page fine
void fine(){
    api.setDebug(debug);
	ruota();//ordine di rotazione verso punta[]
	muovi();//ordine di spostamento verso vai[]
}
//End page fine
//Begin page inizio
void inizio(){
    api.getMyZRState(stato);//ottengo dati attuali
	api.getOtherZRState(statoAvv);//ottengo dati avversario
	for(int i=0;i<3;i++){
	    pos[i]=stato[i];
	    facing[i]=stato[i+6];
	    posAvv[i]=statoAvv[i];
	} //ottengo posizione e direzione miei e posizione avversario (magari poi anche direzione avversario)
}
//End page inizio
//Begin page main
struct punto{float x;float y;float z;};//mi piacerebbe usarla
int BoR;// 0 se sfera rossa, 1 se blu
int fase,sottofase;
float stato[12];
float statoAvv[12];
float vai[3];
float punta[3];
float posAvv[3];
float pos[3];
float facing[3];
float debug[7];//7 variabili di debug, da usare
float zona[4];//posizione e dimensione drop zone

void init(){
	for(int i=0;i<7;i++)debug[i]=0;
	for(int i=0;i<3;i++)vai[i]=punta[i]=0;
	api.getMyZRState(stato);
	BoR=stato[1]>0?0:1;//capisco quale sfera siamo
	fase=sottofase=0;
	game.getItemLoc(vai,0);//temporaneo
}

void loop(){
    inizio();
	switch(fase){
	    case 0://drop SPSs, i cosi per sapere dove si trova la zhohona
	        //game.getItemLoc(vai,0);
	        if (sottofase==0 && dist(vai,pos)<0.1){
	            game.dropSPS();
	            sottofase++;
	            game.getItemLoc(vai,1);
	        } else if (sottofase==1 && dist(vai,pos)<0.1){
	            game.dropSPS();
	            sottofase++;
	            game.getItemLoc(vai,2);
	        } else if(sottofase==2 && dist(vai,pos)<0.1){
	            game.dropSPS();
	            sottofase=0;
	            fase++;
	        }
	    break;
	    case 1://getZone
	        if(!game.getZone(zona)){//se true, carica anche la posizione della sfera in zona
	            fase--;
	        }
	    break;
	    default:
	    DEBUG(("ERROR"));
	}
	
	fine();
}

//End page main
