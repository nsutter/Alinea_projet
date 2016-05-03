#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef struct{int largeur; int hauteur; float ** tab;} *pmatrice, matrice;

typedef struct{char * nom; matrice * pointeur;} *donnee_mat;
typedef struct{char * nom; float * pointeur;} *donnee_flo;
typedef struct{donnee_mat * donnee_m; donnee_flo * donnee_f; int longueurm; int longueurf;} contexte;

char **separe( char *chaine, const char *separateurs )
{
	char **tab;
	int i, s, m, size=10;

	tab = malloc( size * sizeof(char*) );
	m = 0;
	i = 0;
	while( chaine[i] != 0 )
	{
		// saute un séparateur
		for( s=0 ; separateurs[s]!=0 ; s++ )
			if( chaine[i] == separateurs[s] )
				break;
		if( separateurs[s]!=0 )
		{
			chaine[i++] = 0;	// met un fin de chaine à la place du séparateur et avance
			continue;	// les séparateurs n'ont pas été épuisés
		}

		if( chaine[i] != 0 )
			tab[m++] = chaine+i;
		if( m == size )
		{
			// si j'atteinds la limite de la taille de mon tableau, je l'agrandis.
			size += 10;
			tab = realloc( tab, size * sizeof(char*) );
		}
		// saute les caractères non séparateurs
		for( ; chaine[i]!=0 ; i++ )
		{
			for( s=0 ; separateurs[s]!=0 ; s++ )
				if( chaine[i] == separateurs[s] )
					break;
			if( separateurs[s]!=0 )
				break;	// trouvé un caractère séparateur, j'arrête d'avancer et je passe au mot suivant
		}
	}
	tab[m] = NULL;
	return(tab);
}

void matrix(char * cmd)
{
	int i, j;
	for(i=0; cmd[i] != '\n' && cmd[i] != '('; i++);
	for(j=i; cmd[j] != '\n'; j++)
		cmd[j-i]= cmd[j];
}

int main(int argc, char **argv)
{
   struct stat buf;

   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n=0; // initialisation sans importance

	 contexte* ct= malloc(sizeof(contexte));
	 ct->donnee_m= malloc(0*sizeof(donnee_mat));
	 ct->longueurm= 0;
	 ct->donnee_f= malloc(0*sizeof(donnee_flo));
	 ct->longueurf= 0;

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
				 printf("Ligne : %s\n", line);
				 char ** tab= separe(line, " ");
				 int lg;
				 for(lg=0; tab[lg] != NULL ; lg++) ;
				 if(lg>1 && strcmp(tab[1], ":") == 0)
				 {
					 // on traite les données avec

					 ct->longueurf++;
					 ct->donnee_f= realloc(ct->donnee_f, ct->longueurf*sizeof(donnee_flo));
					 donnee_flo d= malloc(sizeof(donnee_flo));
					 d->nom= malloc(strlen(tab[0])+1);
					 strcpy(tab[1], d->nom);
					 d->pointeur= malloc(sizeof(float));
					 *(d->pointeur)= atof(tab[2]);
					 printf("					%f\n", *d->pointeur);
				 }
				 else
				 {
					 if(strncmp(tab[0], "matrix", 6) == 0) matrix(line);
					 else printf("					cmd not found\n");
				 }
         free(line); line=NULL;
         printf("\n>");
       }
    }

   return 0;
}
