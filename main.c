#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

typedef struct{char * nom; void * pointeur} donnee;

typedef struct{ void ** donnee; int longueur} contexte;

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
         if(strcmp(line, "quit\n") == 0){exit(0);}
         int i;
         int var=0, 
         for(i=0; i<n; i++)
         {

         }
        //  printf("Ligne : %s\n", line);
         printf("\n>");
         free(line); line=NULL;
       }
    }

   return 0;
}
