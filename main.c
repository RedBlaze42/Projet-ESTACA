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
#define DEFENSE 1//Défini aussi numéro du joueur defenseur
#define ATTAQUE 2//Défini aussi numéro du joueur attaquant

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
    int num;//Joueur Rouge (Défenseur) = 1 Joueur Bleu (Attaquant) =2
	pion pions[20];
    int nb_pions;
	int cartes[2];
} joueur;

void color (int couleurDuTexte, int couleurDuFond);
/*
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
*/
void rougeCA(pion *pionattaque, pion *poinattaquant);

int main(){
    joueur joueurB;//Attaquant
    joueurB.num=2;
    joueurB.nb_pions=20;
    joueur joueurR;//Defenseur
    joueurR.num=1;
    joueurR.nb_pions=15;
    int gagnant;

    /*for(int i=0; i<20;i++){
        (joueurR.pions[i]).coord_x=12;
        (joueurR.pions[i]).coord_y=12;
        (joueurR.pions[i]).type=CUIRASSE;
        (joueurR.pions[i]).pv=2;
        (joueurR.pions[i]).player=1;
        (joueurR.pions[i]).invisible=0;
        (joueurR.pions[i]).anti_blindage=0;
        (joueurB.pions[i]).coord_x=12;
        (joueurB.pions[i]).coord_y=12;
        (joueurB.pions[i]).type=CUIRASSE;
        (joueurB.pions[i]).pv=2;
        (joueurB.pions[i]).player=1;
        (joueurB.pions[i]).invisible=0;
        (joueurB.pions[i]).anti_blindage=0;
    }*/

    lectureRegles();
    afficherplateau(&joueurB, &joueurR);
    placerpions( &joueurR, &joueurB);
    int vR,vB;//Victoire Bleue ou Rouge
    while(vR==0 && vB==0){
        tour(&joueurR,&joueurB);
        int vR=reste_pions_joueur(&joueurR, -1);
        int vB=reste_pions_joueur(&joueurB, -1);
        if(is_in_zone(ATTAQUE, &(joueurR.pions[1].coord_x), &(joueurR.pions[1].coord_y))==1){
           vR=1;
        }
        if(joueurR.pions[1].pv==0){
           vB=1;
        }
    }
    if(vR==1){
        printf("Bravo pour cette belle victoire,j'ai toujours su que le rouge etait la couleur des vaiqueurs.");
    }else{
        printf("Bravo pour cette belle victoire,j'ai toujours su que le bleu etait la couleur des vaiqueurs.");
    }
}

void tour(joueur *joueur1,joueur *joueur2){
    system("cls");
    if(joueur1->num==DEFENSE){
        printf("C'est au tour du joueur rouge\n\n");
    }else{
        printf("C'est au tour du joueur bleu\n\n");
    }
    int fin_menu=0;
    if(joueur1->num==DEFENSE){//Menu défenseur
        if(joueur1->cartes[0]!=0 || joueur1->cartes[1]!=0){
            printf("Voulez vous utiliser une carte ? o/n");
            char c=0;
            while(c!="o" || c!="n") scanf("%c",&c);
            if(c=='o'){
                //TODO Sous programme utiliser carte
            }
        }
        printf("Veuillez sélectionner une action:\n1. Déplacer votre cuirrassé\n2. Déplacer 2 contre-torpilleurs");
        int d=0;
        while(d!=1 || d!=2) scanf("%d",&d);

        if(d==2){
            int deplacements_restants=2;
            while(deplacements_restants>0){
                if(reste_pions_joueur(joueur1,CTORP)==0 || (reste_pions_joueur(joueur1,CTORP)==1 && deplacements_restants==1) ){//Soit le joueur n'as plus de CTORP soit il en a qu'un seul qu'il a déjà déplacé
                    printf("Vous n'avez plus de contre-torpilleurs");
                }
                //TODO Selectionner pion
            }
        }else{
            deplacer_pions(joueur1,joueur2,0);//Le cuirrassé est toujours au début du tableau
        }
        if(joueur_peut_attaquer(joueur1,joueur2)==1){
            printf("Voulez vous attaquer ? o/n");
            char c=0;
            while(c!="o" || c!="n") scanf("%c",&c);
            if(c=="o") selectionner_attaque(joueur1,joueur2);
        }
        printf("Fin du tour defenseur");
        system("pause");

    }else{
        if(joueur2->cartes[0]!=0 || joueur2->cartes[1]!=0){
            printf("Voulez vous utiliser une carte ? o/n");
            char c=0;
            while(c!="o" || c!="n") scanf("%c",&c);
            if(c=='o'){
                //TODO Sous programme utiliser carte
            }
        }
        int choix_effectue=0;
        int d;
        if(joueur_peut_attaquer(joueur2,joueur1)==1){
            printf("Veuillez sélectionner une action:\n1. Attaquer un ennemi\n2. Déplacer 2 contre-torpilleurs");
            d=0;
            while(d!=1 || d!=2) scanf("%d",&d);
        }else{//Le joueur ne peut pas attaque donc il doit déplacer 2 pions
            d=2;
        }

        if(d==1){
            selectionner_attaque(joueur2,joueur1);
        }else if(d==2){
            int deplacements_restants=2;
            while(deplacements_restants>0){
                if(reste_pions_joueur(joueur2,CTORP)==0 || (reste_pions_joueur(joueur2,CTORP)==1 && deplacements_restants==1) ){//Soit le joueur n'as plus de CTORP soit il en a qu'un seul qu'il a déjà déplacé
                    printf("Vous n'avez plus de contre-torpilleurs");
                }
                //TODO Selectionner pion
            }
        }else{
            deplacer_pions(joueur1,joueur2,0);//Le cuirrassé est toujours au début du tableau
        }
        if(joueur_peut_attaquer(joueur1,joueur2)==1){
            printf("Voulez vous attaquer ? o/n");
            char c=0;
            while(c!="o" || c!="n") scanf("%c",&c);
            if(c=="o") selectionner_attaque(joueur1,joueur2);
        }
        printf("Fin du tour defenseur");
        system("pause"); 
    }

    system("cls");
    
}


int reste_pions_joueur(joueur *joueurSel,int type){//Si type=-1, on vérifie juste si il reste des pions au joueur
    for(int i;i<joueurSel->nb_pions;i++){
        if( (joueurSel->pions[i].type==type || type==-1) && joueurSel->pions[i].pv>0){
            return 1;
        }
    }
}

void deplacer_pions(joueur *joueurSel, joueur *joueur2, int id_pion_sel){
    int en_mouvement=1;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entree
        int x=0;
        int y=0;
        pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
        remplirTab(plateau, joueurSel, joueur2);//Afficher plateau surbrillance
        int direction_prise;
        switch(getch()) {
            case enter:
                joueurSel->pions[id_pion_sel].coord_x=x;
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
int joueur_peut_attaquer(joueur *joueurSel, joueur *joueur2){
    for(int i=0;i<joueurSel->nb_pions;i++){
        if(pion_peut_attaquer(joueurSel, joueur2, &(joueurSel->pions[i]))==1){
            return 1;
        }
    }
    return 0;
}

int pion_peut_attaquer(joueur *joueurSel, joueur *joueur2, pion *pionSel){//Retourne 
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

/*Pour verifier si un joueur peut attaquer
int peut_attaquer=0;
for(int i=0;i<joueurSel->nb_pions;i++){
    if(pion_peut_attaquer(joueurSel, joueur2, joueurSel->pions[i])==1){
        peut_attaquer=1;
    }
}
if(peut_attaquer==0){
    printf("Aucun de vos pions ne peut attaquer\n");
}   
*/

void selectionner_attaque(joueur *joueurSel, joueur *joueur2){//Permet au joueur de selectionner le pion qui attaque et la case a attaquer
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
    remplirTab(plateau, joueurSel, joueur2);
    
    int i=0;
    int fin_boucle=0;
    while(fin_boucle!=0){
        while(pion_peut_attaquer(joueurSel, joueur2, &(joueurSel->pions[i]))!=1){//Cherche le prochain pion en position d'attaquer
            i++;
            if(i>=joueurSel->nb_pions) i=0;
        }
        afficherplateau_sel(joueurSel, joueur2, joueurSel->pions[i].coord_x, joueurSel->pions[i].coord_y);
        if(getch()==enter){//Pion selectionne
            int direction_attaque=0;//Pour cycler les diagonales
            pion *pionSel=&(joueurSel->pions[i]);
            int y = pionSel->coord_y;
            int x = pionSel->coord_x;
            int y_attaque=y-1;
            int x_attaque=x-1;
            int direction_valide;
            while(fin_boucle!=0){
                direction_valide=0;
                while(direction_valide==0){
                    if(direction_attaque==haut_gauche && (y-1>=0 && x-1>=0 && plateau[y-1][x-1]->player!=pionSel->player && plateau[y-1][x-1]->player!=0)==0 ){//Haut Gauche
                        direction_attaque++;
                    }else if(direction_attaque==haut_droit && (y-1>=0 && x+1<DIM_PLATEAU && plateau[y-1][x+1]->player!=pionSel->player && plateau[y-1][x+1]->player!=0)==0){//Haut Droite
                        direction_attaque++;
                    }else if(direction_attaque==bas_gauche && (y+1<DIM_PLATEAU && x-1>=0 && plateau[y+1][x-1]->player!=pionSel->player && plateau[y+1][x-1]->player!=0)==0){//Bas Gauche
                        direction_attaque++;
                    }else if(direction_attaque==bas_droit && (y+1<DIM_PLATEAU && x+1<DIM_PLATEAU && plateau[y+1][x+1]->player!=pionSel->player && plateau[y+1][x+1]->player!=0)==0){//Bas Droite
                        direction_attaque++;
                    }else{
                        direction_valide=1;//Pas eu besoin d'incrementer la direction donc elle est valide
                    }
                    if(direction_attaque>=4) direction_attaque=0;
                }
                switch(direction_attaque){
                    case haut_droit:
                        y_attaque=y-1;
                        x_attaque=x-1;
                        break;
                    case haut_gauche:
                        y_attaque=y-1;
                        x_attaque=x+1;
                        break;
                    case bas_droit:
                        y_attaque=y+1;
                        x_attaque=x-1;
                        break;
                    case bas_gauche:
                        y_attaque=y+1;
                        x_attaque=x+1;
                        break;
                }

                afficherplateau_sel(joueurSel, joueur2,x_attaque,y_attaque);
                if(getch()==enter){//Pion a attaquer selectionne
                    //TODO ATTAQUE
                    pion *victime=plateau[y_attaque][x_attaque];
                    victime->pv--;
                    pionSel->coord_x=x_attaque;
                    pionSel->coord_y=y_attaque;
                    //TODO Rajouter le check pour les pieges en dessous du bateau coule (sous programme ?)
                }else{
                    direction_attaque++;
                }
            }
            
        }
    }
}

void selectionner_case(joueur *joueurSel, joueur *joueur2, int *sel_x, int *sel_y, int zone, int vide){
    int en_mouvement=1;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entree
        pion plateau[DIM_PLATEAU][DIM_PLATEAU];
        int x=0;
        int y=0;
        afficherplateau_sel(joueurSel, joueur2,x,y);
        int direcion_prise;
        switch(getch()) {
            case enter:
                if( (is_in_zone(zone,x,y)==1) && (vide==1 && is_pionSurCase(joueurSel,joueur2,x,y)==0) ){
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

int selectionner_pion(joueur *joueurSel, joueur *joueur2){
    int en_selection=1;
    int id_pion=0;
    do{
        afficherplateau_sel(joueurSel, joueur2, joueurSel->pions[id_pion].coord_x, joueurSel->pions[id_pion].coord_y);
        if(id_pion>joueurSel->nb_pions) id_pion=0;
    }while(getch()!=enter);
    return id_pion;
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
    for(int j=0; j<DIM_PLATEAU; j++){
        for(int i=0; i<DIM_PLATEAU; i++){
            tab[i][j]=NULL;
        }
    }
    
    for(int i=0;i<joueur1->nb_pions;i++){
        if(joueur1->pions[i].pv>0){
            if(tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y]!=NULL){//TODO Plus joli en utilisant un pointeur vers tab[...][...]
                if((tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y])->type=PIEGE){
                    tab[(joueur1->pions[i]).coord_x][(joueur1->pions[i]).coord_y]=&(joueur1->pions[i]);
                }
            }
        }
    }

     for(int i=0;i<joueur2->nb_pions;i++){
        if(joueur2->pions[i].pv>0){
            if(tab[joueur2->pions[i].coord_x][joueur2->pions[i].coord_y]!=NULL){
                if(tab[joueur2->pions[i].coord_x][joueur2->pions[i].coord_y]->type=PIEGE){
                    tab[(joueur2->pions[i]).coord_x][(joueur2->pions[i]).coord_y]=&(joueur2->pions[i]);
                }
            }
        }
    }
}

void afficher_pion(pion* pionAff){//pour afficher une case
    switch((*pionAff).type){
        case CTORP : 
            if((*pionAff).player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            printf("1");
            break;
        case CUIRASSE :
            if((*pionAff).player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            printf("2");
            break;
        case PIEGE :
            if((*pionAff).player==1){
                color(JOUEUR1,15);
            }else{
                color(JOUEUR2,15);
            }
            printf("3");
            break;
        default:
            color(0,15);
            printf(" ");
    }
    color(15,0);
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
        if( (x>2 && x<8 && y>2 && y<8)==0 ){//verifier
            return 1;
        }else{
            return 0;
        }
    }
}

void afficherplateau_sel(joueur *joueurAff,joueur *joueur2, int x, int y){//on affiche le plateau avec une case en surbrillance
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];//TODO Changer l'apparence en fonction des pv du joueur
    remplirTab(plateau, joueurAff, joueur2);
    
    for(int j=0;j<DIM_PLATEAU;j++){
        for(int i=0;i<DIM_PLATEAU;i++){
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
    
    for(int j=0;j<DIM_PLATEAU;j++){
        for(int i=0;i<DIM_PLATEAU;i++){
            if(plateau[i][j]==NULL){
                color(0,15);
                printf("-");
                color(15,0);
            }else if((*plateau[i][j]).invisible==0){
                afficher_pion(plateau[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

void lectureRegles(){
    int c;
    while(c!=5){
        system("CLS");
        printf("Bienvenu sur seawars.\nLe but du jeu est de couler tous les bateaux de l'adversaire,\n ou pour le joueur rouge de sortir le cuirassé de la zone de defense ou\n pour le joueur bleu de couler le cuirassé adverse.\n\nAu debut de la partie, le premier joueur dispose le cuirasse dans la case centrale marquee d'un rond,\npuis il place ses douze contre-torpilleurs a sa convenance, a l'interieur du carre central de vingt-cinq cases.\nLorsque le premier joueur a fini de placer ses pions, c'est au tour du deuxieme joueur\nde disposer ses vingt contretorpilleurs comme il en a envie, mais a l'exterieur du carre central.\nUne fois termine le placement de tous les pions, chaque joueur pioche deux cartes speciales de la couleur correspondante.\nLe rouge a le pouvoir de decider a qui revient de jouer le premier coup, et la partie peut alors commencer.\n "); 
        do{
            printf("Pour voir les deplacement/attaques des pions tapez 1\nPour voir la nature des pionstapez 2\nPour voir les capacitees des cartes speciales bleues tapez 3\nPour voir les capacitees des cartes speciales rouges tapez 4\nPour passer directement au debut de la partie tapez 5\n");
            input("%d",&c);
        }while(c<1 || c>5);
        switch(c){//EMILES Faire une while, il faut pouvoir afficher plusieurs fois les règles
            case '1':
                printf("Deplacments:\nToutes les pieces se deplacent en ligne orthogonale et peuvent parcourir des rangees entieres de cases vides\nen s'arrêtant où bon leur semble,mais elles ne peuvent pas sauter par-dessus une case occupee.\n Elles ne peuvent pas eliminer un pion (de leur camp ou du camp adverse) qui bloque leur avancee,\n mais elles doivent s'arrêter sur une case attenante \n\nAttaques:\nLes pieces s’attaquent mutuellement entre cases voisines directe,\nen se deplaçant en diagonale dans n'importe quelle direction.\nLorsqu'un un navire detruit un autre navire, il enleve du damier le pion elimine et prend sa place.\nAttaquer un pion menace n'est pas une obligation.");
            break;
            case '2':
                printf("Il existe 4 types de pions:\n- Les cuirasses\n- Les contretorpilleurs blindes(pions moyens, doivent être attaque deux fois pour être detruits,\nattaques une fois, ils perdent leurs blindages et deviennent des contre-torpilleurs normaux.)\n- Les contre torpilleurs\n- Les cartes pieges(voir cartes piege rouges et bleues)\n\n\nLe joueur rouge:\n- Un cuirasse(le grand pion),\n- Deux contretorpilleurs blindes\n- Dix contre-torpilleurs(pions normaux).\n\nSon adversaire, le joueur bleu:\n- Deux contretorpilleurs blindes\n- Dix-huit contre-torpilleurs\n");
            break;
            case '3':
                printf("1. Carte deplacement libre: Cette carte peut être activee juste avant un deplacement\net permettra a un pion de se deplacer en diagonale durant un tour.\n\n2. Carte mort subite : Cette carte peut être utilisee au debut ou a la fin d’un tout,\nelle permet d’attaquer n’importe quel navire ennemi (excepte le cuirasse).\n\n3. Carte renfort : Cette carte peut être utilisee au debut ou a la fin d’un tout,\nelle permet de rappeler deux contre-torpilleurs detruits ou un blinde sur le plateau.\nLes navires rappeles doivent être deposer forcement sur la peripherie du plateau. \n\n4. Carte Anti-blindage : Cette carte peut être activee au debut d’un tour, elle permet de rendre,\ndurant toute la partie, un contre-torpilleur capable de detruire un blinde en un seul coup.\n\n5. Carte deplacement multiple : Cette carte peut être activee au debut d’un tour,\nelle permet d’effectuer trois deplacements au lieu de deux.\n");
            break;
            case '4':
                printf("1. Carte piege : Cette carte peut être activee au debut ou a la fin d’un tour et\nelle permet de placer deux pieges sur la carte. Ces piges sont invisibles pour l’adversaire.\nSi lors d’un deplacement un pion adverse passe par un piege,\nle pion sera detruit et le piege desactive. Si c’est un blinde qui passe par le piege, il perdra son blindage.\n\n2. Carte bouclier : Cette carte peut être activee au debut ou a la fin d’un tour et\nelle permet de rajouter un blindage supplementaire a n’importe quel navire.\n\n3. Carte contre-attaque : Cette carte peut être activee lors qu’on un pion rouge est attaque.\nLe pion attaque ne subit pas de degâts et c’est l’attaquant qui subit les degâts a sa place.\n\n4. Carte permutation : Cette carte peut être activee au debut ou a la fin d’un tour et\nelle permet a deux pions (rouges ou bleus) d’echanger leurs positons sur le plateau.\n\n 5. Carte deplacement furtif : Cette carte peut être activee juste avant un deplacement.\nLe pion deplace devient invisible pour l’adversaire durant le tour suivant.\nSi lors d’un deplacement d’un pion bleu, il croise sur son chemin le navire invisible,\ncelui-ci sera decouvert et le pion bleu devra, bien sûr, s’arrêter.\n");
                break;
            case '5':
                printf("Ok let's go\n");
        }
    }
}

void color (int couleurDuTexte, int couleurDuFond){// permet de gerer les couleurs
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDuFond*16+couleurDuTexte);
}

/*
void placerpions(joueur *joueurR, joueur *joueurB){
    printf("Au joueur rouge de placer ses pions\n");
    (*joueurR->pions[1].type)=2;//EMILES Corriger tout les (*joueurR->pions[1]) par des joueurR->pions[1]
    (*joueurR->pions[1].coord_x)=6;
    (*joueurR->pions[1].coord_y)=6;
    do{
        afficherplateau( joueurR, joueurB);//EMILES Pas besoin d'afficher le plateau car la fonction séléctionner case l'affiche déjà
        selectionner_case(*joueurR, *joueurB, (*joueurR->pions[1].coord_x),(*joueurR->pions[1].coord_y), 0, DEFENSE);
        deplacer_pions(*joueurR,*joueurB, int id_pion_sel);//EMILES Pas utiliser cette fonction mais plutôt modifier direct les coordonnées des pions
    }while((*joueurR->pions[1].coord_x)<4 || (*joueurR->pions[1].coord_x)>7 || (*joueurR->pions[1].coord_y)<4 || (*joueurR->pions[1].coord_y)>7);
    int i;
    for(i=1,i<3,i++){
        (*joueurR->pions[i].type)=1;
        (*joueurR->pions[i].pv)=2;
        int x , y, f;
        do{
            for(y=3,y<8,y++){//EMILES Pas besoin de ça
                for(x=3,y<8,y++){
                    if(is_pionSurCase(*joueurR,*joueurB, x, y)==0){
                        (*joueurR->pions[i].coord_x)=x;
                        (*joueurR->pions[i].coord_y)=y;
                        f=0;
                    }else{
                        f=1;
                    }
                }
            }
        }while(f!=0)
        
        do{
            afficherplateau( *joueurR, *joueurB);
            selectionner_case(*joueurR, *joueurB, (*joueurR->pions[i].coord_x),(*joueurR->pions[i].coord_y), 0, DEFENSE);
            deplacer_pions(*joueurR,*joueurB, int id_pion_sel);
        }while((*joueurR->pions[i].coord_x)<4 || (*joueurR->pions[i].coord_x)>7 || (*joueurR->pions[i].coord_y)<4 || (*joueurR->pions[i].coord_y)>7);
    }

    for(i=3,i<13,i++){
        (*joueurR->pions[i].type)=1;
        (*joueurR->pions[i].pv)=1;
        int x , y, f;
        do{
            for(y=3,y<8,y++){
                for(x=3,y<8,y++){
                    if(is_pionSurCase(*joueurR,*joueurB, x, y)==0){
                        (*joueurR->pions[i].coord_x)=x;
                        (*joueurR->pions[i].coord_y)=y;
                        f=0;
                    }else{
                        f=1;
                    }
                }
            }
        }while(f!=0)
        
        do{
            afficherplateau( *joueurR, *joueurB);
            selectionner_case(*joueurR, *joueurB, (*joueurR->pions[i].coord_x),(*joueurR->pions[i].coord_y), 0, DEFENSE);
            deplacer_pions(*joueurR,*joueurB, int id_pion_sel);
        }while((*joueurR->pions[i].coord_x)<4 || (*joueurR->pions[i].coord_x)>7 || (*joueurR->pions[i].coord_y)<4 || (*joueurR->pions[i].coord_y)>7);
    }

    printf("Au tour du joueur bleu de placer ses pions\n");
    int i;
    for(i=0,i<2,i++){
        (*joueurB->pions[i].type)=1;
        (*joueurB->pions[i].pv)=2;
        int x , y, f;
        do{
            for(y=,y<8,y++){
                for(x=3,y<8,y++){
                    if(is_pionSurCase(*joueurR,*joueurB, x, y)==0){
                        (*joueurB->pions[i].coord_x)=x;
                        (*joueurB->pions[i].coord_y)=y;
                        f=0;
                    }else{
                        f=1;
                    }
                }
            }
        }while(f!=0)
        
        do{
            afficherplateau( *joueurR, *joueurB);
            selectionner_case(*joueurR, *joueurB, (*joueurB->pions[i].coord_x),(*joueurB->pions[i].coord_y), 0, DEFENSE);
            deplacer_pions(*joueurR,*joueurB, int id_pion_sel);
        }while((*joueurB->pions[i].coord_x)<4 || (*joueurB->pions[i].coord_x)>7 || (*joueurB->pions[i].coord_y)<4 || (*joueurB->pions[i].coord_y)>7);
    }

    for(i=3,i<13,i++){
        (*joueurB->pions[i].type)=1;
        (*joueurB->pions[i].pv)=1;
        int x , y, f;
        do{
            for(y=3,y<8,y++){
                for(x=3,y<8,y++){
                    if(is_pionSurCase(*joueurR,*joueurB, x, y)==0){
                        (*joueurB->pions[i].coord_x)=x;
                        (*joueurB->pions[i].coord_y)=y;
                        f=0;
                    }else{
                        f=1;
                    }
                }
            }
        }while(f!=0)
        
        do{
            afficherplateau( *joueurR, *joueurB);
            selectionner_case(*joueurR, *joueurB, (*joueurB->pions[i].coord_x),(*joueurB->pions[i].coord_y), 0, DEFENSE);
            deplacer_pions(*joueurR,*joueurB, int id_pion_sel);
        }while((*joueurB->pions[i].coord_x)<4 || (*joueurB->pions[i].coord_x)>7 || (*joueurB->pions[i].coord_y)<4 || (*joueurB->pions[i].coord_y)>7);
    }



}*/

void placerpions(joueur *joueurR, joueur *joueurB){
    printf("Au joueur rouge de placer ses pions\n");
    joueurR->pions[1].type=2;
    joueurR->pions[1].coord_x=0;
    joueurR->pions[1].coord_y=0;
    int i;
    for(i=1;i<3;i++){
        joueurR->pions[i].type=1;
        joueurR->pions[i].pv=2;
        joueurR->pions[i].coord_x=0;
        joueurR->pions[i].coord_y=0;
    }
    for(i=3;i<13;i++){
        joueurR->pions[i].type=1;
        joueurR->pions[i].pv=1;
        joueurR->pions[i].coord_x=0;
        joueurR->pions[i].coord_y=0;
    }
    for(i=0;i<13;i++){
        selectionner_case(joueurR,joueurB , &(joueurR->pions[i].coord_x), &(joueurR->pions[i].coord_y), DEFENSE, 1);
    }
    printf("Au joueur bleu de placer ses pions\n");
    for(i=0;i<2;i++){
        joueurR->pions[i].type=1;
        joueurR->pions[i].pv=2;
        joueurR->pions[i].coord_x=0;
        joueurR->pions[i].coord_y=0;
    }
    for(i=2;i<19;i++){
        joueurR->pions[i].type=1;
        joueurR->pions[i].pv=1;
        joueurR->pions[i].coord_x=0;
        joueurR->pions[i].coord_y=0;
    }
    for(i=0;i<19;i++){
        selectionner_case(joueurR,joueurB , &(joueurR->pions[i].coord_x), &(joueurR->pions[i].coord_y), DEFENSE, 1);
    }
}

void rougeCA(pion *pionattaque, pion *pionattaquant){
    pionattaque->pv=pionattaque->pv+1;
    pionattaquant->pv=pionattaquant->pv-1;
}
void rougepermut(pion *pion1, pion *pion2){
    int x1=pion1->coord_x;
    int y1=pion1->coord_y;
    int x2=pion2->coord_x;
    int y2=pion2->coord_y;
    pion1->coord_x=x2;
    pion1->coord_y=y2;
    pion2->coord_x=x1;
    pion2->coord_y=y1;
}