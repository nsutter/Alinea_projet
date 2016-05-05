#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"

float getElt(pmatrice m, int i, int j)
{
  return m->tab[i][j];
}

void setElt(pmatrice m, int i, int j, float valeur)
{
  m->tab[i][j] = valeur;
}

pmatrice nouvelleMatrice(int hauteur, int largeur)
{
  pmatrice m = malloc(sizeof(matrice));

  m->hauteur = hauteur;
  m->largeur = largeur;

  int i;

  m->tab = malloc(hauteur * sizeof(float*));

  for(i = 0; i < m->hauteur; i++)
  {
    m->tab[i] = malloc(largeur * sizeof(float));
  }

  return m;
}

pmatrice copieMatrice(pmatrice a)
{
  pmatrice b = nouvelleMatrice(a->hauteur, a->largeur);

  int i, j;

  for(i = 0; i < b->hauteur; i++)
  {
    for(j = 0; j < b->largeur; j++)
    {
      setElt(b, i, j, getElt(a, i, j));
    }
  }

  return b;
}

pmatrice tabMatrice(int hauteur, int largeur, float * tab)
{
  pmatrice m = nouvelleMatrice(hauteur, largeur);

  int i, j, k = 0;

  for(i = 0; i < hauteur; i++)
  {
    for(j = 0; j < largeur; j++)
    {
      setElt(m, i, j, tab[k]);
      k++;
    }
  }

  return m;
}

void identite(pmatrice m)
{
  int i, j;

  for(i = 0; i < m->hauteur; i++)
  {
    for(j = 0; j < m->largeur; j++)
    {
      if(i == j)
      {
        setElt(m, i, j, 1);
      }
      else
      {
        setElt(m, i, j, 0);
      }
    }
  }
}

pmatrice addition(pmatrice m1, pmatrice m2)
{
  if(m1->largeur == m2->largeur && m1->hauteur == m2->hauteur) // addition possible
  {
    pmatrice m = nouvelleMatrice(m1->hauteur, m1->largeur);

    int i, j;

    for(i = 0; i < m->hauteur; i++)
    {
      for(j = 0; j < m->largeur; j++)
      {
        setElt(m, i, j, getElt(m1, i, j) + getElt(m2, i, j));
      }
    }

    return m;
  }
  else // addition impossible
  {
    return NULL;
  }
}

pmatrice soustraction(pmatrice m1, pmatrice m2)
{
  if(m1->largeur == m2->largeur && m1->hauteur == m2->hauteur) // soustraction possible
  {
    pmatrice m = nouvelleMatrice(m1->hauteur, m1->largeur);

    int i, j;

    for(i = 0; i < m->hauteur; i++)
    {
      for(j = 0; j < m->largeur; j++)
      {
        setElt(m, i, j, getElt(m1, i, j) - getElt(m2, i, j));
      }
    }

    return m;
  }
  else // addition impossible
  {
    return NULL;
  }
}

float produit(pmatrice m1, pmatrice m2, int a, int b)
{
  int i;
  float f = 0;

  for(i = 0; i < m1->largeur; i++)
  {
    f += getElt(m1, a, i) * getElt(m2, i, b);
  }

  return f;
}

pmatrice multiplication(pmatrice m1, pmatrice m2)
{
  if(m1->largeur == m2->hauteur)
  {
    pmatrice m = nouvelleMatrice(m1->hauteur, m2->largeur);

    int i, j;

    for(i = 0; i < m->hauteur; i++)
    {
      for(j = 0; j < m->largeur; j++)
      {
        setElt(m, i, j, produit(m1, m2, i, j));
      }
    }

    return m;
  }
  else
  {
    return NULL;
  }
}

pmatrice multiplication_scal(pmatrice M, float valeur)
{
  pmatrice m = nouvelleMatrice(M->hauteur, M->largeur);

  int i, j;

  for(i = 0; i < M->hauteur; i++)
  {
    for(j = 0; j < M->largeur; j++)
    {
      setElt(m, i, j, valeur * getElt(M, i, j));
    }
  }

  return m;
}

pmatrice transposition(pmatrice M)
{
  pmatrice m = nouvelleMatrice(M->largeur, M->hauteur);

  int i, j;

  for(i = 0; i < M->hauteur; i++)
  {
    for(j = 0; j < M->largeur; j++)
    {
      setElt(m, j, i, getElt(M, i, j));
    }
  }

  return m;
}

void afficheMatrice(pmatrice m)
{
  if(m == NULL){printf("erreur");}
  else
  {
    int i,j;

    for(i = 0; i < m->hauteur; i++)
    {
      for(j = 0; j < m->largeur; j++)
      {
        if(j == 0) printf("         [ ");
        printf("%.20g ", getElt(m,i,j));
      }
      printf("]");
      printf("\n");
    }

    printf("\n");
  }
}

pmatrice expo(pmatrice M, int n)
{
  int i;

  pmatrice a = nouvelleMatrice(M->hauteur, M->largeur);

  if(n < 0)
  {
    return NULL;
  }
  else if(n == 0)
  {
    identite(a);

    return a;
  }
  else if(n == 1)
  {
    a = copieMatrice(M);

    return a;
  }
  else
  {
    a = copieMatrice(M);

    pmatrice * tab = malloc(n * sizeof(pmatrice)); // tableau des pointeurs à supprimer à la fin

    for(i = 1; i < n; i++)
    {
      a = multiplication(a, M);

      if(i < n - 1)
      {
        tab[i] = a;
      }

    }

    for(i = 1; i < n - 1; i++)
    {
      libereMatrice(tab[i]);
    }

    free(tab);

    return a;
  }
}

void libereMatrice(pmatrice a)
{
  int i;

  for(i = 0; i < a->hauteur; i++)
  {
    free(a->tab[i]);
  }

  free(a->tab);
  free(a);
}
