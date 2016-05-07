#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include "matrice.h"

#define GNUPLOT_PATH "/usr/bin/gnuplot" // chemin d'accès vers gnuplot

#define MIN -99
#define MAX 99

pid_t pid;

int flag_s = 1;

/* @brief Récupèration d'un élément d'une matrice
 *
 * @param pmatrice m
 * @param int i la ligne
 * @param int j la colonne
*/
float getElt(pmatrice m, int i, int j)
{
  return m->tab[i][j];
}

/* @brief Insertion d'un élément d'une matrice
 *
 * @param pmatrice m
 * @param int i la ligne
 * @param int j la colonne
 * @param float valeur
*/
void setElt(pmatrice m, int i, int j, float valeur)
{
  m->tab[i][j] = valeur;
}

/* @brief Création et allocation d'une nouvelle matrice
 *
 * @param int hauteur
 * @param int largeur
*/
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

/* @brief Renvoie une copie d'une matrice
 *
 * @param pmatrice a
*/
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

/* @brief Renvoie une matrice avec les valeurs du tableau en paramètre
 *
 * @param int hauteur de la matrice
 * @param int largeur de la matrice
 * @param float * tab
*/
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

/* @brief La matrice en paramètre devient une matrice identité
 *
 * @param pmatrice m modifiée par effet de bord
*/
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

/* @brief Renvoie une nouvelle matrice avec l'addition m1 + m2
 *
 * @param pmatrice m1
 * @param pmatrice m2
*/
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
    printf("          addition impossible\n");
    return NULL;
  }
}

/* @brief Renvoie une nouvelle matrice avec la soustraction m1 - m2
 *
 * @param pmatrice m1
 * @param pmatrice m2
*/
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
    printf("          soustraction impossible\n");
    return NULL;
  }
}

/* @brief Renvoie le produit de deux lignes de deux matrices
 *
 * @param pmatrice m1
 * @param pmatrice m2
 * @param int a la ligne
 * @param int b la colonne
*/
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

/* @brief Renvoie une nouvelle matrice avec la multiplication m1 * m2
 *
 * @param pmatrice m1
 * @param pmatrice m2
*/
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
    printf("          multiplication impossible\n");
    return NULL;
  }
}

/* @brief Renvoie une nouvelle matrice avec la multiplication par un scalaire M * valeur
 *
 * @param pmatrice M
 * @param float valeur
*/
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

/* @brief Renvoie une nouvelle matrice avec la transposée de M
 *
 * @param pmatrice M
*/
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

/* @brief Affiche une matrice
 *
 * @param pmatrice m
*/
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


/* @brief Renvoie une nouvelle matrice avec M^n
 *
 * @param pmatrice M
 * @param int n
*/
pmatrice expo(pmatrice M, int n)
{
  if(M->hauteur == M->largeur)
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
  else
  {
    printf("          expo impossible\n");
    return NULL;
  }
}

/* @brief Libére l'espace alloué par une matrice
 *
 * @param pmatrice a
*/
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

/* @brief Génère une matrice aléatoire entre les variables MIN et MAX
 *
 * @param pmatrice m
*/
void genereMatrice(pmatrice m)
{
  int i, j;

  for(i = 0; i < m->hauteur; i++)
  {
    for(j = 0; j < m->largeur; j++)
    {
      setElt(m, i, j, rand()%(MAX-MIN) + MIN);
    }
  }
}

/* @brief Interruption du speedtest
*/
void handlerSpeedtest()
{
  kill(pid, 9);
  flag_s = 0;
}

/* @brief Effectue un speedtest sur une commande
 *
 * @param int f l'identifiant de la fonction
 * @param int debut la taille minimale
 * @param int fin la taille maximale
 * @param int pas
 * @param int s la durée maximum (unité : seconde), -1 = pas de maximum
*/
void speedtest(int f, int debut, int fin, int pas, int s)
{
  if(f == 1 || f == 2 || f == 3)
  {
    int i, raison;

    FILE * gnuplot_data = fopen("gnuplot_data", "w+");

    struct timeval tv;
    struct timeval tv2;

    pmatrice a, b;

    struct sigaction sa;
    sa.sa_handler = handlerSpeedtest;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGALRM, &sa, NULL);

    for(i = debut; i <= fin && flag_s; i += pas)
    {
      a = nouvelleMatrice(i, i);
      genereMatrice(a);

      b = nouvelleMatrice(i, i);
      genereMatrice(b);

      if(s != -1)
      {
        alarm(s);
      }

      gettimeofday(&tv, NULL); // début du temps

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

          exit(2);

        default:

          wait(&raison);
          break;
      }

      gettimeofday(&tv2, NULL); // fin du temps

      if(WIFEXITED(raison))
      {
        fprintf(gnuplot_data, "%d %ld\n", i, (long int)(tv2.tv_usec - tv.tv_usec));
      }

      free(a);
      free(b);
    }

    // on affiche maintenant un graphique avec gnuplot

    FILE *gp;

    gp = popen(GNUPLOT_PATH, "w");

    if(gp == NULL)
    {
        fprintf(stderr, "Oops, I can't find %s.", GNUPLOT_PATH);
        exit(1);
    }

    fprintf(gp, "load \"config\"\n");
    fflush(gp);
    getchar();
    pclose(gp);
  }
  else
  {
    printf("          commande incorrecte pour le speedtest\n");
  }
}

// int main()
// {
//   pmatrice res;
//
//   float f;
//
//   pmatrice a = nouvelleMatrice(2, 2);
//
//   setElt(a, 0, 0, 5);
//   setElt(a, 0, 1, -3);
//   setElt(a, 1, 0, 6);
//   setElt(a, 1, 1, -4);
//
//   vecteurValeurPropre(a, res, &f, 50);
//
//   printf("----------\n");
//
//   afficheMatrice(res);
//
//   printf("f = %f\n", f);
// }
