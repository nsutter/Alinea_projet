#ifndef MATRICE_H
#define MATRICE_H

typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

typedef struct{char * nom; matrice * pointeur;} *mat;
typedef struct{char * nom; float val;} *flo;

typedef struct{mat * tab_mat; flo * tab_flo; int longueurm; int longueurf;} contexte;

#include "resolution.h"

float getElt(pmatrice m, int i, int j);
void setElt(pmatrice m, int i, int j, float valeur);

pmatrice nouvelleMatrice(int hauteur, int largeur);
pmatrice copieMatrice(pmatrice a);

pmatrice tabMatrice(int hauteur, int largeur, float * tab);

void afficheMatrice(pmatrice m);

void identite(pmatrice m); // m devient une matrice identité

pmatrice addition(pmatrice m1, pmatrice m2);
pmatrice soustraction(pmatrice m1, pmatrice m2);
float produit(pmatrice m1, pmatrice m2, int a, int b);
pmatrice multiplication(pmatrice m1, pmatrice m2);
pmatrice multiplication_scal(pmatrice M, float valeur);
pmatrice transposition(pmatrice M);
pmatrice expo(pmatrice M, int n);

void libereMatrice(pmatrice m);

void genereMatrice(pmatrice m);
void handlerSpeedtest();
void speedtest(int f, int debut, int fin, int pas, int s);

#endif
