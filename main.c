// SUTTER Nicolas et POIZAT Théo - L2 CMI ISR
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "matrice.h"
#include "resolution.h"

#define PRECISION 0.05

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


void free_context(contexte * ct)
{
	int i;
	for(i=0; i<ct->longueurm; i++)
	{
		free(ct->tab_mat[i]->nom);
		libereMatrice(ct->tab_mat[i]->pointeur);
		free(ct->tab_mat[i]);
	}
	free(ct->tab_mat);
	for(i=0; i<ct->longueurf; i++)
	{
		free(ct->tab_flo[i]->nom);
		free(ct->tab_flo[i]);
	}
	free(ct->tab_flo);
	free(ct);
}

// shift un tableau à partir d'une position
void shift(char * tab, int pos)
{
	int i;
	for(i=pos; i< (int)strlen(tab); i++)
		tab[i]=tab[i+1];
}

// recherche si une matrice à deja été définie et renvoie un pointeur vers elle ou NULL
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

// recherche un flottant dans le contexte renvoi 1 si il à été trouvé et modifie res par effet de bords, renvoi 0 sinnon
int recherche_flo(char * nom_rech, contexte * c, float* res)
{
	int i;
	for(i=0; i< c->longueurf; i++)
	{
		if(strcmp(c->tab_flo[i]->nom, nom_rech) == 0)
		{
			*(res)= c->tab_flo[i]->val;
			return 1;
		}
	}
	return 0;
}

// cette fonction creer une matrice avec matrix([...])
pmatrice matrix(char * cmd, contexte * ct)
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
	//suppression des charactères inutiles
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
		float* res_flo= malloc(sizeof(float));
		if((res[i][0]<'0' || res[i][0]>'9') && res[i][0] != '.' && res[i][0] != '-')
		{
			if(recherche_flo(res[i], ct, res_flo) == 1)
			{
				res_f[i]=*res_flo;
			}
			else
			{
				printf("					erreur variable inconnue\n");
				free(res_f);
				free(res);
				free(res_flo);
				return NULL;
			}
		}
		else
			res_f[i]= atof(res[i]);
		free(res_flo);
	}
	mat = tabMatrice(lignes, colonnes, res_f);
	free(res_f);
	free(res);
	return mat;
}

//appel les fonctions qui renvoie un float et renvoi ce float
// int fct est un int qui indique quelle fonction appeler
float fonction_f(char * cmd, contexte * ct, int fct, float * res)
{
	int i; int lg= strlen(cmd); int parenthese=0; int fin=0;
	for(i=0; i < lg; i++)
	{
		if(fin == 1) {shift(cmd, i); i--; lg--;}
		else if(cmd[i] == '(') {parenthese ++; shift(cmd, i); i--; lg--;}
		else if(parenthese == 0){shift(cmd, i); i--; lg--;}
		else if(cmd[i] == ')') {parenthese --; fin = 1; shift(cmd, i); i--;lg--;}
		else if(cmd[i] == ' ') {shift(cmd, i); i--;lg--;}
	}
	matrice * m1;
	int alloue_mat=0;
	if(strncmp(cmd, "matrix", 6) == 0)
	{
		alloue_mat=1;
		m1=matrix(cmd, ct);
	}
	else
		m1= recherche_mat(cmd, ct);
	if(m1 != NULL)
	{
		if(fct == 20)
		{
			*res= determinant(m1);
		}
		else if(fct == 21)
		{
			*res= rang(m1);
		}
		if(alloue_mat == 1) libereMatrice(m1);
		return 0;
	}
	return -1;
}

//fonction qui eppele les fonctions qui renvoie une matrice et renvoie cette matrice
// fct est un int qui indique quelle fonction appeler
pmatrice fonction(char * cmd, contexte * ct, int fct)
{
	//suppression du nom de la focntion
	while(cmd[0] != '(') shift(cmd, 0);
	int i, virgule=0;
	//suppression des trucs inutiles
	for(i=0; cmd[i] != '\0'; i++)
	{
		if(cmd[i] == '(' || cmd[i] == ')' || cmd[i] == ' ' || cmd[i] == '\n' || cmd[i]== 13)
		{
			shift(cmd, i);
			i-- ;
		}
		else if(cmd[i] == ',') virgule=1;
	}

	if((virgule == 0 && fct < 10 && fct!=13) || (virgule != 0 && fct >10 && fct != 13))
	{
		printf("					erreur argument");
		return NULL;
	}
	matrice * m1; matrice * m2;
	float f1;
	char ** tab;
	int alloue_mat=0;
	tab= separe(cmd, ",");
	if(strncmp(tab[0], "matrix", 6) == 0)
	{
		alloue_mat =1;
		m1=matrix(tab[0], ct);
	}
	else
		m1= recherche_mat(tab[0], ct);

	if(fct < 5)
	{
		if(fct != 13 && strncmp(tab[1], "matrix", 6) == 0)
		{
			alloue_mat= alloue_mat+2;
			m2=matrix(tab[1], ct);
		}
		else
			m2= recherche_mat(tab[1], ct);
	}
	else if(fct < 10)
	{
		if((tab[1][0] >= '0' && tab[1][0] <= '9') || tab[1][0] == '-' || tab[1][0] == '.')
			f1=atoi(tab[1]);
		else
			if(recherche_flo(tab[1], ct, &f1) == 1){ printf("					erreur argument\n"); free(tab); return NULL;}
	}
	free(tab);
	// appel de la fonction si les paramètres sont les bons
	if(m1 != NULL && (fct >5 || m2 != NULL))
	{
		if(fct == 1)
		{
			matrice * res= multiplication(m1, m2);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 2)
		{
			matrice * res= addition(m1, m2);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 3)
		{
			matrice * res= soustraction(m1, m2);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 4)
		{
			matrice * res= resolutionGauss(m1, m2);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 5)
		{
			matrice * res= multiplication_scal(m1, f1);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 6)
		{
			matrice * res= expo(m1, (int)f1);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 11)
		{
			matrice * res= transposition(m1);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 12)
		{
			matrice * res= inverse(m1);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
		else if(fct == 13)
		{
			matrice * res;
			if(virgule == 0)
				res= moindreCarre(m1,"gnuplot_points");
			else if(virgule == 1)
				res= moindreCarre(m1,tab[1]);
			if(alloue_mat == 1) libereMatrice(m1);
			else if(alloue_mat == 2) libereMatrice(m2);
			else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
			return res;
		}
	}
	else
		printf("					erreur paramètre\n");
	if(alloue_mat == 1) libereMatrice(m1);
	else if(alloue_mat == 2) libereMatrice(m2);
	else if(alloue_mat == 3) {libereMatrice(m1); libereMatrice(m2);}
	return NULL;
}

int main()
{
   struct stat buf;
   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n = 0; // initialisation sans importance

	 srand(time(NULL));

	 contexte* ct= malloc(sizeof(contexte));
	 ct->tab_mat= malloc(0*sizeof(mat));
	 ct->longueurm= 0;
	 ct->tab_flo= malloc(0*sizeof(flo));
	 ct->longueurf= 0;


   line=NULL;
	 if (!fstat(0, &buf) && S_ISREG(buf.st_mode));
	 else printf("\n>");
   while (getline(&line, &n, f_in)!=1)
   {
		 if (!fstat(0, &buf) && S_ISREG(buf.st_mode))
		 {
		 		printf(">%s\n", line);
		 }
		 if(strcmp(line, "quit\n") == 0){free(line); free_context(ct); fclose(f_in); exit(0);}
		 else if(strcmp(line, "quit") == 0){free(line); free_context(ct); fclose(f_in); exit(0);}
		 char cmd[strlen(line)];
		 strcpy(cmd, line);
		 char ** tab= separe(line, ":");

		 int lg; int i;
		 for(lg=0; tab[lg] != NULL ; lg++) ;
		 if(lg>1)
		 {
			 while(tab[1][0] == ' ')for(i=0; i< (int) strlen(tab[1]); i++) tab[1][i]=tab[1][i+1]; // suppression du premier espace
			 tab[1][strlen(tab[1])]= '\0';
			 for(i=1; tab[0][strlen(tab[0])-i] == ' '; i++) tab[0][strlen(tab[0])-i]= '\0'; // suppression espace
			 if(tab[1][0] < '0' || tab[1][0] > '9')
			 {
				 matrice * ptr_mat_tmp;
				 int fct=0;
				 if(strncmp(tab[1], "matrix", 6) == 0)
				 {
					 ptr_mat_tmp= matrix(tab[1], ct);
					 fct = -1;
				 }
				 else if(strncmp(tab[1], "mult(", 5) == 0)
					 fct = 1;
				 else if(strncmp(tab[1], "addition(", 9) == 0)
				 	 fct = 2;
				 else if(strncmp(tab[1], "sub(", 4) == 0)
				 	 fct = 3;
				 else if(strncmp(tab[1], "solve(", 6) == 0)
				 	 fct = 4;
				 else if(strncmp(tab[1], "mult_scal(", 10) == 0)
					  fct = 5;
			 	 else if(strncmp(tab[1], "expo(", 5) == 0)
					  fct = 6;
				 else if(strncmp(tab[1], "transpose(", 10) == 0)
			  		fct = 11;
				 else if(strncmp(tab[1], "invert(", 7) == 0)
 				  	fct = 12;
				 else if(strncmp(tab[1], "mcarre(", 7) == 0)
 				    	fct = 13;
				 else if(strncmp(tab[1], "determinant(", 12) == 0)
				 	 fct= 20;
				 else if(strncmp(tab[1], "rang(", 5) == 0)
			  		fct= 21;
				 if(fct > 0 && fct < 20)
				 {
					 ptr_mat_tmp= fonction(tab[1], ct, fct);
				 }
				 if(fct != 0 && ptr_mat_tmp != NULL)
				 {
					 afficheMatrice(ptr_mat_tmp);

					 int i;
					 int ok = 0;
			 		 for(i=0; i< ct->longueurm; i++)
					 {
				  		if(strcmp(ct->tab_mat[i]->nom, tab[0]) == 0)
							{
								libereMatrice(ct->tab_mat[i]->pointeur);
								ct->tab_mat[i]->pointeur= ptr_mat_tmp;
								ok = 1;
							}
					 }
					 for(i=0; i< ct->longueurf; i++)
				 	 {
				 			if(strcmp(ct->tab_flo[i]->nom, tab[0]) == 0)
				 			{
								free(ct->tab_flo[i]->nom);
								for(; i< ct->longueurf-1; i++)
								{
									ct->tab_mat[i]= ct->tab_mat[i+1];
								}
								ct->longueurf--;
								free(ct->tab_flo[ct->longueurf]);
								break;
				 			}
				 	 }
					 if(ok == 0)
					 {
						 mat m= malloc(sizeof(sizemat));
						 m->nom= malloc((int)strlen(tab[0])+1);
						 strcpy(m->nom, tab[0]);
						 ct->longueurm++;
						 ct->tab_mat= realloc(ct->tab_mat, ct->longueurm*sizeof(mat));
						 m->pointeur= ptr_mat_tmp;
						 ct->tab_mat[ct->longueurm-1]= m;
					 }
				 }
				 else if(fct > 19)
				 {
					 float res;
					 if(fonction_f(tab[0], ct, fct, &res) == 0)
					 {
						 int i;
						 int ok =0;
				 		 for(i=0; i< ct->longueurm; i++)
						 {
					  		if(strcmp(ct->tab_mat[i]->nom, tab[0]) == 0)
								{
									libereMatrice(ct->tab_mat[i]->pointeur);
									free(ct->tab_mat[i]->nom);
									for(; i< ct->longueurm-1; i++)
									{
										ct->tab_mat[i]= ct->tab_mat[i+1];
									}
									ct->longueurm--;
									free(ct->tab_mat[i+1]);
									break;
								}
						 }
						 for(i=0; i< ct->longueurf; i++)
					 	 {
					 			if(strcmp(ct->tab_flo[i]->nom, tab[0]) == 0)
					 			{
									ct->tab_flo[i]->val= res;
									ok =1;
					 			}
					 	 }
						 if(ok == 0)
						 {
							 printf("					%.20g\n", res);
							 flo f= malloc(sizeof(sizeflo));
							 f->nom= malloc((int)strlen(tab[0])+1 );
							 strcpy(f->nom, tab[0]);
							 f->val= res;
							 ct->longueurf++;
							 ct->tab_flo= realloc(ct->tab_flo, ct->longueurf*sizeof(flo));
							 ct->tab_flo[ct->longueurf-1]= f;
						 }
					 }
				 }
				 else if(fct == 0)
				 	printf("					cmd not found\n");
			 }
			 else
			 {
				 int i;
				 int ok =0;
				 for(i=0; i< ct->longueurm; i++)
				 {
						if(strcmp(ct->tab_mat[i]->nom, tab[0]) == 0)
						{
							libereMatrice(ct->tab_mat[i]->pointeur);
							free(ct->tab_mat[i]->nom);
							for(; i< ct->longueurm-1; i++)
							{
								ct->tab_mat[i]= ct->tab_mat[i+1];
							}
							ct->longueurm--;
							free(ct->tab_mat[ct->longueurm]);
							break;
						}
				 }
				 for(i=0; i< ct->longueurf; i++)
				 {
						if(strcmp(ct->tab_flo[i]->nom, tab[0]) == 0)
						{
							ct->tab_flo[i]->val= atof(tab[1]);
							printf("					%.20g\n", ct->tab_flo[i]->val);
							ok =1;
						}
				 }
				 if(ok == 0)
				 {
					 flo f= malloc(sizeof(sizeflo));
					 f->nom= malloc(strlen(tab[0])+1 );
					 strcpy(f->nom, tab[0]);
					 f->val= atof(tab[1]);
					 ct->longueurf++;
					 ct->tab_flo= realloc(ct->tab_flo, ct->longueurf*sizeof(flo));
					 ct->tab_flo[ct->longueurf-1]= f;
					 printf("					%.20g\n", f->val);
				 }
			 }
		 }
		 else
		 {
			 int fct =0;
			 tab[0][strlen(tab[0])-1]='\0'; // suppression du \n
			 for(i=1; tab[0][strlen(tab[0])-i] == ' '; i++) tab[0][strlen(tab[0])-i]= '\0'; // suppression des espaces
			 if(strncmp(tab[0], "matrix(", 7) == 0)
			 {
				 matrice * m= matrix(cmd, ct);
				 if(m!=NULL)
				 {
					 afficheMatrice(m);
					 libereMatrice(m);
				 }
			 }
			 else if(strncmp(tab[0], "mult(", 5) == 0)
			 	 fct = 1;
			 else if(strncmp(tab[0], "addition(", 9) == 0)
				 fct = 2;
			 else if(strncmp(tab[0], "sub(", 4) == 0)
					fct = 3;
			 else if(strncmp(tab[0], "solve(", 6) == 0)
		 		  fct = 4;
			 else if(strncmp(tab[0], "mult_scal(", 10) == 0)
				  fct = 5;
		 	 else if(strncmp(tab[0], "expo(", 5) == 0)
				  fct = 6;
			 else if(strncmp(tab[0], "transpose(", 10) == 0)
		  		fct = 11;
			 else if(strncmp(tab[0], "invert(", 7) == 0)
				  fct = 12;
		   else if(strncmp(tab[0], "mcarre(", 7) == 0)
					fct = 13;
			 else if(strncmp(tab[0], "determinant(", 12) == 0)
		  		fct= 20;
			 else if(strncmp(tab[0], "rang(", 5) == 0)
		  		fct= 21;
			 else if(strncmp(tab[0], "speedtest ", 10) ==0)
			 {
				 char ** speed_arg= separe(tab[0], " ");
				 int i,j;
				 int erreur= 0;
				 for(i=0; speed_arg[i] != NULL; i++)
				 {
					 for(j=0; speed_arg[i][j] != '\0'; j++)
					 {
						 if(i > 1 && speed_arg[i][j] >= '0' && speed_arg[i][j] <= '9');
						 else if(i>1)
						 {
							 printf("					erreur arguments\n");
							 erreur = 1;
						 }
					 }
				 }
				 if(i < 4 && erreur == 0)
				 		printf("					erreur nombre d'arguments\n");
				 else if(erreur == 0)
				 {
					 int fct_speed=0;
					 if(strcmp("addition", speed_arg[1]) == 0) fct_speed=2;
					 else if(strcmp("sub", speed_arg[1]) == 0) fct_speed=3;
					 else if(strcmp("mult", speed_arg[1]) == 0) fct_speed= 1;
					 else
					    printf("					commande en argument inconnue\n");
					 if(fct_speed != 0 && i != 5)
					 	 speedtest(fct_speed, atoi(speed_arg[2]), atoi(speed_arg[3]), atoi(speed_arg[4]), -1);
					 else if(fct_speed != 0 && i == 5)
					 	 speedtest(fct_speed, atoi(speed_arg[2]), atoi(speed_arg[3]), atoi(speed_arg[4]), atoi(speed_arg[5]));
				 }
				 free(speed_arg);
			 }
			 else if(strncmp(tab[0], "vp(", 3) == 0)
			 {
				 char** vp_arg_int= separe(tab[0], "(");
				 char** vp_arg= separe(vp_arg_int[1], ")");
				 free(vp_arg_int);
				 matrice * m1; float res;
				 int alloue_mat=0;
			 	 if(strncmp(vp_arg[0], "matrix", 6) == 0)
				 {
					 alloue_mat =1;
					 m1=matrix(vp_arg[0], ct);
				 }
				 else
			 		 m1= recherche_mat(vp_arg[0], ct);
				 if(m1 == NULL)
				 	 printf("					erreur argument\n");
				 else
				 {
					 pmatrice m2 = vecteurValeurPropre(m1, &res, PRECISION);

					 if(m2 != NULL)
					 {
						 printf("					valeur propre : %.20g\n", res);
						 afficheMatrice(m2);
						 libereMatrice(m2);
					 }
					 else
					 	 printf("						an error occured\n");
				 }
				 free(vp_arg);
				 if(alloue_mat == 1) libereMatrice(m1);
			 }
			 else
			 {
				 float * tmp= malloc(sizeof(float));
				 if(recherche_flo(tab[0], ct, tmp) == 1) printf("					%.20g\n", *(tmp) );
				 else if(recherche_mat(tab[0], ct) != NULL) afficheMatrice(recherche_mat(tab[0], ct));
				 else printf("					cmd not found\n");
				 free(tmp);
			 }
			 if(fct > 0 && fct < 20)
			 {
				 matrice * m= fonction(tab[0], ct, fct);
				 if(m!=NULL)
				 {
					 afficheMatrice(m);
					 libereMatrice(m);
				 }
			 }
			 else if(fct > 19)
			 {
				 float res;
				 if(fonction_f(tab[0], ct, fct, &res) == 0)
				 {
					 printf("					%.20g\n", res);
				 }
			 }
		 }
		 free(tab);
     free(line); line=NULL;
		 if (!fstat(0, &buf) && S_ISREG(buf.st_mode));
		 else printf("\n>");
   }

	 free_context(ct);
	 free(line);
	 fclose(f_in);
   return 0;
}
