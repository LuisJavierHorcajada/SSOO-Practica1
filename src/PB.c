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
    char *dni;
    char *token;

    if ((file = fopen(FILENAME, "r"))!= NULL){
        while (fgets(line, sizeof(line), file) != NULL) {
            dni = strtok(line, " ");
            token = strtok(NULL, " ");
            getExam(token, dni);
        }
    fclose(file);
    }
        
    else{
        printf("Error opening file");
    }
    
    exit(0);
    return 0;
}

void getExam(char *letters, char *dni){

    char letter = letters[0];
    char sourceFile [MAX_STRING_SIZE];
    char destFile [MAX_STRING_SIZE];

    sprintf(sourceFile, "./MODELOSEXAMEN/MODELO%c.pdf", letter);
    sprintf(destFile, "./estudiantes/%s/MODELO%c.pdf", dni, letter);
    copyFile(sourceFile, destFile);
}

int copyFile(char *sourceFile, char *destFile){

    FILE *source, *destination;
    if((source = fopen(sourceFile,"r")) == NULL)
        return EXIT_FAILURE;
    if((destination = fopen(destFile, "w")) == NULL){
        fclose(source);
        return EXIT_FAILURE;
    }
    int n;
    char buf [BUFSIZ];

    while ((n = fread(buf, 1, BUFSIZ, source)) > 0) {
       fwrite(buf, 1, n, destination);
    }
    fclose(source);
    fclose(destination);

    return EXIT_SUCCESS;
}
