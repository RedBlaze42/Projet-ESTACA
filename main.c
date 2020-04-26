#include<stdlib.h>
#include<stdio.h>

#define max_pions 20

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

void deplacer_pions(joueur joueurSel, joueur joueur2, int pion_sel){
    int fin_mouvement=1;
    while(fin_mouvement){//Attendre que le joueur appuie sur la touche entrée
        pion plateau[25][25];
        int x,y;
        remplirTab(plateau, joueurSel, joueur2);
        switch(getch()) {
            case enter:
                joueurSel.pions[pion_sel].coord_x=x;//Mauvaise synthaxe
                joueurSel.pions[pion_sel].coord_y=y;
                fin_mouvement=1;
                break;
            case bas:
                
                break;
            case haut:
                
                break;
            case gauche:
                
                break;
            case droite:

                break;
        }
    }
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
void remplirTab(int tab[],){
    
}