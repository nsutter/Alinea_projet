#include <stdio.h>
#include <stdlib.h>
#include "resolution.h"
#include <math.h>

void remonteeMatrice(pmatrice a, pmatrice b, pmatrice x)
{
  int i, j, n = a->hauteur - 1;

  for(i = n; i >= 0; i--)
  {
    setElt(x, i, 0, getElt(b, i, 0));

    for(j = i + 1; j <= n; j++)
    {
      setElt(x, i, 0, getElt(x, i, 0) - getElt(a, i, j) * getElt(x, j, 0));
    }

    setElt(x, i, 0, getElt(x, i, 0) / getElt(a, i, i));
  }
}

void additionMultiple(pmatrice a, pmatrice b, int i, int j, float coefficient)
{
  int k;
  float v, v2;

  for(k = 0; k < a->hauteur; k++)
  {
    v = getElt(a, i, k) + coefficient * getElt(a, j, k);
    setElt(a, i, k, v);
  }

  v2 = getElt(b, i, 0) + coefficient * getElt(b, j, 0);
  setElt(b, i, 0, v2);
}

int choixPivotPartiel(pmatrice a, int i)
{
  int t = i, j, n = a->hauteur;

  float f = abs(getElt(a, i, i));

  for(j = i; j < n; j++)
  {
    if(abs(getElt(a, j, i)) > f)
    {
      t = j;
      f = abs(getElt(a, j, i));
    }
  }

  return t;
}

void echangeLigne(pmatrice a, pmatrice b, int i, int j)
{
  int k;

  float f;

  for(k = 0; k < a->hauteur; k++)
  {
    f = getElt(a, i, k);
    setElt(a, i, k, getElt(a, j, k));
    setElt(a, j, k, f);
  }

  f = getElt(b, i, 0);
  setElt(b, i, 0, getElt(b, j, 0));
  setElt(b, j, 0, f);
}

void triangulaireMatrice(pmatrice a, pmatrice b)
{
  int i, j;
  float v;

  for(i = 0; i < a->hauteur - 1; i++)
  {
	  printf("i =%d\n",i);
    j = choixPivotPartiel(a, i);
    echangeLigne(a, b, i, j);
    printf("APRES echangeLigne \n");
    displayMatrix(a);
    displayMatrix(b);

    for(j = i + 1; j < a->hauteur; j++)
    {
      printf("YOLO ");
      v = -(getElt(a, j, i) / getElt(a, i, i));
      printf("v = %f\n",v);
      additionMultiple(a, b, j, i, v);
    }

    printf("APRES additionMultiple \n");
    displayMatrix(a);
    displayMatrix(b);
  }
}

void resolutionGauss(pmatrice a, pmatrice b, pmatrice x)
{
  displayMatrix(a);
  displayMatrix(b);

  triangulaireMatrice(a, b);
  remonteeMatrice(a, b, x);

  displayMatrix(a);
  displayMatrix(b);
}
