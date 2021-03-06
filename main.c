#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include <conio.h>
#include <time.h>

#define DIM_PLATEAU 11
#define max_pions 19

//Couleurs
#define COULEUR_JOUEURDEF 12
#define COULEUR_JOUEURATT 9

//Types de pions
#define CTORP 1
#define CUIRASSE 2
#define PIEGE 3

//Touches
#define haut 72
#define bas 80
#define droite 77
#define gauche 75
#define enter 13

//Diagonales
#define haut_gauche 1
#define haut_droit 2
#define bas_gauche 3
#define bas_droit 4

//Zones
#define PLATEAU 0
#define DEFENSE 1//Defini aussi numero du joueur defenseur
#define ATTAQUE 2//Defini aussi numero du joueur attaquant
#define PERIPHERIE 3


typedef struct pion{
	int coord_x;
	int coord_y;
	int type; //CTORP CUIRASSE ou PIEGE
	int pv;//2=blinde 1=vivant 0=mort
	int invisible; //0=visible pour tout le monde >0=nombre de tours invisibles -1=tout le temps invisible (piege)
	int anti_blindage;
	int player;
} pion;

typedef struct joueur{
    int num;//Joueur Rouge (Defenseur) = 1 Joueur Bleu (Attaquant) =2
	pion pions[20];
    int nb_pions;
	int cartes[2];
} joueur;

void color (int couleurDuTexte, int couleurDuFond);
void tour(joueur *joueur1,joueur *joueur2);
void deplacer_pions(joueur *joueurSel, joueur *joueur2, int id_pion_sel);
int pion_peut_attaquer(joueur *joueurSel, joueur *joueur2, pion *pionSel);
void selectionner_attaque(joueur *joueurSel, joueur *joueur2);
void selectionner_case(joueur *joueurSel, joueur *joueur2, int *sel_x, int *sel_y, int zone, int vide);
int is_pionSurCase(joueur *joueurAff, joueur *joueur2, int x, int y);
void remplirTab(pion *tab[DIM_PLATEAU][DIM_PLATEAU], joueur *joueur1, joueur *joueur2);
void afficher_pion(pion* pionAff);
int is_in_zone(int zone, int x, int y);
void afficherplateau_sel(joueur *joueurAff,joueur *joueur2, int x, int y);
void afficherplateau(joueur *joueurAff,joueur *joueur2);
void lectureRegles();
int get_fleche();
void placerpions(joueur *joueurR, joueur *joueurB);
int reste_pions_joueur(joueur *joueurSel,int type);
int joueur_peut_attaquer(joueur *joueurSel, joueur *joueur2);
void direction_attaque_dispo(joueur *joueurSel, joueur *joueur2, pion *pionSel, int directions[5]);
int selectionner_pion(joueur *joueurSel, joueur *joueur2);
void color (int couleurDuTexte, int couleurDuFond);

//cartes rouges :
void utiliser_carte_rouge(joueur *joueurR, joueur *joueurB);
void rouge_CA(pion *pionattaque, pion *poinattaquant);
void rouge_bouclier(joueur *joueurSel, joueur *joueur2);
void rouge_permut(joueur *joueur1, joueur *joueur2);
void rouge_furtif(joueur *joueurSel, joueur *joueur2);
void rouge_piege(joueur *joueurSel, joueur *joueur2);

//cartes bleues :
void utiliser_carte_bleu(joueur *joueurB, joueur *joueurR);
void bleu_renfort(joueur *joueur1,joueur *joueur2);
void bleu_mort(joueur *joueurSel, joueur *joueur2);
void bleu_anti_blindage(joueur *joueurSel, joueur *joueur2);
void bleu_deplacement_multiple(joueur *joueurSel, joueur *joueur2);

int main(){
    joueur joueurB;//Attaquant
    joueurB.num=ATTAQUE;
    joueurB.nb_pions=19;
    joueur joueurR;//Defenseur
    joueurR.num=DEFENSE;
    joueurR.nb_pions=15;
    int gagnant;
    int d=0;

    lectureRegles();

    srand(time(0));//Distributiond des cartes
    joueurR.cartes[0]=(rand() % 4)+1;
    do{
        joueurR.cartes[1]=(rand() % 4)+1;
    }while(joueurR.cartes[1]==joueurR.cartes[0]);

    joueurB.cartes[0]=(rand() % 4)+2;
    do{
        joueurB.cartes[1]=(rand() % 4)+2;
    }while(joueurB.cartes[1]==joueurB.cartes[0]);
    joueurR.cartes[0]=2;
    joueurR.cartes[1]=1;


    placerpions( &joueurR, &joueurB);
    printf("Joueur Rouge tapez r si vous souhaitez commencer sinon tapez b\n");
    char commence;
    while(commence!='b' && commence!='r') scanf("%c",&commence);
    int x=0;
    int y=0;

    int vR=0;//Victoire Bleue ou Rouge
    int vB=0;
    while(vR==0 && vB==0){
        if(commence=='b'){
            tour(&joueurB,&joueurR);
        }else{
            tour(&joueurR,&joueurB);
        }
        int vR=reste_pions_joueur(&joueurR, -1);//-1 pour n'importe quel type de pion
        int vB=reste_pions_joueur(&joueurB, -1);
        if(vR==0 && vB==0){
            if(is_in_zone(ATTAQUE, joueurR.pions[1].coord_x, joueurR.pions[1].coord_y)==1){
                vR=1;
            }
            if(joueurR.pions[1].pv==0){
                vB=1;
            }
        }
    }
    if(vR==1){
        printf("Bravo pour cette belle victoire, j'ai toujours su que le rouge etait la couleur des vainqueurs.");
    }else{
        printf("Bravo pour cette belle victoire, j'ai toujours su que le bleu etait la couleur des vainqueurs.");
    }
}

//Déroulement du jeu

void tour(joueur *joueur1,joueur *joueur2){//structure du jeux permet aux tour de bien se derouler. Et aux joueurs de jouer a tour de role
    system("cls");

    for(int i=0;i<2;i++){
        if(joueur1->num==DEFENSE){
            for(int i=0;i<joueur1->nb_pions;i++){
                if(joueur1->pions[i].invisible>0) joueur1->pions[i].invisible--;
            }
            printf("C'est au tour du joueur rouge\n\n");
        }else{
            printf("C'est au tour du joueur bleu\n\n");
        }
        if(joueur1->num==DEFENSE){//Menu defenseur
            if(joueur1->cartes[0]!=0 || joueur1->cartes[1]!=0){
                printf("Voulez vous utiliser une carte ? o/n ");
                char c=0;
                while(c!='o' && c!='n') scanf("%c",&c);
                if(c=='o'){
                    utiliser_carte_rouge(joueur1, joueur2);
                    system("pause");
                }
            }
            printf("Veuillez selectionner une action:\n1. Deplacer votre cuirrasse\n2. Deplacer 2 contre-torpilleurs\n");
            int d=0;
            while(d!=1 && d!=2) scanf("%d",&d);

            if(d==2){
                int deplacements_restants=2;
                while(deplacements_restants>0){
                    if(reste_pions_joueur(joueur1,CTORP)==0 || (reste_pions_joueur(joueur1,CTORP)==1 && deplacements_restants==1) ){//Soit le joueur n'as plus de CTORP soit il en a qu'un seul qu'il a dejà deplace
                        printf("Vous n'avez plus de contre-torpilleurs\n");
                    }
                    deplacer_pions(joueur1,joueur2,selectionner_pion(joueur1,joueur2));
                    printf("Contre-Torpilleur deplace !\n");
                    afficherplateau(joueur1, joueur2);
                    deplacements_restants--;
                    system("pause");
                }
            }else{
                printf("Deplacement du cuirrasse\n");
                deplacer_pions(joueur1,joueur2,0);//Le cuirrasse est toujours au debut du tableau
                printf("Cuirrasse deplace !\n");
                afficherplateau(joueur1, joueur2);
                system("pause");
            }
            if(joueur_peut_attaquer(joueur1,joueur2)==1){
                printf("Voulez vous attaquer ? o/n\n");
                char c=0;
                while(c!='o' && c!='n') scanf("%c",&c);
                if(c=='o') selectionner_attaque(joueur1,joueur2);
            }
            printf("Fin du tour defenseur\n");
        }else{
            system("pause");
            if(joueur1->cartes[0]!=0 || joueur1->cartes[1]!=0){
                printf("Voulez vous utiliser une carte ? o/n\n");
                char c=0;
                while(c!='o' && c!='n') scanf("%c",&c);
                if(c=='o'){
                    utiliser_carte_bleu(joueur1,joueur2);
                    system("pause");
                }
            }
            int choix_effectue=0;
            int d;
            if(joueur_peut_attaquer(joueur1,joueur2)==1){
                printf("Veuillez selectionner une action:\n1. Attaquer un ennemi\n2. Deplacer 2 contre-torpilleurs\n");
                d=0;
                while(d!=1 && d!=2) scanf("%d",&d);
            }else{//Le joueur ne peut pas attaque donc il doit deplacer 2 pions
                d=2;
            }

            if(d==1){
                if(joueur_peut_attaquer(joueur1,joueur2)==1){
                    selectionner_attaque(joueur1,joueur2);
                }else{
                    printf("Vous n'etes à porte d'aucun ennemi\n");
                }
            }else if(d==2){
                int deplacements_restants=2;
                while(deplacements_restants>0){
                    if(reste_pions_joueur(joueur1,CTORP)==0 || (reste_pions_joueur(joueur1,CTORP)==1 && deplacements_restants==1) ){//Soit le joueur n'as plus de CTORP soit il en a qu'un seul qu'il a dejà deplace
                        printf("Vous n'avez plus de contre-torpilleurs\n");
                    }
                    deplacer_pions(joueur1,joueur2,selectionner_pion(joueur1,joueur2));
                    printf("Contre-Torpilleur deplace !\n");
                    afficherplateau(joueur1, joueur2);
                    deplacements_restants--;
                    system("pause");
                }
            }
            printf("Fin du tour attaquant\n");
            system("pause");
        }
        joueur *temporaire=joueur2;//Interverti les 2 joueurs pour la deuxieme partie du tour
        joueur2=joueur1;
        joueur1=temporaire;
        system("cls");
    }

}

int reste_pions_joueur(joueur *joueurSel,int type){//Si type=-1, on verifie juste si il reste des pions au joueur
    for(int i;i<joueurSel->nb_pions;i++){
        if( (joueurSel->pions[i].type==type || type==-1) && joueurSel->pions[i].pv>0){
            return 1;
        }
    }
}

void deplacer_pions(joueur *joueurSel, joueur *joueur2, int id_pion_sel){//permet de deplacer un pion grace aux fleches du clavier
    int en_mouvement=1;
    int x=joueurSel->pions[id_pion_sel].coord_x;
    int y=joueurSel->pions[id_pion_sel].coord_y;
    int direction_prise=0;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entree
        pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
        remplirTab(plateau, joueurSel, joueur2);//Afficher plateau surbrillance
        system("cls");
        afficherplateau_sel(joueurSel, joueur2,x,y);
        switch(get_fleche()) {
            case enter:
                if(plateau[x][y]!=NULL && plateau[x][y]->type==PIEGE){
                    joueurSel->pions[id_pion_sel].pv--;
                    plateau[x][y]->pv=0;
                }
                joueurSel->pions[id_pion_sel].coord_x=x;
                joueurSel->pions[id_pion_sel].coord_y=y;
                en_mouvement=0;
                break;
            case bas:
                if(y+1<DIM_PLATEAU && (is_pionSurCase(joueurSel,joueur2,x,y+1)==0 || plateau[x][y+1]==&(joueurSel->pions[id_pion_sel]) ) && (direction_prise==0 || direction_prise==bas || direction_prise==haut)){
                    y++;
                    if(direction_prise==0) direction_prise=bas;
                }
                break;
            case haut:
                if(y-1>=0 && (is_pionSurCase(joueurSel,joueur2,x,y-1)==0 || plateau[x][y-1]==&(joueurSel->pions[id_pion_sel]) ) && (direction_prise==0 || direction_prise==bas || direction_prise==haut)){
                    y--;
                    if(direction_prise==0) direction_prise=haut;
                }
                break;
            case gauche:
                if(x-1>=0 && (is_pionSurCase(joueurSel,joueur2,x-1,y)==0 || plateau[x-1][y]==&(joueurSel->pions[id_pion_sel]) ) && (direction_prise==0 || direction_prise==droite || direction_prise==gauche)){
                    x--;
                    if(direction_prise==0) direction_prise=gauche;
                }
                break;
            case droite:
                if(x+1<DIM_PLATEAU && (is_pionSurCase(joueurSel,joueur2,x+1,y)==0 || plateau[x+1][y]==&(joueurSel->pions[id_pion_sel]) ) && (direction_prise==0 || direction_prise==droite || direction_prise==gauche)){
                    x++;
                    if(direction_prise==0) direction_prise=droite;
                }
                break;
        }
        if(joueurSel->pions[id_pion_sel].coord_x==x && joueurSel->pions[id_pion_sel].coord_y==y) direction_prise=0;
    }
    system("cls");
}

void remplirTab(pion *tab[DIM_PLATEAU][DIM_PLATEAU], joueur *joueur1, joueur *joueur2){// remplit le tableau
    for(int j=0; j<DIM_PLATEAU; j++){
        for(int i=0; i<DIM_PLATEAU; i++){
            tab[i][j]=NULL;
        }
    }

    for(int i=0;i<joueur1->nb_pions;i++){//remplissage des cases du joueur 1
        if(joueur1->pions[i].pv>0){
            if(tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y]!=NULL){
                if(tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y]->type==PIEGE){
                    tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y]=&(joueur1->pions[i]);
                }
            }else{
                tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y]=&(joueur1->pions[i]);
            }
        }
    }

    for(int i=0;i<joueur2->nb_pions;i++){//remplissage des cases du joueur 2
        if(joueur2->pions[i].pv>0){
            if(tab[joueur2->pions[i].coord_x][joueur2->pions[i].coord_y]!=NULL){
                if(tab[joueur2->pions[i].coord_x][joueur2->pions[i].coord_y]->type==PIEGE){
                    tab[joueur2->pions[i].coord_x][joueur2->pions[i].coord_y]=&(joueur2->pions[i]);
                }
            }else{
                tab[joueur2->pions[i].coord_x][joueur2->pions[i].coord_y]=&(joueur2->pions[i]);
            }
        }
    }

}

//Vérifications booléennes

int is_pionSurCase(joueur *joueurAff, joueur *joueur2, int x, int y){// on controle si une case est occupee par un pion
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurAff, joueur2);
    if(plateau[x][y]!=NULL && plateau[x][y]->type==PIEGE) return 0;
    return plateau[x][y]!=NULL;
}

int is_in_zone(int zone, int x, int y){// Permet de savoir dans quelle zone prescise se trouve un pion
    if(zone==PERIPHERIE){
        return (x==0 || y==0 || x==DIM_PLATEAU-1 || y==DIM_PLATEAU-1) && x>=0 && y>=0 && x<DIM_PLATEAU && y<DIM_PLATEAU;
    }else if(zone==PLATEAU){
        return x>=0 && y>=0 && x<DIM_PLATEAU && y<DIM_PLATEAU;
    }else if(zone==DEFENSE){
        return x>2 && x<8 && y>2 && y<8 && x>=0 && y>=0 && x<DIM_PLATEAU && y<DIM_PLATEAU;
    }else if(zone==ATTAQUE){
        return (x>2 && x<8 && y>2 && y<8)==0 && x>=0 && y>=0 && x<DIM_PLATEAU && y<DIM_PLATEAU;
    }
}

//Attaque

int joueur_peut_attaquer(joueur *joueurSel, joueur *joueur2){//renvoie si un joueur peut attaquer en retournant 1
    for(int i=0;i<joueurSel->nb_pions;i++){
        if(pion_peut_attaquer(joueurSel, joueur2, &(joueurSel->pions[i]))==1){
            return 1;
        }
    }
    return 0;
}

void direction_attaque_dispo(joueur *joueurSel, joueur *joueur2, pion *pionSel, int directions[5]){//revoie les attaques possibles
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);
    int y = pionSel->coord_y;
    int x = pionSel->coord_x;
    for(int i=0;i<5;i++) directions[i]=0;
    if(y-1>=0 && x-1>=0 && plateau[x-1][y-1]!=NULL && plateau[x-1][y-1]->player!=pionSel->player){//Haut Gauche
        directions[haut_gauche]=1;
    }
    if(y+1<DIM_PLATEAU && x-1>=0 && plateau[x-1][y+1]!=NULL && plateau[x-1][y+1]->player!=pionSel->player){//Bas Gauche
        directions[bas_gauche]=1;
    }
    if(y-1>=0 && x+1<DIM_PLATEAU && plateau[x+1][y-1]!=NULL && plateau[x+1][y-1]->player!=pionSel->player){//Haut Droite
        directions[haut_droit]=1;
    }
    if(y+1<DIM_PLATEAU && x+1<DIM_PLATEAU && plateau[x+1][y+1]!=NULL && plateau[x+1][y+1]->player!=pionSel->player){//Bas Droite
        directions[bas_droit]=1;
    }
}

int pion_peut_attaquer(joueur *joueurSel, joueur *joueur2, pion *pionSel){//Retourne 1 si pionSel a un ennemi à porte
    if(pionSel->pv==0) return 0;
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);
    int y = pionSel->coord_y;
    int x = pionSel->coord_x;
    if(y-1>=0 && x-1>=0 && plateau[x-1][y-1]!=NULL && plateau[x-1][y-1]->player!=pionSel->player){//Haut Gauche
        return 1;
    }else if(y+1<DIM_PLATEAU && x-1>=0 && plateau[x-1][y+1]!=NULL && plateau[x-1][y+1]->player!=pionSel->player){//Bas Gauche
        return 1;
    }else if(y-1>=0 && x+1<DIM_PLATEAU && plateau[x+1][y-1]!=NULL && plateau[x+1][y-1]->player!=pionSel->player){//Haut Droite
        return 1;
    }else if(y+1<DIM_PLATEAU && x+1<DIM_PLATEAU && plateau[x+1][y+1]!=NULL && plateau[x+1][y+1]->player!=pionSel->player){//Bas Droite
        return 1;
    }else{
        return 0;
    }
}

void selectionner_attaque(joueur *joueurSel, joueur *joueur2){//Permet au joueur de selectionner le pion qui attaque et la case a attaquer
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);

    int i=0;
    int fin_boucle=0;
    while(fin_boucle==0){
        while(pion_peut_attaquer(joueurSel, joueur2, &(joueurSel->pions[i]))!=1){//Cherche le prochain pion en position d'attaquer
            i++;
            if(i>=joueurSel->nb_pions) i=0;
        }
        system("cls");
        afficherplateau_sel(joueurSel, joueur2, joueurSel->pions[i].coord_x, joueurSel->pions[i].coord_y);
        if(get_fleche()==enter){//Pion selectionne
            printf("\nattaque sel\n");
            pion *pionSel=&(joueurSel->pions[i]);
            int y = pionSel->coord_y;
            int x = pionSel->coord_x;
            int y_attaque=y-1;
            int x_attaque=x-1;
            int directions[5];
            int direction_sel=1;
            direction_attaque_dispo(joueurSel, joueur2,pionSel,directions);
            while(fin_boucle==0){
                system("cls");
                while(directions[direction_sel]==0){
                    direction_sel++;
                    if(direction_sel>4)
                        direction_sel=1;
                }
                switch(direction_sel){
                    case haut_droit:
                        y_attaque=y-1;
                        x_attaque=x-1;
                        break;
                    case haut_gauche:
                        y_attaque=y-1;
                        x_attaque=x+1;
                        break;
                    case bas_gauche:
                        y_attaque=y+1;
                        x_attaque=x-1;
                        break;
                    case bas_droit:
                        y_attaque=y+1;
                        x_attaque=x+1;
                        break;
                }
                afficherplateau_sel(joueurSel, joueur2, x_attaque, y_attaque);
                if(get_fleche()==enter){//Pion a attaquer selectionne
                    plateau[x_attaque][y_attaque]->pv--;
                    if(pionSel->anti_blindage==1) plateau[x_attaque][y_attaque]->pv=0;
                    remplirTab(plateau, joueurSel, joueur2);//actualise le plateau
                    if (plateau[x_attaque][y_attaque]==NULL){
                        pionSel->coord_x=x_attaque;
                        pionSel->coord_y=y_attaque;
                    }else if(plateau[x_attaque][y_attaque]->type==PIEGE){
                        pionSel->pv--;
                    }

                    fin_boucle=1;

                    system("cls");
                    printf("Touche !\n");
                    afficherplateau(joueurSel, joueur2);
                    system("pause");
                }else{
                    direction_sel++;
                    if(direction_sel>4)
                        direction_sel=1;
                    usleep(100000);
                }
            }
        }
        i++;
    }
}


//Interface utilisateur

int get_fleche(){//Attends que l'utilisateur presse une fleche ou la touche entree, du clavier
    char c = getch();
    char init_c=c;
    if(c!=enter && c!=gauche && c!=droite && c!=haut && c!=bas)
        c=getch();
    return c;
}

void selectionner_case(joueur *joueurSel, joueur *joueur2, int *sel_x, int *sel_y, int zone, int vide){//selectionne une case et la transmet par les variables x y
    int en_mouvement=1;
    int x=*sel_x;
    int y=*sel_y;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entree
        pion plateau[DIM_PLATEAU][DIM_PLATEAU];
        afficherplateau_sel(joueurSel, joueur2,x,y);
        int direcion_prise;
        switch(get_fleche()) {
            case enter:
                if( (is_in_zone(zone,x,y)==1) && (vide==0 || is_pionSurCase(joueurSel,joueur2,x,y)==0) ){
                    *sel_x=x;
                    *sel_y=y;
                    en_mouvement=0;
                }
                system("pause");
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
                if(is_in_zone(zone,x+1,y)==1) x=x+1;
                break;
        }
        system("cls");
    }
}

int selectionner_pion(joueur *joueurSel, joueur *joueur2){
    int en_selection=1;
    int id_pion=0;
    while(en_selection==1){
        system("cls");
        afficherplateau_sel(joueurSel, joueur2, joueurSel->pions[id_pion].coord_x, joueurSel->pions[id_pion].coord_y);
        usleep(200000);//Pour eviter de sauter trop de pion à chaque fois
        if(id_pion>joueurSel->nb_pions) id_pion=0;
        int fleche=get_fleche();
        if(fleche==enter){
            if(joueurSel->pions[id_pion].type!=CUIRASSE && joueurSel->pions[id_pion].type!=PIEGE) en_selection=0;
        }else if(fleche==gauche){
            id_pion--;
            if(joueurSel->pions[id_pion].type==CUIRASSE || joueurSel->pions[id_pion].type==PIEGE) id_pion++;
            if(id_pion<0) id_pion=joueurSel->nb_pions-1;
        }else if(fleche==droite){
            id_pion++;
            if(joueurSel->pions[id_pion].type==CUIRASSE || joueurSel->pions[id_pion].type==PIEGE) id_pion++;
            if(id_pion>=joueurSel->nb_pions) id_pion=0;
        }

    }
    return id_pion;
}

void placerpions(joueur *joueurR, joueur *joueurB){
    joueurR->pions[0].type=CUIRASSE;// on initialise les pions rouges et on initialise leurs pv a 0 pour faciliter le placement
    joueurR->pions[0].coord_x=5;
    joueurR->pions[0].coord_y=5;
    joueurR->pions[0].pv=1;
    joueurR->pions[0].invisible=0;
    joueurR->pions[0].anti_blindage=0;
    joueurR->pions[0].player=DEFENSE;
    for(int i=1;i<13;i++){//initialistion contre torpilleurs(blinde ou pas)
        joueurR->pions[i].type=CTORP;
        joueurR->pions[i].coord_x=5;
        joueurR->pions[i].coord_y=6;
        joueurR->pions[i].pv=0;
        joueurR->pions[i].invisible=0;
        joueurR->pions[i].anti_blindage=0;
        joueurR->pions[i].player=DEFENSE;
    }
    for(int i=13; i<15; i++){//initialistion piege
        joueurR->pions[i].type=PIEGE;
        joueurR->pions[i].coord_x=5;
        joueurR->pions[i].coord_y=4;
        joueurR->pions[i].pv=0;
        joueurR->pions[i].invisible=-1;
        joueurR->pions[i].anti_blindage=0;
        joueurR->pions[i].player=DEFENSE;
    }
    for(int i=0;i<19;i++){// on initialise les pions rouges et on initialise leurs pv a 0 pour faciliter le placement
        joueurB->pions[i].type=CTORP;
        joueurB->pions[i].pv=0;
        joueurB->pions[i].coord_x=0;
        joueurB->pions[i].coord_y=0;
        joueurB->pions[i].anti_blindage=0;
        joueurB->pions[i].invisible=0;
        joueurB->pions[i].player=ATTAQUE;
    }


    printf("Au joueur rouge de placer ses pions\n");
    for(int i=1;i<joueurR->nb_pions-2;i++){// le joueur rouge place ses pions chacun leur tour et les pv s'initialisent en meme temps
        if(i<3){
            printf("Vous deplacez votre contre torpilleur blinde numero %d\n", i);
        }else{
            printf("Vous deplacez votre contre torpilleur numero %d\n", i-2);
        }
        selectionner_case(joueurR, joueurB, &(joueurR->pions[i].coord_x), &(joueurR->pions[i].coord_y), DEFENSE, 1);//On a pas le droit de bouger le cuirasse au debut
        if(i<3){
            joueurR->pions[i].pv=2;
        }else{
            joueurR->pions[i].pv=1;
        }
    }
    printf("Vous avez place tout vos pions\n");
    system("pause");
    system("cls");
    printf("Au joueur bleu de placer ses pions\n");
    for(int i=0;i<joueurB->nb_pions;i++){// le joueur rouge place ses pions chacun leur tour et les pv s'initialisent en meme temps
        if(i<2){
            printf("Vous deplacez votre contre torpilleur blinde numero %d\n", i+1);
        }else{
            printf("Vous deplacez votre contre torpilleur numero %d\n", i-1);
        }
        selectionner_case(joueurR,joueurB , &(joueurB->pions[i].coord_x), &(joueurB->pions[i].coord_y), ATTAQUE, 1);
        if(i<2){
            joueurB->pions[i].pv=2;
        }else{
            joueurB->pions[i].pv=1;
        }
    }
    afficherplateau(joueurR,joueurB);
}

//Affichage

void lectureRegles(){
    int c;
    system("CLS");
    while(c!=5){//Le while permet au joueur de voir chaque sectioon des regles autant de fois qu'il veut.
        printf("Bienvenu sur seawars.\nLe but du jeu est de couler tous les bateaux de l'adversaire,\n ou pour le joueur rouge de sortir le cuirasse de la zone de defense ou\n pour le joueur bleu de couler le cuirasse adverse.\n\nAu debut de la partie, le premier joueur dispose le cuirasse dans la case centrale marquee d'un rond,\npuis il place ses douze contre-torpilleurs a sa convenance, a l'interieur du carre central de vingt-cinq cases.\nLorsque le premier joueur a fini de placer ses pions, c'est au tour du deuxieme joueur\nde disposer ses vingt contretorpilleurs comme il en a envie, mais a l'exterieur du carre central.\nUne fois termine le placement de tous les pions, chaque joueur pioche deux cartes speciales de la couleur correspondante.\nLe rouge a le pouvoir de decider a qui revient de jouer le premier coup, et la partie peut alors commencer.\n ");
        do{
            printf("Pour voir les deplacement/attaques des pions tapez 1\nPour voir la nature des pions tapez 2\nPour voir les capacitees des cartes speciales bleues tapez 3\nPour voir les capacitees des cartes speciales rouges tapez 4\nPour passer directement au debut de la partie tapez 5\nEt pour voir la legende tapez 6.");
            scanf("%d",&c);
        }while(c<1 || c>6);
        system("CLS");
        switch(c){
            case 1:
                printf("Deplacments:\nToutes les pieces se deplacent en ligne orthogonale et peuvent parcourir des rangees entieres de cases vides\nen s'arretant où bon leur semble,mais elles ne peuvent pas sauter par-dessus une case occupee.\n Elles ne peuvent pas eliminer un pion (de leur camp ou du camp adverse) qui bloque leur avancee,\n mais elles doivent s'arreter sur une case attenante \n\nAttaques:\nLes pieces s’attaquent mutuellement entre cases voisines directe,\nen se deplaçant en diagonale dans n'importe quelle direction.\nLorsqu'un un navire detruit un autre navire, il enleve du damier le pion elimine et prend sa place.\nAttaquer un pion menace n'est pas une obligation.");
                break;
            case 2:
                printf("Il existe 4 types de pions:\n- Les cuirasses\n- Les contretorpilleurs blindes(pions moyens, doivent etre attaque deux fois pour etre detruits,\nattaques une fois, ils perdent leurs blindages et deviennent des contre-torpilleurs normaux.)\n- Les contre torpilleurs\n- Les cartes pieges(voir cartes piege rouges et bleues)\n\n\nLe joueur rouge:\n- Un cuirasse(le grand pion),\n- Deux contretorpilleurs blindes\n- Dix contre-torpilleurs(pions normaux).\n\nSon adversaire, le joueur bleu:\n- Deux contretorpilleurs blindes\n- Dix-huit contre-torpilleurs\n");
                break;
            case 3:
                printf("1. Carte deplacement libre: Cette carte peut etre activee juste avant un deplacement\net permettra a un pion de se deplacer en diagonale durant un tour.\n\n2. Carte mort subite : Cette carte peut etre utilisee au debut ou a la fin d’un tout,\nelle permet d’attaquer n’importe quel navire ennemi (excepte le cuirasse).\n\n3. Carte renfort : Cette carte peut etre utilisee au debut ou a la fin d’un tout,\nelle permet de rappeler deux contre-torpilleurs detruits ou un blinde sur le plateau.\nLes navires rappeles doivent etre deposer forcement sur la peripherie du plateau. \n\n4. Carte Anti-blindage : Cette carte peut etre activee au debut d’un tour, elle permet de rendre,\ndurant toute la partie, un contre-torpilleur capable de detruire un blinde en un seul coup.\n\n5. Carte deplacement multiple : Cette carte peut etre activee au debut d’un tour,\nelle permet d’effectuer trois deplacements au lieu de deux.\n");
                break;
            case 4:
                printf("1. Carte piege : Cette carte peut etre activee au debut ou a la fin d’un tour et\nelle permet de placer deux pieges sur la carte. Ces piges sont invisibles pour l’adversaire.\nSi lors d’un deplacement un pion adverse passe par un piege,\nle pion sera detruit et le piege desactive. Si c’est un blinde qui passe par le piege, il perdra son blindage.\n\n2. Carte bouclier : Cette carte peut etre activee au debut ou a la fin d’un tour et\nelle permet de rajouter un blindage supplementaire a n’importe quel navire.\n\n3. Carte contre-attaque : Cette carte peut etre activee lors qu’on un pion rouge est attaque.\nLe pion attaque ne subit pas de degâts et c’est l’attaquant qui subit les degâts a sa place.\n\n4. Carte permutation : Cette carte peut etre activee au debut ou a la fin d’un tour et\nelle permet a deux pions (rouges ou bleus) d’echanger leurs positons sur le plateau.\n\n 5. Carte deplacement furtif : Cette carte peut etre activee juste avant un deplacement.\nLe pion deplace devient invisible pour l’adversaire durant le tour suivant.\nSi lors d’un deplacement d’un pion bleu, il croise sur son chemin le navire invisible,\ncelui-ci sera decouvert et le pion bleu devra, bien sûr, s’arreter.\n");
                break;
            case 6:
                printf("P=piege\nO=cuirasse\n%c=contre torpilleur blinde\no=contre tropilleur\nUn des signes precedent en vert signifie que le pion est anti blindage\nUn des signes precedent en violet signifie que le pion est invisible\n\n",147);
                break;
            case 5:
                printf("Ok let's go\n");
                sleep(1);
        }
    }
}

void afficherplateau_sel(joueur *joueurAff,joueur *joueur2, int sel_x, int sel_y){//On affiche le plateau avec une case en surbrillance.
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];

    remplirTab(plateau, joueurAff, joueur2);
    //printf("t%d\n",)
    for(int y=0;y<DIM_PLATEAU;y++){
        for(int x=0;x<DIM_PLATEAU;x++){
            if(x==sel_x && y==sel_y){//detection et affichage du curseur
                color(0,10);
                printf(" ");
                color(15,0);
            }else{
                if(plateau[x][y]!=NULL && (plateau[x][y]->invisible==0 || plateau[x][y]->player==joueurAff->num) ){
                    afficher_pion(plateau[x][y]);
                }else{
                    if(is_in_zone(DEFENSE,x,y)==1){
                        color(0,7);
                    }else{
                        color(0,15);
                    }
                    printf("-");
                    color(15,0);
                }
            }
        }
        printf("\n");
    }
    printf("\n");
}

void afficherplateau(joueur *joueurAff,joueur *joueur2){//On affiche le plateau.
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];

    remplirTab(plateau, joueurAff, joueur2);
    for(int y=0;y<DIM_PLATEAU;y++){//On affiche le plateau dans la hauteur.
        for(int x=0;x<DIM_PLATEAU;x++){//On affiche chaque case d'un ligne.
            if(plateau[x][y]!=NULL && (plateau[x][y]->invisible==0 || plateau[x][y]->player==joueurAff->num) ){//On regarde s'il y a un pion sur la case a afficher.
                afficher_pion(plateau[x][y]);
            }else{
                if(is_in_zone(DEFENSE,x,y)==1){
                        color(0,7);
                }else{
                        color(0,15);
                }
                printf("-");
                color(15,0);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void afficher_pion(pion* pionAff){//pour afficher une case
    int couleur_pion=0;
    int couleur_fond=0;
    if((*pionAff).player==ATTAQUE){//On verifie l'appartenance des pions
        couleur_fond=COULEUR_JOUEURATT;
    }else{
        couleur_fond=COULEUR_JOUEURDEF;
    }

    if(pionAff->anti_blindage==1){
        couleur_pion=2;
    }else if(pionAff->invisible!=0){
        couleur_pion=5;
    }else{
        couleur_pion=15;
    }
    color(couleur_pion,couleur_fond);
    switch(pionAff->type){// on affiche differement chaque type de pion
        case CTORP :
            if(pionAff->pv==2){
                printf("%c",147);
            }else{
                printf("%c",111);
            }
            break;
        case CUIRASSE :
            printf("%c",79);
            break;
        case PIEGE :
            printf("P");
            break;
        default:
            color(0,15);
            printf("-");
    }
    color(15,0);
}

void color (int couleurDuTexte, int couleurDuFond){// permet de gerer les couleurs
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDuFond*16+couleurDuTexte);
}


//Cartes rouges

void utiliser_carte_rouge(joueur *joueurR, joueur *joueurB){
    printf("Quel carte voulez vous utiliser:\n");
    for(int i=0; i<2;i++){
        switch(joueurR->cartes[i]){
            case 1:
                printf("1. Carte piege : Cette carte peut etre activee au debut ou a la fin d’un tour et\n   elle permet de placer deux pieges sur la carte. Ces piges sont invisibles pour l’adversaire.\n    Si lors d’un deplacement un pion adverse passe par un piege,\n   le pion sera detruit et le piege desactive. Si c’est un blinde qui passe par le piege, il perdra son blindage.\n");
                break;
            case 2:
                printf("2. Carte bouclier : Cette carte peut etre activee au debut ou a la fin d’un tour et\n  elle permet de rajouter un blindage supplementaire a n’importe quel navire.\n");
                break;
            case 3:
                printf("3. Carte deplacement furtif : Cette carte peut etre activee juste avant un deplacement.\n    Le pion deplace devient invisible pour l’adversaire durant le tour suivant.\n   Si lors d’un deplacement d’un pion bleu, il croise sur son chemin le navire invisible,\n   celui-ci sera decouvert et le pion bleu devra, bien sûr, s’arreter.\n");
                break;
            case 4:
                printf("4. Carte permutation : Cette carte peut etre activee au debut ou a la fin d’un tour et\nelle permet a deux pions (rouges ou bleus) d’echanger leurs positons sur le plateau.\n");
                break;
            case 5:
                printf("5. Carte contre-attaque : Cette carte peut etre activee lors qu’on un pion rouge est attaque.\n   Le pion attaque ne subit pas de degâts et c’est l’attaquant qui subit les degâts a sa place.\n");
                break;
        }
    }
    int carte=-1;
    while(carte!=joueurR->cartes[0] && carte!=joueurR->cartes[1]) scanf("%d",&carte);
    switch(carte){
        case 1:
            rouge_piege(joueurR,joueurB);
            break;
        case 2:
            rouge_bouclier(joueurR,joueurB);
            break;
        case 3:
            rouge_furtif(joueurR,joueurB);
            break;
        case 4:
            rouge_permut(joueurR,joueurB);
            break;
        case 5:
            rouge_CA(joueurR,joueurB);
            break;
    }
    if(joueurR->cartes[0]==carte){
        joueurR->cartes[0]=0;
    }else{
        joueurR->cartes[1]=0;
    }
}

void rouge_CA(pion *pionattaque, pion *pionattaquant){//carte speciale rouge contre attaque
    printf("Vous utilisez votre carte contre attaque joueur rouge\n");
    pionattaque->pv=pionattaque->pv+1;
    pionattaquant->pv=pionattaquant->pv-1;
}

void rouge_permut(joueur *joueur1, joueur *joueur2){//carte speciale rouge de permutation
    printf("Joueur rouge vous utilisez votre carte permutation\n");
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueur1, joueur2);
    system("cls");
    int x1=0;
    int y1=0;
    do{
        printf("Selectionnez le premier pion à permuter:\n");
        selectionner_case(joueur1,joueur2,&x1,&y1,PLATEAU,0);
    }while(plateau[x1][y1]==NULL || plateau[x1][y1]->type==CUIRASSE);

    system("cls");
    int x2=0;
    int y2=0;
    do{
        printf("Selectionnez le deuxieme pion à permuter:\n");
        selectionner_case(joueur1,joueur2,&x2,&y2,PLATEAU,0);
    }while(plateau[x2][y2]==NULL || plateau[x2][y2]->type==CUIRASSE);
    plateau[x1][y1]->coord_x=x2;
    plateau[x1][y1]->coord_y=y2;
    plateau[x2][y2]->coord_x=x1;
    plateau[x2][y2]->coord_y=y1;//Pas besoin de tampon car le plateau n'est pas actualise

    printf("Les 2 pions ont bien ete permutes !\n");
    afficherplateau(joueur1, joueur2);
    system("pause");
}

void rouge_bouclier(joueur *joueurSel, joueur *joueur2){//carte speciale rouge bouclier
    int id_pion=selectionner_pion(joueurSel, joueur2);//EMILE MESSAGE
    joueurSel->pions[id_pion].pv++;
}

void rouge_furtif(joueur *joueurSel, joueur *joueur2){//carte speciale rouge deplacement furtif
    int id_pion=selectionner_pion(joueurSel, joueur2);//selection du pion a cacher
    joueurSel->pions[id_pion].invisible=1;
}

void rouge_piege(joueur *joueurSel, joueur *joueur2){//carte speciale rouge piege
    for(int i=0;i<2;i++){//placement des deux piege
        printf("vous placez le piege numero %d\n", i+1);
        joueurSel->pions[13+i].pv=1;
        joueurSel->pions[13+i].invisible=-1;
        selectionner_case(joueurSel, joueur2,&(joueurSel->pions[13+i].coord_x),&(joueurSel->pions[13+i].coord_y),PLATEAU,1);
    }
    system("pause");
}


//Cartes bleus

void utiliser_carte_bleu(joueur *joueurB, joueur *joueurR){
    printf("Quel carte voulez vous utiliser: %d %d \n",joueurB->cartes[0],joueurB->cartes[1]);
    for(int i=0; i<2;i++){
        switch(joueurB->cartes[i]){
            case 1:
                printf("1. Carte deplacement libre: Cette carte peut etre activeejuste avant un deplacement\n    et permettra a un pion de se deplacer en diagonale durant un tour.\n");
                break;
            case 2:
                printf("2. Carte mort subite : Cette carte peut etre utilisee au debut ou a la fin d’un tout,\n    elle permet d’attaquer n’importe quel navire ennemi (excepte le cuirasse).\n");
                break;
            case 3:
                printf("3. Carte renfort : Cette carte peut etre utilisee au debut ou a la fin d’un tout,\n    elle permet de rappeler deux contre-torpilleurs detruits ou un blinde sur le plateau. Les navires rappeles doivent etre deposer forcement sur la peripherie du plateau. \n");
                break;
            case 4:
                printf("4. Carte Anti-blindage : Cette carte peut etre activee au debut d’un tour, elle permet de rendre,\n    durant toute la partie, un contre-torpilleur capable de detruire un blinde en un seul coup.\n");
                break;
            case 5:
                printf("5. Carte deplacement multiple : Cette carte peut etre activee au debut d’un tour,\n    elle permet d’effectuer trois deplacements au lieu de deux.\n");
                break;
        }
    }
    int carte=-1;
    while(carte!=joueurB->cartes[0] && carte!=joueurB->cartes[1]) scanf("%d",&carte);
    switch(carte){
        case 1:
            break;
        case 2:
            bleu_mort(joueurB,joueurR);
            break;
        case 3:
            bleu_renfort(joueurB,joueurR);
            break;
        case 4:
            bleu_anti_blindage(joueurB,joueurR);
            break;
        case 5:
            bleu_deplacement_multiple(joueurB,joueurR);
            break;
    }
    if(joueurB->cartes[0]==carte){
        joueurB->cartes[0]=0;
    }else{
        joueurB->cartes[1]=0;
    }
}

void bleu_renfort(joueur *joueur1, joueur *joueur2){// carte speciale bleue renfort
    int choix=0;

    do{
        printf("Tapez 1 si vous voulez rappeler deux contre torpilleur, tapez 2 si vous preferez rappeler un contre torpilleur blinde\n");
        while(choix!=1 && choix!=2) scanf("%d",&choix);
        if(choix==2){//Torpilleur blinde
            int id_pion_ressucite=0;
            if((joueur1->pions[0]).pv==0){
                id_pion_ressucite=0;
            }else if((joueur1->pions[1]).pv==0){
                id_pion_ressucite=1;
            }else{
                printf("Vous ne pouvez pas utiliser cette carte car vous n'avez aucun contre torpilleur blinde en reserve\n");
            }
            (joueur1->pions[id_pion_ressucite]).pv=2;
            selectionner_case(joueur1, joueur2, &(joueur1->pions[id_pion_ressucite].coord_x), &(joueur1->pions[id_pion_ressucite].coord_y),PERIPHERIE,1);
        }else if(choix==2){
            int mort=0;
            for(int i=2;i<19;i++){
                if((joueur1->pions[i]).pv==0){
                    mort++;
                }
            }
            if(mort<2){
                printf("Vous ne pouvez pas utiliser cette carte car vous n'avez aucun contre torpilleur detruit\n");
            }else{
                int pions_ressusciter=0;
                for(int i=2;i<19;i++){
                    if((joueur1->pions[i]).pv>0){
                        (joueur1->pions[i]).pv=1;
                        selectionner_case(joueur1, joueur2, &(joueur1->pions[i].coord_x), &(joueur1->pions[i].coord_y),PERIPHERIE,1);
                    }
                    if(pions_ressusciter>=2) break;
                }
            }
        }
    } while(choix!=1 && choix!=2);
    printf("Vous venez d'appeler du renfort, utilisez le a bon escient moussaillon.\n");
    afficherplateau(joueur1, joueur2);
    system("pause");
    system("cls");
}

void bleu_mort(joueur *joueurSel, joueur *joueur2){//carte mort subite bleue
    printf("joueur bleu vous utilisez votre carte mort subite\n");
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);
    int x=0;
    int y=0;
    do{//Selection du bateau a couler
        printf("Selectionnez le bateau à couler:\n");
        selectionner_case(joueurSel,joueur2,&x,&y,PLATEAU,0);
        system("pause");
    }while(plateau[x][y]==NULL || plateau[x][y]->type==CUIRASSE || plateau[x][y]->player==ATTAQUE);

    plateau[x][y]->pv=0;
    printf("Vous avez coule le bateau adverse\n");
    afficherplateau(joueurSel, joueur2);
    system("pause");
    system("cls");
}

void bleu_anti_blindage(joueur *joueurSel, joueur *joueur2){//Carte speciale bleu anti_blindage
    printf("Joueur bleu vous utilisez votre carte anti blindage\n");
    int id_pion=selectionner_pion(joueurSel, joueur2);
    joueurSel->pions[id_pion].anti_blindage=1;
    system("cls");
    afficherplateau(joueurSel,joueur2);
    system("pause");
    system("cls");
}

void bleu_deplacement_multiple(joueur *joueurSel, joueur *joueur2){//carte deplacement multiple bleu
    printf("joueur bleu vous utilisez votre carte deplacements multiples\n");
    int id_pion=selectionner_pion(joueurSel, joueur2);
    deplacer_pions(joueurSel, joueur2, id_pion);
}
