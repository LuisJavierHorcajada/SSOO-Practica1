#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAX_STRING_SIZE 100
#define FILENAME "./estudiantes_p1.text"

int createFile(int, char*);
int main(int argc, char *argv[]){

    FILE *file;

    char line[MAX_STRING_SIZE];
    char *dni;
    char *token;
    int mark;
    int markNeeded = 10;

    if ((file = fopen(FILENAME, "r"))!= NULL){
        while (fgets(line, sizeof(line), file) != NULL) {
            dni = strtok(line, " ");
            token = strtok(NULL, " ");
            token = strtok(NULL, " ");
            mark = markNeeded - atoi(token);
            createFile(mark, dni);
        }
    fclose(file);
    }
        
    else{
        printf("Error opening file");
    }
    return EXIT_SUCCESS;
}

int createFile(int mark, char *dni){

    FILE *file;
    char destFile [MAX_STRING_SIZE];
    sprintf(destFile, "./estudiantes/%s/nota.txt", dni);
    if((file = fopen(destFile, "w")) == NULL)
        return EXIT_FAILURE;
    
    fprintf(file, "La nota que debes obtener en este nuevo examen para superar la prueba es %d.\n", mark);
    fclose(file);
    return EXIT_SUCCESS;
}