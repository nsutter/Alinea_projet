#ifndef MATRICE_H
#define MATRICE_H

float getElt(pmatrice m, int i, int j);
void setElt(pmatrice m, int i, int j, float valeur);

void identite(pmatrice m); // m devient une matrice identité

pmatrice addition(pmatrice m1, pmatrice m2); // addition
pmatrice soustraction(pmatrice m1, pmatrice m2); // sub
float produit(pmatrice m1, pmatrice m2, int a, int b);
pmatrice multiplication(pmatrice m1, pmatrice m2); // mult
pmatrice multiplication_scal(const pmatrice M, float valeur); // mult_scal



#endif
