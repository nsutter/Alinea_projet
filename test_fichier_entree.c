#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
   struct stat buf;

   FILE *f_in = fdopen(0, "r");
   char *line;
   size_t n=0; // initialisation sans importance

   if (!fstat(0, &buf) && S_ISREG(buf.st_mode)) {
       line=NULL;
       while (getline(&line, &n, f_in) != -1) {
           printf("Ligne : %s\n", line);
           free(line); line=NULL;
        }
    } else {
       printf("prompt>");
       line=NULL;
       while (getline(&line, &n, f_in)!=1) {
           printf("Ligne : %s\n", line);
           printf("prompt>");
           free(line); line=NULL;
        }
    }

   return 0;
}
