#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAX_STRING_SIZE 100
#define FILENAME "./estudiantes_p1.text"

void getExam(char*, char*);
int copyFile(char *, char*);
int main(int argc, char *argv[]){

    FILE *file;

    char line[MAX_STRING_SIZE];
    char *p_dni;
    char *p_token;

    if ((file = fopen(FILENAME, "r"))!= NULL){
        while (fgets(line, sizeof(line), file) != NULL) {
            p_dni = strtok(line, " ");
            p_token = strtok(NULL, " ");
            getExam(p_token, p_dni);
        }
    fclose(file);
    }
        
    else{
    }
    return EXIT_SUCCESS;
}

void getExam(char *letters, char *p_dni){

    char letter = letters[0];
    char source_file [MAX_STRING_SIZE];
    char dest_file [MAX_STRING_SIZE];

    sprintf(source_file, "./MODELOSEXAMEN/MODELO%c.pdf", letter);
    sprintf(dest_file, "./estudiantes/%s/MODELO%c.pdf", p_dni, letter);
    copyFile(source_file, dest_file);
}

int copyFile(char *sourceFile, char *destFile){

    FILE *source, *destination;
    if((source = fopen(sourceFile,"r")) == NULL)
        return EXIT_FAILURE;
    if((destination = fopen(destFile, "w")) == NULL){
        fclose(source);
        return EXIT_FAILURE;
    }
    int n = 0;
    char buf [BUFSIZ];

    while ((n = fread(buf, 1, BUFSIZ, source)) > 0) {
       fwrite(buf, 1, n, destination);
    }
    fclose(source);
    fclose(destination);

    return EXIT_SUCCESS;
}
