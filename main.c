#include<stdlib.h>
#include<stdio.h>
#include<windows.h>

#define DIM_PLATEAU 25
#define max_pions 20

//Couleurs
#define JOUEUR1 1
#define JOUEUR2 4

//Types de pions
#define CTORP 1
#define CUIRASSE 2
#define PIEGE 3

//Touches
#define haut 65
#define bas 66
#define droite 67
#define gauche 68
#define enter 13

//Zones
#define PLATEAU 0
#define DEFENSE 1
#define ATTAQUE 2

typedef struct pion{
	int coord_x;
	int coord_y;
	int type=0; //CTORP CUIRASSE ou PIEGE
	int pv=1;//2=blindé 1=vivant 0=mort
	int invisible; //0=visible pour tout le monde >0=nombre de tours invisibles -1=tout le temps invisible (piège)
	int anti_blindage;
	int player;
} pion;

typedef struct joueur{
    int num;
	pion pions[20];
	int cartes[2];
} joueur;


void color (int couleurDuTexte, int couleurDuFond);

int main(){
    
}

void deplacer_pions(joueur joueurSel, joueur joueur2, int pion_sel){
    int en_mouvement=1;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entrée
        pion plateau[DIM_PLATEAU][DIM_PLATEAU];
        int x,y;
        //Afficher plateau surbrillance
        remplirTab(plateau, joueurSel, joueur2);
        int direcion_prise;
        switch(getch()) {
            case enter:
                joueurSel.pions[pion_sel].coord_x=x;//Mauvaise synthaxe
                joueurSel.pions[pion_sel].coord_y=y;
                en_mouvement=0;
                break;
            case bas:
                if(y+1<DIM_PLATEAU && is_pionSurCase(joueurSel,joueur2,x,y-1)==0 && (direction_prise==0 || direction_prise==bas)){
                    y+=1;
                    if(direction_prise==0) direction_prise=bas;
                }
                break;
            case haut:
                if(y-1>0 && is_pionSurCase(joueurSel,joueur2,x,y-1)==0 && (direction_prise==0 || direction_prise==haut)){
                    y-=1;
                    if(direction_prise==0) direction_prise=haut;
                }
                break;
            case gauche:
                if(x-1>0 && is_pionSurCase(joueurSel,joueur2,x-1,y)==0 && (direction_prise==0 || direction_prise==gauche)){
                    x-=1;
                    if(direction_prise==0) direction_prise=gauche;
                }
                break;
            case droite:
                if(x+1<DIM_PLATEAU && is_pionSurCase(joueurSel,joueur2,x+1,y)==0 && (direction_prise==0 || direction_prise==droite)){
                    x+=1;
                    if(direction_prise==0) direction_prise=droite;
                }
                break;
        }
    }
}

int pion_peut_attaquer(joueur joueurSel, joueur joueur2, pion pionSel){
    pion plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);
    if(y-1>=0 && x-1>=0 && (*plateau[y-1][x-1]).player!=pionSel.player && plateau[y-1][x-1].player!=0){//Haut Gauche
        return 1;
    }else if(y+1<DIM_PLATEAU && x-1>=0 && plateau[y+1][x-1].player!=pionSel.player && platesau[y+1][x-1].player!=0){//Bas Gauche
        return 1;
    }else if(y-1>=0 && x+1<DIM_PLATEAU && plateau[y-1][x+1].player!=pionSel.player && plateau[y-1][x+1].player!=0){//Haut Droite
        return 1;
    }else if(y+1<DIM_PLATEAU && x+1<DIM_PLATEAU && plateau[y+1][x+1].player!=pionSel.player && plateau[y+1][x+1].player!=0){//Bas Droite
        return 1;
    }else{
        return 0;
    }
}
    int en_mouvement=1;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entrée
        pion plateau[DIM_PLATEAU][DIM_PLATEAU];
        int x,y;
        //Afficher plateau surbrillance
        remplirTab(plateau, joueurSel, joueur2);
        int direcion_prise;
        switch(getch()) {
            case enter:
                if(libre!=1 || is_pionSurCase(joueurSel,joueur2,x,y)!=1){
                    *sel_x=x;
                    *sel_y=y;
                    en_mouvement=0;
                }
                break;
            case bas:
                if(is_in_zone(zone,x,y+1)==1) y+=1;
                break;
            case haut:
                if(is_in_zone(zone,x,y-1)==1) y-=1;
                break;
            case gauche:
                if(is_in_zone(zone,x-1,y)==1) x-=1;
                break;
            case droite:
                if(is_in_zone(zone,x+1,y)==1) x+=1;
                break;
        }
    }
}

int is_pionSurCase(joueur joueurAff, joueur joueur2, int x, int y){
    for(int i=0;i<20;i++){
        if(joueurAff.pions[i].coord_x==x && joueurAff.pions[i].coord_y==y && joueurAff.pions[i].vie>0){
            return 1;
        }
    }
    for(int i=0;i<15;i++){
        if(joueur2.pions[i].coord_x==x && joueur2.pions[i].coord_y==y && joueur2.pions[i].vie>0 && joueurAff.pions[i].invisible==0){
            return 1;
        }
    }
    return 0;
}
void remplirTab(pion tab[][], joueur *joueur1, joueur *joueur2){
    int i;
    for(i=0,i<nb,i++){
        if(joueur1.pion[i].pv>0){
            tab[(*joueur1.pions[i]).coord_x][(joueur1.pions[i]).coord_y]=&(joueur1.pion[i]);
        }
    }
     for(i=0,i<nb,i++){
        if(joueur2.pion[i].pv>0){
            tab[joueur2.pions[i].coord_x][joueur2.pions[i].coord_y]=&(joueur2.pion[i]);
        }
    }
}
void afficher_pion(pion pionAff){//pour afficher une case
    switch(pionAff.type){
        case 1 : 
            printf("1");
            if(pionAff.player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            break;
        case 2 :
            printf("2");
            if(pionAff.player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            break;
        case 3 :
            printf("3");
            if(pionAff.player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            break;
        default:
            color(0,15);
            print(" ");
    }
}

int is_in_zone(int zone, int x, int y){
    if(zone==PLATEAU){
        return 1;
    }else if(zone==DEFENSE){
        if(x>2 && x<8 && y>2 && y<8){
            return 1;
        }else{
            return 0;
        }
    }else if(zone==ATTAQUE){
        if( (x>2 && x<8 && y>2 && y<8)==0 ){//vérifier
            return 1;
        }else{
            return 0;
        }
    }
}

void afficherplateau(pion tab[][], joueur joueurAff,joueur joueur2){//on affiche le plateau
     remplirTab(pion tab[][], joueuAff, joueur2);
    int i, j;
    for(j=0,j<25,j++){
        for(i=0,i<25,i++){
            if(*joueurAFF.pion[i].invisible=0){
                afficher_pion(*joueurAff.pion[i]);
            }
        }
        printf("\n");
    }
    printf("\n");
}


void color (int couleurDuTexte, int couleurDuFond){// permet de gérer les couleurs
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDuFond*16+couleurDuTexte);
}