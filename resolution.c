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
    j = choixPivotPartiel(a, i);
    echangeLigne(a, b, i, j);

    for(j = i + 1; j < a->hauteur; j++)
    {
      v = -(getElt(a, j, i) / getElt(a, i, i));
      additionMultiple(a, b, j, i, v);
    }
  }
}

pmatrice resolutionGauss(pmatrice a, pmatrice b)
{
  pmatrice c = copieMatrice(a);
  pmatrice d = copieMatrice(b);

  pmatrice res = nouvelleMatrice(b->hauteur, b->largeur);

  triangulaireMatrice(c, d);
  remonteeMatrice(c, d, res);

  libereMatrice(c);
  libereMatrice(d);

  return res;
}

float triangulaireDeterminant(pmatrice a)
{
    int i, j;
    float v = 1;

    pmatrice b = nouvelleMatrice(a->hauteur, 1);

    for(i = 0; i < b->hauteur; i++)
    {
      setElt(b, i, 0, 0);
    }

    for(i = 0; i < a->hauteur - 1; i++)
    {
      j = choixPivotPartiel(a, i);

      if(j != i)
      {
        echangeLigne(a, b, i, j);
        v = -v;
      }

      for(j = i + 1; j < a->hauteur; j++)
      {
        additionMultiple(a, b, j, i, -getElt(a, j, i) / getElt(a, i, i));
      }
    }

  return v;
}

float determinant(pmatrice a)
{
  int i;

  pmatrice b = copieMatrice(a);

  float v = triangulaireDeterminant(b);

  for(i = 0; i < b->hauteur; i++)
  {
    v = v * getElt(b, i, i);
  }

  libereMatrice(b);

  return v;
}

int rang(pmatrice a)
{
  int i, j, rang, flag;

  pmatrice b = copieMatrice(a);

  pmatrice c = nouvelleMatrice(a->hauteur, 1);

  for(i = 0; i < c->hauteur; i++)
  {
    setElt(c, i, 0, 0);
  }

  triangulaireMatrice(b, c);

  for(i = 0; i < a->hauteur; i++)
  {
    flag = 0;

    for(j = 0; j < a->largeur && flag == 0; j++)
    {
      if(getElt(a, i, j) != 0)
      {
        flag = 1;
        rang++;
      }
    }
  }

  libereMatrice(b);

  return rang;
}

pmatrice inverse(pmatrice a)
{
  if(determinant(a) != 0)
  {
    int i, j;
    float f;

    pmatrice res = nouvelleMatrice(a->hauteur, a->largeur);

    pmatrice id = nouvelleMatrice(a->hauteur, a->largeur);
    identite(id);
    
    pmatrice b = nouvelleMatrice(a->hauteur, 1);

    pmatrice x;

    for(i = 0; i < a->largeur; i++)
    {
      for(j = 0; j < a->largeur; j++)
      {
        f = getElt(id, j, i);
        setElt(b, j, 0, f);
      }

      x = resolutionGauss(a, b);

      for(j = 0; j < a->hauteur; j++)
      {
        f = getElt(x, j, 0);
        setElt(res, j, i, f);
      }

      free(x);
    }

    return res;
  }

  return NULL;
}

float maximumAbsolue(pmatrice a)
{
  int i, j;

  float max = -1;

  for(i = 0; i < a->hauteur; i++)
  {
    for(j = 0; j < a->largeur; j++)
    {
      if(abs(getElt(a, i, j)) > max)
      {
        max = abs(getElt(a, i, j));
      }
    }
  }

  return max;
}
