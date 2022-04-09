#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tangle.h"

Abr *tab[20]; //tableau des noeuds encore non confirmé

void add(Abr **arbre, int key){ //fonction ajouter dans le tangle
    #include <time.h>

    Abr *tangle = *arbre;

    Abr *new = malloc(sizeof(Abr)); //initialisation du noeud à ajouter
    new->key = key;
    new->dad1 = NULL;
    new->dad2 = NULL;
    new->son1 = NULL;
    new->son2 = NULL;
    new->fid = 0;
    new->state = 0;
    new->checkSon1 = 0;
    new->checkSon2 = 0;

    if(!tangle) { //cas ou le tangle n'admet aucun noeud
        tab[0] = new;
        *arbre = new;      
    }
    else if(tangle && countTabNode() == 0) printf("error, no unconfirmed node available"); //cas ou il n'y a aucun noeud non confirmé
    else if(countTabNode() == 1) { //cas ou il y a qu'un node dispo
        trierTab(); //mets le noeud tout à gauche du tableau
        new->dad1 = tab[0];
        if(!tab[0]->son1) tab[0]->son1 = new; //si l'enfant 1 n'est pas occupé
        else if(!tab[0]->son2) tab[0]->son2 = new; //si l'enfant 2 n'est pas occupé
        else printf("erreur d'enfants sur genesis"); 
        tab[0]->state = tab[0]->state + 0.5; //maj de l'état du noeud 
        if(tab[0]->state == 1){ //si le noeud a deux enfants
            tab[1] = tab[0]->son1; //ajoute l'enfant 1 dans tab 1
            tab[2] = tab[0]->son2; //ajoute l'enfant 2 dans tab 2
            removeTabNode(); //enleve le noeud tab 0
        }
    }
    else{
        srand(time(NULL));
        removeTabNode(); //enleve les noeuds deja confirmés et mets tous les noeuds à gauche
        int dad1 = rand()%countTabNode(); //choisis un pere aléatoire
        int dad2 = rand()%countTabNode(); //choisis un autre pere aléatoire
        if(dad1 == dad2) //si ce sont les memes
        do{
            dad2 = rand()%countTabNode();
        }while(dad1 == dad2);
        printf("dad1 = %d, dad2 = %d\n", dad1, dad2);

        new->dad1 = tab[dad1]; //le père du noeud new devient dad1
        new->dad2 = tab[dad2]; //le père 2 du noeud new devient dad2

        if(!tab[dad1]->son1) tab[dad1]->son1 = new; //si le dad1 n'admet pas un fils 1 
        else if(!tab[dad1]->son2) tab[dad1]->son2 = new; //si le dad1 n'admet pas un fils 2
        else printf("erreur d'enfants sur pere 1"); 
        if(tab[dad1]->son1 && !tab[dad2]->son1) tab[dad2]->son1 = new; //si le dad2 n'admet pas un fils 1
        else if(!tab[dad2]->son2) tab[dad2]->son2 = new; //si le dad2 n'admet pas un fils 2
        else printf("erreur d'enfants sur pere 2");

        tab[dad1]->state += 0.5; //maj des etat 
        tab[dad2]->state += 0.5;
        tab[countTabNode()] = new; //ajoute le nouveau noeud dans la liste des transactions non confirmées
    }

}

//fonctions sur le tableau
void initTab(){ //initialisation du tableau des noeuds non confirmées
    for (int i = 0; i <20; i++) tab[i] = NULL;
}

void affichTab(){  //fonction affichage du tableau
    printf("---Affichage du TAB---\n");
    for (int i = 0; i <20; i++) {
        if(tab[i]) printf("%d ", tab[i]->key);
        if(!tab[i]) printf("null ");
    }
    printf("\n");
}

int countTabNull(){ //compte le nombre de null dans le tableau des noeuds
    int n = 0;
    for (int i = 0; i <20; i++) if(!tab[i]) n++;
    return n;
}

int countTabNode(){ //compte le nombre de null dans le tableau des noeuds
    int n = 0;
    for (int i = 0; i <20; i++) if(tab[i]) n++;
    return n;
}

void trierTab(){ //mets tous les noeuds non nuls à gauche
    if(countTabNode()==0) return;

    int index = 0;
    for (int i = 0; i <20; i++){
        if(tab[i]) {
            Abr *tmp = tab[i];
            tab[i] = NULL;
            tab[index] = tmp;
            index++;
        }
    }
}

void removeTabNode(){ //enlève les noeuds avec l'état 1 
    if(countTabNode()==0) return;
    trierTab();
    
    for (int i = 0; i <countTabNode(); i++){
        if(tab[i]->state == 1.000000) tab[i] = NULL;
    }
    trierTab();
}

void testTab(){
    printf("son1 %d", tab[2]->son1->key);
    // Abr *tmp = malloc(sizeof(Abr));
    // tmp->key = 25;
    // Abr *tmp1 = malloc(sizeof(Abr));
    // tmp1->key = 95;
    // Abr *tmp2 = malloc(sizeof(Abr));
    // tmp2->key = 88;
    // Abr *tmp3 = malloc(sizeof(Abr));
    // tmp3->key = 23;

    // tab[5] = tmp;
    // tab[8] = tmp1;
    // tab[13] = tmp2;
    // tab[18] = tmp3;
}

//fonction d'affichage
void printDot(Abr *arbre, char *name){ //affichage du fichier .dot

    FILE* fichier = NULL;
    fichier = fopen(name,"w");
    fputs("digraph G{", fichier);
    fclose(fichier);

    dotRecursive(arbre,name);
    
    fichier = fopen(name,"a");
    fputs("\n}", fichier);
}

void dotRecursive(Abr *arbre, char *name){ //fonction recursive appelé dans le fonction printDot

    if(!arbre) return;

    FILE* fichier = NULL;
    fichier = fopen(name,"a");
    Abr *tmp = arbre;

    if(arbre->son1 && arbre->checkSon1!=1){
        tmp = arbre->son1;
        fputs("\n    ",fichier);
        fprintf(fichier, "%d", tmp->key);
        fputs(" -> ",fichier);
        fprintf(fichier, "%d", arbre->key);
        dotRecursive(arbre->son1, name);
        arbre->checkSon1 = 1;
    }

    if(arbre->son2 && arbre->checkSon2!=1){
        tmp = arbre->son2;
        fputs("\n    ",fichier);
        fprintf(fichier, "%d", tmp->key);
        fputs(" -> ",fichier);
        fprintf(fichier, "%d", arbre->key);
        dotRecursive(arbre->son2, name);
        arbre->checkSon2 = 1;
    }
}