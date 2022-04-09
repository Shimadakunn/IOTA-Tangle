#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "tangle.h"

Abr *tab[20];

void add(Abr **arbre, int key){
    #include <time.h>

    Abr *tangle = *arbre;

    Abr *new = malloc(sizeof(Abr));
    new->key = key;
    new->dad1 = NULL;
    new->dad2 = NULL;
    new->son1 = NULL;
    new->son2 = NULL;
    new->fid = 0;
    new->state = 0;
    new->checkSon1 = 0;
    new->checkSon2 = 0;

    if(!tangle) { //initialisation
        tab[0] = new;
        *arbre = new;

        
    }
    else if(tangle && countTabNode() == 0) printf("error, no unconfirmed node available"); //cas ou il n'y a aucun noeud dispo
    else if(countTabNode() == 1) { //cas ou il y a qu'un node dispo
        trierTab();
        new->dad1 = tab[0];
        if(!tab[0]->son1) tab[0]->son1 = new;
        else if(!tab[0]->son2) tab[0]->son2 = new;
        else printf("erreur d'enfants sur genesis");
        tab[0]->state = tab[0]->state + 0.5;
        if(tab[0]->state == 1){
            tab[1] = tab[0]->son1;
            tab[2] = tab[0]->son2;
            removeTabNode();
        }
    }
    else{
        srand(time(NULL));
        removeTabNode(); //enleve les noeuds deja confirmés et mets tous les noeuds à gauche
        int dad1 = rand()%countTabNode();
        int dad2 = rand()%countTabNode();
        if(dad1 == dad2) 
        do{
            dad2 = rand()%countTabNode();
        }while(dad1 == dad2);
        printf("dad1 = %d, dad2 = %d\n", dad1, dad2);

        new->dad1 = tab[dad1]; //les deux pères
        new->dad2 = tab[dad2];

        if(!tab[dad1]->son1) tab[dad1]->son1 = new;
        else if(!tab[dad1]->son2) tab[dad1]->son2 = new;
        else printf("erreur d'enfants sur pere 1");
        if(tab[dad1]->son1 && !tab[dad2]->son1) tab[dad2]->son1 = new;
        else if(!tab[dad2]->son2) tab[dad2]->son2 = new;
        else printf("erreur d'enfants sur pere 2");

        tab[dad1]->state += 0.5;
        tab[dad2]->state += 0.5;
        tab[countTabNode()] = new; //ajoute le nouveau noeud dans la liste des transactions non confirmés
    }

}

//fonctions sur le tableau
void initTab(){
    for (int i = 0; i <20; i++) tab[i] = NULL;
}

void affichTab(){
    printf("---Affichage du TAB---\n");
    for (int i = 0; i <20; i++) {
        if(tab[i]) printf("%d ", tab[i]->key);
        if(!tab[i]) printf("null ");
    }
    printf("\n");
}

int countTabNull(){
    int n = 0;
    for (int i = 0; i <20; i++) if(!tab[i]) n++;
    return n;
}

int countTabNode(){
    int n = 0;
    for (int i = 0; i <20; i++) if(tab[i]) n++;
    return n;
}

void trierTab(){
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

void removeTabNode(){
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
void printDot(Abr *arbre, char *name){

    FILE* fichier = NULL;
    fichier = fopen(name,"w");
    fputs("digraph G{", fichier);
    fclose(fichier);

    dotRecursive(arbre,name);
    
    fichier = fopen(name,"a");
    fputs("\n}", fichier);
}

void dotRecursive(Abr *arbre, char *name){

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