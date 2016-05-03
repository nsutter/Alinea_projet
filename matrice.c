<<<<<<< HEAD
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

// typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

pmatrice nouvelleMatrice(int hauteur, int largeur)
{
  pmatrice m = malloc(sizeof(matrice));

  m->hauteur = hauteur;
  m->largeur = largeur;

  int i, j;

  m->tab = malloc(hauteur * sizeof(float*));

  for(i = 0; i < m->hauteur; i++)
  {
    m->tab[i] = malloc(largeur * sizeof(float));
  }

  return m;
}

pmatrice tabMatrice(int hauteur, int largeur, int tab[])
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
  int i, j;
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

void displayMatrix(pmatrice m)
{
  int i,j;

  for(i = 0; i < m->hauteur; i++)
  {
    for(j = 0; j < m->largeur; j++)
    {
      printf("%f ", getElt(m,i,j));
    }
    printf("\n");
  }

  printf("\n");
}

int main()
{

  //int tab[9] = {1, -1, 2, 3, 2, 1, 2, -3, -2};
  int tab[9] = {1, -1, 2, 0, 5, -5, 0, 0, 7};

  pmatrice m1 = tabMatrice(3, 3, tab);

  displayMatrix(m1);

  //int tab2[3] = {5, 10, -10};
  int tab2[3] = {5, -5, -21};

  pmatrice m2 = tabMatrice(3, 1, tab2);

  int tabres[3] = {0, 0, 0};

  pmatrice mres = tabMatrice(3, 1, tabres);

  displayMatrix(mres);

  remontee(m1, m2, mres);

  displayMatrix(mres);
}
