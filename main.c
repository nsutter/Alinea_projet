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

// matrix([11,2, 3], [3,4, 5])
void matrix(char * cmd)
{
	int i, j, lignes=0, colonnes=0;
	int lg= strlen(cmd);
	for(i=0; cmd[i] != '\n' && cmd[i] != '('; i++);
	for(j=i; cmd[j] != '\n'; j++)
		cmd[j-i]= cmd[j];
	cmd[j-i]= '\0';
	int bool=1;
	for(i=0; i<lg-1; i++)
		cmd[i]=cmd[i+2];
	lg = lg-2;
	for(i=0; cmd[i] != '\0'; i++)
	{
		if(cmd[i] == ']' && bool == 1) {for(j=i; j<lg; j++) cmd[j]= cmd[j+1]; i--; lg --; lignes++;}
		else if(cmd[i] == ']' || cmd[i] == ' ' || cmd[i] == '[' || cmd[i] == ')') {for(j=i; j<lg; j++) cmd[j]= cmd[j+1]; i--; lg --;}
	}
	char ** res= separe(cmd, ",");
	for(i=0; res[i] != NULL; i++)
		colonnes ++;
	colonnes= colonnes/lignes;
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
				 char cmd[strlen(line)];
				 strcpy(cmd, line);

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
					 if(strncmp(tab[0], "matrix", 6) == 0) matrix(cmd);
					 else printf("					cmd not found\n");
				 }
         free(line); line=NULL;
         printf("\n>");
       }
    }

   return 0;
}
