#ifndef ___BALISE___
#define ___BALISE___

typedef struct arbre {
    int key;
    int deseq;
    int fid;
    float state; //transaction confirmée : 1 transaction non confirmée : 0
    struct arbre* dad1;
    struct arbre* dad2;
    struct arbre* son1;
    struct arbre* son2;
    int checkSon1;
    int checkSon2;
} Abr;

void add(Abr **arbre, int key);

//fonctions sur la tableau
void initTab();
void affichTab();
int countTabNull();
int countTabNode();
void trierTab();
void removeTabNode();
void testTab();

//fonction affichage
void printDot(Abr *arbre, char *name);
void dotRecursive(Abr *arbre, char *name);


#endif