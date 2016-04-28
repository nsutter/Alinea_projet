#ifndef MATRICE_H
#define MATRICE_H

float getElt(pmatrice m, int i, int j);
void setElt(pmatrice m, int i, int j, float valeur);
pmatrice addition(pmatrice m1, pmatrice m2);
pmatrice soustraction(pmatrice m1, pmatrice m2);
float produit(pmatrice m1, pmatrice m2, int a, int b);
pmatrice multiplication(pmatrice m1, pmatrice m2);


#endif
