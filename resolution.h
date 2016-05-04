#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "matrice.h"

void remonteeMatrice(pmatrice a, pmatrice b, pmatrice x);
void additionMultiple(pmatrice a, pmatrice b, int h, int l, float coefficient);
int choixPivotPartiel(pmatrice a, int i);
void echangeLigne(pmatrice a, pmatrice b, int i, int j);
void triangulaireMatrice(pmatrice a, pmatrice b);
void resolutionGauss(pmatrice a, pmatrice b, pmatrice x);

float triangulaireDeterminant(pmatrice a);
float determinant(pmatrice a);

int rang(pmatrice a);

#endif
