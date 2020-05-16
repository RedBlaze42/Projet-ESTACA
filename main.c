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
    int nb_pions;
	int cartes[2];
} joueur;


void color (int couleurDuTexte, int couleurDuFond);

int main(){
    pion *plateau[DIM_PLATEAU][DIM_PLATEAU];
}

void deplacer_pions(joueur *joueurSel, joueur *joueur2, int id_pion_sel){
    int en_mouvement=1;
    while(en_mouvement){//Attendre que le joueur appuie sur la touche entrée
        int x=0;
        int y=0;
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

/*Pour vérifier si un joueur peut attaquer
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

void selectionner_attaque(joueur *joueurSel, joueur *joueur2){//Permet au joueur de sélectionner le pion qui attaque et la case à attaquer
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
        if(getch()==enter){//Pion sélectionné
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
                        direction_valide=1;//Pas eu besoin d'incrémenter la direction donc elle est valide
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
                if(getch()==enter){//Pion à attaquer sélectionné
                    //TODO ATTAQUE
                    pion *victime=plateau[y_attaque][x_attaque];
                    victime->pv--;
                    pionSel->coord_x=x_attaque;
                    pionSel->coord_y=y_attaque;
                    //TODO Rajouter le check pour les pièges en dessous du bateau coulé (sous programme ?)
                }else{
                    direction_attaque++;
                }
            }
            
        }
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
    for(int j=0; j<DIM_PLATEAU; j++){
        for(int i=0; i<DIM_PLATEAU; i++){
            tab[i][j]=NULL;
        }
    }
    
    for(int i=0;i<joueur1->nb_pions;i++){
        if(joueur1->pions[i].pv>0){
            if(tab[joueur1->pions[i].coord_x][joueur1->pions[i].coord_y]!=NULL)
            tab[(joueur1->pions[i]).coord_x][(joueur1->pions[i]).coord_y]=&(joueur1->pions[i]);
        }
    };

     for(int i=0;i<joueur2->nb_pions;i++){
        if(joueur2->pions[i].pv>0){
            tab[(joueur2->pions[i]).coord_x][(joueur2->pions[i]).coord_y]=&(joueur2->pions[i]);
        }
    }
}

void afficher_pion(pion* pionAff){//pour afficher une case
    switch((*pionAff).type){//TODO Remplacer par les define
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
                color(15,15);
                printf(" ");
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
    printf("Bienvenu sur seawars./n
    le but du jeu est de manger tous les pions de l'adversaire/n 
    
    Au début de la partie, le premier joueur dispose le cuirassé dans la case centrale marquée d'un rond,/n
    puis il place ses douze contre-torpilleurs à sa convenance, à l'intérieur du carré central de vingt-cinq cases./n
    Lorsque le premier joueur a fini de placer ses pions, c'est au tour du deuxième joueur/n
    de disposer ses vingt contretorpilleurs comme il en a envie, mais à l'extérieur du carré central./n
    Une fois terminé le placement de tous les pions, chaque joueur pioche deux cartes spéciales de la couleur correspondante./n
    Le rouge a le pouvoir de décider à qui revient de jouer le premier coup, et la partie peut alors commencer./n "); 
    do{
        printf("Pour voir les déplacement/attaques des pions tapez 1/n
        Pour voir la nature des pionstapez 2/n
        Pour voir les capacitées des cartes spéciales bleues tapez 3/n
        Pour voir les capacitées des cartes spéciales rouges tapez 4/n
        Pour passer directement au début de la partie tapez 5/n");
        c = getchar();
        }while(c<1 || c>5);
    if(c != '\n' && c != EOF)
      {
         int d;
         while((d = getchar()) != '\n' && d != EOF);
      }
 
    switch(c){
        case '1':
            printf("Déplacments:
            Toutes les pièces se déplacent en ligne orthogonale et peuvent parcourir des rangées entières de cases vides/n
            en s'arrêtant où bon leur semble,mais elles ne peuvent pas sauter par-dessus une case occupée./n 
            Elles ne peuvent pas éliminer un pion (de leur camp ou du camp adverse) qui bloque leur avancée,/n 
            mais elles doivent s'arrêter sur une case attenante /n

            Attaques:
            Les pièces s’attaquent mutuellement entre cases voisines directe,/N
            en se déplaçant en diagonale dans n'importe quelle direction./n
            Lorsqu'un un navire détruit un autre navire, il enlève du damier le pion éliminé et prend sa place./n
            Attaquer un pion menacé n'est pas une obligation.");
        break;
        case '2':
            printf("Il existe 4 types de pions:
            - Les cuirassés()/n
            - Les contretorpilleurs blindés(pions moyens, doivent être attaqué deux fois pour être détruits,/n
                attaqués une fois, ils perdent leurs blindages et deviennent des contre-torpilleurs normaux.)/n
            - Les contre torpilleurs()/n
            - Les cartes pièges(voir cartes piège rouges et bleues)/n
            
                
                Le joueur rouge:/n
            - Un cuirassé(le grand pion),/n
            - Deux contretorpilleurs blindés(pions moyens, doivent être attaqué deux fois pour être détruits,/n
                attaqués une fois, ils perdent leurs blindages et deviennent des contre-torpilleurs normaux.)/n
            - Dix contre-torpilleurs(pions normaux)./n

            Son adversaire, le joueur bleu:/n
            - Deux contretorpilleurs blindés/n
            - Dix-huit contre-torpilleurs/n");
        break;
        case '3':
            printf("1. Carte déplacement libre: Cette carte peut être activée juste avant un déplacement/n
            et permettra à un pion de se déplacer en diagonale durant un tour./n
 
            2. Carte mort subite : Cette carte peut être utilisée au début ou à la fin d’un tout,/n
            elle permet d’attaquer n’importe quel navire ennemi (excepté le cuirassé)./n 
            
            3. Carte renfort : Cette carte peut être utilisée au début ou à la fin d’un tout,/n
            elle permet de rappeler deux contre-torpilleurs détruits ou un blindé sur le plateau./n
            Les navires rappelés doivent être déposer forcément sur la périphérie du plateau. /n 
            
            4. Carte Anti-blindage : Cette carte peut être activée au début d’un tour, elle permet de rendre,/n
            durant toute la partie, un contre-torpilleur capable de détruire un blindé en un seul coup./n  
            
            5. Carte déplacement multiple : Cette carte peut être activée au début d’un tour,/n
            elle permet d’effectuer trois déplacements au lieu de deux./n");
        break;
        case '4':
            printf("1. Carte piège : Cette carte peut être activée au début ou à la fin d’un tour et/n
            elle permet de placer deux pièges sur la carte. Ces piges sont invisibles pour l’adversaire./n
            Si lors d’un déplacement un pion adverse passe par un piège,/n
            le pion sera détruit et le piège désactivé. Si c’est un blindé qui passe par le piège, il perdra son blindage./n 
    
            2. Carte bouclier : Cette carte peut être activée au début ou à la fin d’un tour et/n
            elle permet de rajouter un blindage supplémentaire à n’importe quel navire./n 
            
            3. Carte contre-attaque : Cette carte peut être activée lors qu’on un pion rouge est attaqué./n
            Le pion attaqué ne subit pas de dégâts et c’est l’attaquant qui subit les dégâts à sa place./n  
            
            4. Carte permutation : Cette carte peut être activée au début ou à la fin d’un tour et/n
            elle permet à deux pions (rouges ou bleus) d’échanger leurs positons sur le plateau./n  
            
            
            5. Carte déplacement furtif : Cette carte peut être activée juste avant un déplacement./n
            Le pion déplacé devient invisible pour l’adversaire durant le tour suivant./n
            Si lors d’un déplacement d’un pion bleu, il croise sur son chemin le navire invisible,/n
            celui-ci sera découvert et le pion bleu devra, bien sûr, s’arrêter./n");
            break;
        case '5':
        printf("C'est parti");
    }
}

void color (int couleurDuTexte, int couleurDuFond){// permet de gérer les couleurs
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDuFond*16+couleurDuTexte);
}