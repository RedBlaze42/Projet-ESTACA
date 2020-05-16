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

//Diagonales
#define haut_gauche 1
#define haut_droit 2
#define bas_gauche 3
#define bas_droit 4

//Zones
#define PLATEAU 0
#define DEFENSE 1
#define ATTAQUE 2

typedef struct pion{
	int coord_x;
	int coord_y;
	int type; //CTORP CUIRASSE ou PIEGE
	int pv;//2=blindé 1=vivant 0=mort
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
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
}

void deplacer_pions(joueur *joueurSel, joueur *joueur2, int id_pion_sel){
    int en_mouvement=1;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entrée
        int x,y;
        pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
        remplirTab(plateau, joueurSel, joueur2);//Afficher plateau surbrillance
        int direction_prise;
        switch(getch()) {
            case enter:
                joueurSel->pions[id_pion_sel].coord_x=x;//Mauvaise synthaxe
                joueurSel->pions[id_pion_sel].coord_y=y;
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

int pion_peut_attaquer(joueur *joueurSel, joueur *joueur2, pion *pionSel){
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);
    int y = pionSel->coord_y;
    int x = pionSel->coord_x;
    if(y-1>=0 && x-1>=0 && plateau[y-1][x-1]->player!=pionSel->player && plateau[y-1][x-1]->player!=0){//Haut Gauche
        return 1;
    }else if(y+1<DIM_PLATEAU && x-1>=0 && plateau[y+1][x-1]->player!=pionSel->player && plateau[y+1][x-1]->player!=0){//Bas Gauche
        return 1;
    }else if(y-1>=0 && x+1<DIM_PLATEAU && plateau[y-1][x+1]->player!=pionSel->player && plateau[y-1][x+1]->player!=0){//Haut Droite
        return 1;
    }else if(y+1<DIM_PLATEAU && x+1<DIM_PLATEAU && plateau[y+1][x+1]->player!=pionSel->player && plateau[y+1][x+1]->player!=0){//Bas Droite
        return 1;
    }else{
        return 0;
    }
}

int is_pionSurCase(joueur *joueurAff, joueur *joueur2, int x, int y){
    for(int i=0;i<20;i++){
        if(joueurAff->pions[i].coord_x==x && joueurAff->pions[i].coord_y==y && joueurAff->pions[i].pv>0){
            return 1;
        }
    }
    for(int i=0;i<15;i++){
        if(joueur2->pions[i].coord_x==x && joueur2->pions[i].coord_y==y && joueur2->pions[i].pv>0 && joueur2->pions[i].invisible==0){
            return 1;
        }
    }
    return 0;
}

void remplirTab(pion *tab[DIM_PLATEAU][DIM_PLATEAU], joueur *joueur1, joueur *joueur2){
    int i;
    for(i=0;i<DIM_PLATEAU;i++){
        if(joueur1->pions[i].pv>0){
            tab[(joueur1->pions[i]).coord_x][(joueur1->pions[i]).coord_y]=&(joueur1->pions[i]);
        }
    };
     for(i=0;i<DIM_PLATEAU;i++){
        if(joueur2->pions[i].pv>0){
            tab[(joueur2->pions[i]).coord_x][(joueur2->pions[i]).coord_y]=&(joueur2->pions[i]);
        }
    }
}

void afficher_pion(pion* pionAff){//pour afficher une case
    switch((*pionAff).type){
        case 1 : 
            printf("1");
            if((*pionAff).player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            break;
        case 2 :
            printf("2");
            if((*pionAff).player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            break;
        case 3 :
            printf("3");
            if((*pionAff).player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            break;
        default:
            color(0,15);
            printf(" ");
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

void afficherplateau_sel(joueur *joueurAff,joueur *joueur2, int x, int y){//on affiche le plateau avec une case en surbrillance
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurAff, joueur2);
    int i, j;
    for(j=0;j<25;j++){
        for(i=0;i<25;i++){
            if(i==x && j==y){
                color(15,15);
                printf(" ");
            }else{
                if((joueurAff->pions[i]).invisible=0){
                    afficher_pion(&(joueurAff->pions[i]));
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

void afficherplateau(joueur *joueurAff,joueur *joueur2){//on affiche le plateau
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurAff, joueur2);
    int i, j;
    for(j=0;j<25;j++){
        for(i=0;i<25;i++){
            if((*plateau[i][j]).invisible==0){
                afficher_pion(plateau[i][j]);
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