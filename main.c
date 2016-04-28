#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

typedef struct{char * nom; void * pointeur;} donnee;

typedef struct{ void ** donnee; int longueur;} contexte;

int main(int argc, char **argv)
{
   struct stat buf;

   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n=0; // initialisation sans importance

   if (!fstat(0, &buf) && S_ISREG(buf.st_mode))
   {
       line=NULL;
       while (getline(&line, &n, f_in) != -1)
       {
           printf("Ligne : %s\n", line);
           free(line); line=NULL;
       }
    }
    else
    {
       printf(">");
       line=NULL;
       while (getline(&line, &n, f_in)!=1)
       {
         int lg= strlen(line);
         char * chaine= malloc(lg);
         strcpy(line, chaine);
         if(strcmp(chaine, "quit\n") == 0){exit(0);}
         int i;
         int var=0, cmd=0;
         for(i=0; i<n; i++)
         {
           if(line[i] == ':') var =i;
           if(line[i] == '(') cmd =i;
         }
         if(strncmp(line, "matrix", 6) == 0)
         {
           printf("creation d'une matrice \n");
           int ligne =0; int colonne =0;
           for(i=cmd; i<n; i++)
           {
             if(line[i] == '[') {colonne ++;}
             if(line[i] == ',' && colonne%2 == 1) ligne ++;
           }
           ligne= ligne/colonne;
           ligne++;
           //nouvelleMatrice(ligne, colonne);
         }
        //  printf("Ligne : %s\n", line);
         free(line); line=NULL;
         printf("\n>");
       }
    }

   return 0;
}
