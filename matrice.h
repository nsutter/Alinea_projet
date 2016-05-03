#ifndef MATRICE_H
#define MATRICE_H

typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

typedef struct{char * nom; matrice * pointeur;} *donnee_mat;
typedef struct{char * nom; float * pointeur;} *donnee_flo;

typedef struct{donnee_mat * donnee_m; donnee_flo * donnee_f; int longueurm; int longueurf;} contexte;

#include "resolution.h"

float getElt(pmatrice m, int i, int j);
void setElt(pmatrice m, int i, int j, float valeur);

pmatrice nouvelleMatrice(int hauteur, int largeur);

pmatrice tabMatrice(int hauteur, int largeur, char ** tab);

void identite(pmatrice m); // m devient une matrice identité

// En commentaire, les commandes associées de minicas
pmatrice addition(pmatrice m1, pmatrice m2); // addition
pmatrice soustraction(pmatrice m1, pmatrice m2); // sub
float produit(pmatrice m1, pmatrice m2, int a, int b);
pmatrice multiplication(pmatrice m1, pmatrice m2); // mult
pmatrice multiplication_scal(pmatrice M, float valeur); // mult_scal
pmatrice transposition(pmatrice M); // transpose

void libereMatrice(pmatrice m);

#endif
