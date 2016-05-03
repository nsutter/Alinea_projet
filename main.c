#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "matrice.h"
#include "resolution.h"

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


matrice * recherche_mat(char * nom_rech, contexte * c)
{
	int i;
	for(i=0; i< c->longueurm; i++)
	{
		if(strcmp(c->tab_mat[i]->nom, nom_rech) == 0)
			return c->tab_mat[i]->pointeur;
	}

	return NULL;
}

int recherche_flo(char * nom_rech, contexte * c, float* res)
{
	int i;
	for(i=0; i< c->longueurf; i++)
	{
		if(strcmp(c->tab_flo[i]->nom, nom_rech) == 0)
		{
			*res= c->tab_flo[i]->val;
			return 1;
		}
	}
	return 0;
}

// matrix([11,2, 3], [3,4, 5])
pmatrice matrix(char * cmd)
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
	pmatrice mat;
	float *res_f= malloc(lignes*colonnes*sizeof(float));

	for(i=0; i<colonnes*lignes; i++)
	{
		for(j=0; j<(int)strlen(res[i]);j++) if(res[i][j]<'0' && res[i][j]>'9' && res[i][j] != '.'); // remplacer
		else
			res_f[i]= atof(res[i]);
	}
	mat = tabMatrice(lignes, colonnes, res_f);
	afficheMatrice(mat);
	free(res_f);
	free(res);
	return mat;
}

int main(int argc, char **argv)
{
   struct stat buf;

   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n=0; // initialisation sans importance

	 contexte* ct= malloc(sizeof(contexte));
	 ct->tab_mat= malloc(0*sizeof(mat));
	 ct->longueurm= 0;
	 ct->tab_flo= malloc(0*sizeof(flo));
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

				 char ** tab= separe(line, ":");

				 int lg; int i;
				 for(lg=0; tab[lg] != NULL ; lg++) ;
				 if(lg>1)
				 {
					 while(tab[1][0] == ' ')for(i=0; i< (int) strlen(tab[1]); i++) tab[1][i]=tab[1][i+1]; // suppression du premier espace
					 for(i=1; tab[0][strlen(tab[0])-i] == ' '; i++) tab[0][strlen(tab[0])-i]= '\0'; // suppression espace
					 if(strncmp(tab[1], "matrix", 6) == 0)
					 {
						 mat m= malloc(sizeof(mat));
						 m->nom= malloc(strlen(tab[0]));
						 strcpy(m->nom, tab[0]);
						 ct->longueurm++;
						 ct->tab_mat= realloc(ct->tab_mat, ct->longueurm*sizeof(mat));
						 m->pointeur= matrix(tab[1]);
						 ct->tab_mat[ct->longueurm-1]= m;
					 }
					 else
					 {
						 flo f= malloc(sizeof(flo));
						 f->nom= malloc(strlen(tab[0])+1 );
						 strcpy(f->nom, tab[0]);
						 f->val= atof(tab[1]);
						 ct->longueurf++;
						 ct->tab_flo= realloc(ct->tab_flo, ct->longueurf*sizeof(flo));
						 ct->tab_flo[ct->longueurf-1]= f;
						 printf("					%.20g\n", f->val);
					 }
				 }
				 else
				 {
					 tab[0][strlen(tab[0])-1]='\0'; // suppression du \n
					 for(i=1; tab[0][strlen(tab[0])-i] == ' '; i++) tab[0][strlen(tab[0])-i]= '\0'; // suppression des espaces
					 float * tmp= malloc(sizeof(float));
					 if(strncmp(tab[0], "matrix", 6) == 0) matrix(cmd);
					 else if(recherche_flo(tab[0], ct, tmp) == 1) printf("					%.20g\n", *(tmp) );
					 else if(recherche_mat(tab[0], ct) != NULL) afficheMatrice(recherche_mat(tab[0], ct));
					 else printf("					cmd not found\n");
					 free(tmp);
				 }
         free(line); line=NULL;
         printf("\n>");
       }
    }

   return 0;
}
