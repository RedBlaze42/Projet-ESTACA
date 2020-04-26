#include<stdlib.h>
#include<stdio.h>

#define max_pions 20

//Types de pions
#define CTORP 1
#define CUIRASSE 2
#define PIEGE 3


typedef struct pion{
	int coord_x;
	int coord_y;
	int type; //CTORP CUIRASSE ou PIEGE
	int pv=1;//2=blindé 1=vivant 0=mort
	int invisible; //numéro de tour
	int anti_blindage;
	int player;
} pion;

typedef struct joueur{
	pion pions[20];
	int cartes[2];
} joueur;


int is_pionSurCase(joueur attaquant, joueur defenseur, int x, int y);

int main(){
    
}

int is_pionSurCase(joueur attaquant, joueur defenseur, int x, int y){
    for(int i=0;i<20;i++){
        if(attaquant.pions[i].coord_x==x && attaquant.pions[i].coord_y==y){
            return 1;
        }
    }
    for(int i=0;i<15;i++){
        if(defenseur.pions[i].coord_x==x && defenseur.pions[i].coord_y==y){
            return 1;
        }
    }
    return 0;
}