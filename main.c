#include<stdlib.h>
#include<stdio.h>
#include<windows.h>

#define DIM_PLATEAU 25
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
	int invisible; //0=visible pour tout le monde >0=nombre de tours invisibles -1=tout le temps invisible (piège)
	int anti_blindage;
	int player;
} pion;

typedef struct joueur{
    int num;
	pion pions[20];
	int cartes[2];
} joueur;


int is_pionSurCase(joueur attaquant, joueur defenseur, int x, int y);
void remplirTab(pion tab[][], joueur joueurAff, joueur joueur2);
void afficherTab(pion tab[][], joueur joueurAff,joueur joueur2);
void affichercase(pion PiOn[][], joueur joueurAFF);
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

void remplirTab(pion tab[][], joueur joueur1, joueur joueur2){
    int i;
    for(i=0,i<nb,i++){
        if(*joueur1.pion[i].pv>0){
            tab[*joueur1.pions[i].coord_x][*joueur1.pions[i].coord_y]=*joueur1.pion[i];
        }
    }
     for(i=0,i<nb,i++){
        if(*joueur2.pion[i].pv>0){
            tab[*joueur2.pions[i].coord_x][*joueur2.pions[i].coord_y]=*joueur2.pion[i];
        }
    }
}
void affichercase(pion PiOn[][], joueur joueurAFF){//pour afficher une case 
    if(*joueurAFF.pion[i].player==joueur.nb){
       switch(*joueurAFF.pion[i].type){// on vérifie l'indentité du joueur pour l'affichage du pion
            case 1 : printf("1");
                if(*joueuAff.num==1){
                    color(1,15);
                }else{
                    color(4,15);
                }
                break;
            case 2 : printf("2");
                  if(*joueuAff.num==1){
                    color(1,15);
                }else{
                    color(4,15);
                }
                break;
            case 3 : printf("3");
                  if(*joueuAff.num==1){
                    color(1,15);
                }else{
                    color(4,15);
                }
                break;
       }else{
           if(*joueurAFF.pion[i].player!=joueurAFF){// on affiche le pion pour le joueur dont ce n'est pas le tour
               if(*joueurAFF.pion[i].type!=3){
                   printf(" ");
                   color(15,15);
               }else{
                    switch(*joueurAFF.pion[i].type==1){
                        case 1 : printf("1");
                             if(*joueuAff.num==1){
                                color(4,15);
                            }else{
                                color(1,15);
                }
                            break;
                        case 2 : printf("2");
                            if(*joueuAff.num==1){
                                color(4,15);
                            }else{
                                color(1,15);
                            break;
                    }
               }
           }
       }
    }
}
void afficherplateau(pion tab[][], joueur joueurAff,joueur joueur2){//on affiche le plateau
    remplirTab(pion tab[][], joueuAff, joueur2);
    int i, j;
    for(j=0,j<25,j++){
        for(i=0,i<25,i++){
            affichercase(tab[i][j], joueurAff);
        }
        printf("\n");
    }
}
void color (int couleurDuTexte, int couleurDuFond){// permet de gérer les couleurs
     HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(H, couleurDuFond*16+couleurDuTexte);
}