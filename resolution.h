#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "matrice.h"

void remonteeMatrice(pmatrice a, pmatrice b, pmatrice x);
void additionMultiple(pmatrice a, pmatrice b, int h, int l, float coefficient);
int choixPivotPartiel(pmatrice a, int i);
void echangeLigne(pmatrice a, pmatrice b, int i, int j);
void triangulaireMatrice(pmatrice a, pmatrice b);
pmatrice resolutionGauss(pmatrice a, pmatrice b);

float triangulaireDeterminant(pmatrice a);
float determinant(pmatrice a);

int rang(pmatrice a);

pmatrice inverse(pmatrice a);

pmatrice moindreCarre(pmatrice m);

float maximumAbsolue(pmatrice a);

int vecteurValeurPropre(pmatrice a, pmatrice b, float * f, int precision);

#endif
