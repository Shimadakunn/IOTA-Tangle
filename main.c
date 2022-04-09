#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tangle.h"

int main(){

    initTab();

    Abr *tangle = NULL;

    add(&tangle, 1);
    affichTab();
    add(&tangle,2);
    affichTab();
    add(&tangle,3);
    affichTab();

    add(&tangle,4);
    affichTab();
    add(&tangle,5);
    affichTab();
    add(&tangle,6);
    affichTab();
    add(&tangle,7);
    affichTab();
    add(&tangle,8);
    affichTab();
    add(&tangle,9);
    affichTab();
    add(&tangle,10);
    removeTabNode();
    affichTab();

    
    printDot(tangle,"./Fichiers .dot/arbre.dot");
    
}