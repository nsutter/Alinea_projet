#include <stdio.h>
#include <stdlib.h>

float getElt(pmatrice m, int i, int j)
{
  return m->tab[i][j];
}

void setElt(pmatrice m, int i, int j, float valeur)
{
  m->tab[i][j] = valeur;
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
    pmatrice m = nouvelleMatrice(m1->largeur,m1->hauteur);

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
    pmatrice m = nouvelleMatrice(m1->largeur,m1->hauteur);

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
    pmatrice m = nouvelleMatrice(m1->hauteur,m2->largeur);

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

pmatrice multiplication_scal(const pmatrice M, float valeur)
{
  pmatrice m = nouvelleMatrice(M->hauteur,M->largeur);

  int i, j;

  for(i = 0; i < M->hauteur; i++)
  {
    for(j = 0; j < M->largeur; j++)
    {
      setElt(m, i, j, v * getElt(M, i, j));
    }
  }

  return m;
}

int main(int argc, char *argv[])
{

  return 0;
}
