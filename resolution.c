#include <stdio.h>
#include <stdlib.h>

// typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

pmatrice remontee(pmatrice a, pmatrice b, pmatrice x)
{
  int i, j, n = a->hauteur - 1;

  for(i = n; i >= 0; i--)
  {
    setElt(x, 0, i, getElt(b, 0, i));

    for(j = i + 1; j <= n; j++)
    {
      setElt(x, 0, i, getElt(x, 0, i) - getElt(a, i, j) * getElt(x, 0, j));
    }

    setElt(x, 0, i, getElt(x, 0, i) / getElt(a, i, i));
  }

  return x;
}


pmatrice addition_multiple(pmatrice a, pmatrice b, int h, int l, int coefficient)
{
  int i;

  for(i = 0; i < a->hauteur; i++)
  {
    setElt(a, h, i, getElt(a, h, i) + coefficient * getElt(a, l, i))
  }

  setElt(b, 0, l, getElt(b, 0, l) + coefficient * getElt(b, 0, l));
}
