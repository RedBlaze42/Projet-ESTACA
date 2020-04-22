#include<stdlib.h>
#include<stdio.h>

//Types de pions
#define CTORP 1
#define CUIRASSE 2
#define PIEGE 3


typedef struct pion{
	int coord_x;
	int coord_y;
	int type; //CTORP CUIRASSE ou PIEGE
	int vivant;//true=vivant false=mort
	int invisible; //numéro de tour
	int anti_blindage;
	int player;
    int blindage;//1 si blindé
}pion;

int is_pionSurCase(pion *pions_attaquant, pion *pions_défenseur, int x, int y);

int main(){
    pion pions_attaquant[20]; //2CTORPB 18CTORP + 2CTORP OU 1TORP
    pion pions_défenseur[15]; //1CUIRASSE 2CTORPB 10CTORP + 2 PIEGES

    int cartes_attaquant[2]; //numéro de la carte ou 0 si il l’as déjà utilisé
    int cartes_défenseur[2]; //numéro de la carte ou 0 si il l’as déjà utilisé

    
}

int is_pionSurCase(pion *pions_attaquant, pion *pions_défenseur,int x, int y){
    for(int i=0;i<20;i++){
        if(pions_attaquant[i].coord_x==x && pions_attaquant[i].coord_y==y){
            return 1;
        }
    }
    for(int i=0;i<15;i++){
        if(pions_défenseur[i].coord_x==x && pions_défenseur[i].coord_y==y){
            return 1;
        }
    }
    return 0;
}