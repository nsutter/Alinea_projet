#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "matrice.h"

#define GNUPLOT_PATH "/usr/bin/gnuplot"

pid_t pid;

int flag_s = 1;

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

    pmatrice * tab = malloc((n - 1) * sizeof(pmatrice)); // tableau des pointeurs à supprimer à la fin

    for(i = 1; i < n; i++)
    {
      if(i < n)
      {
        tab[i - 1] = a;
      }

      a = multiplication(a, M);
    }

    for(i = 0; i < n - 1; i++)
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

void genereMatrice(pmatrice m)
{
  int i, j, min = 0, max = 99;

  for(i = 0; i < m->hauteur; i++)
  {
    for(j = 0; j < m->largeur; j++)
    {
      setElt(m, i, j, rand()%(max-min) + min);
    }
  }
}

void handlerSpeedtest()
{
  kill(pid, 9);
  flag_s = 0;
}

// -1 = pas d'attente avec s
void speedtest(int f, int debut, int fin, int pas, int s)
{
  if(f == 1 || f == 2 || f == 3)
  {
    int i, r;

    struct timeval tv;
    struct timeval tv2;

    pmatrice a, b;

    struct sigaction sa;
    sa.sa_handler = handlerSpeedtest;
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGALRM, &sa, NULL) != -1);

    for(i = debut; i <= fin && flag_s; i += pas)
    {
      a = nouvelleMatrice(i, i);
      genereMatrice(a);

      b = nouvelleMatrice(i, i);
      genereMatrice(b);

      alarm(s);

      gettimeofday(&tv,NULL); // début du temps

      // appel de la fonction f
      pid = fork();

      switch (pid)
      {
        case -1:

          exit(1);
          break;

        case 0:

          if(f == 2)
          {
            addition(a, b);
          }
          else if(f == 3)
          {
            soustraction(a, b);
          }
          else if(f == 1)
          {
            multiplication(a, b);
          }
          break;

        default:

          wait(&r);
          break;
      }

      gettimeofday(&tv2,NULL); // fin du temps

      if(!WIFSIGNALED(r))
      {
        //ecrire
      }

      printf("Temps écoulé : %ld secondes et %ld microsecondes",tv2.tv_sec - tv.tv_sec,tv2.tv_usec - tv.tv_usec);

      free(a);
      free(b);
    }

    // gnupllot

    FILE *gp;

    gp = popen(GNUPLOT_PATH, "w");

    if(gp == NULL)
    {
        fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
        exit(EXIT_FAILURE);
    }

    fprintf(gp, "load \"gnuplot_d\"\n");
    fflush(gp);
    getchar();
    pclose(gp);

  }
  else
  {
    printf("commande inccorecte pour le speedtest\n");
  }
}
