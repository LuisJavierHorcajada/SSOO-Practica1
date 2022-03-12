#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#define MAX_STRING_SIZE 100
#define FILENAME "./estudiantes_p1.text"

int main(int argc, char *argv[]){

    FILE *file;

    char line[MAX_STRING_SIZE];
    char *dni;
    char dir_path [MAX_STRING_SIZE];
    struct stat st = {0};

    if (stat("./estudiantes", &st) == -1) {
        mkdir("./estudiantes", 0700);
    }

    if ((file = fopen(FILENAME, "r")) != NULL){
        while (fgets(line, sizeof(line), file) != NULL) {
            dni = strtok(line, " "); 
            sprintf(dir_path, "./estudiantes/%s", dni);
            if (stat(dir_path, &st) == -1) {
                mkdir(dir_path, 0700);
            }
        }
    fclose(file);
    }
        
    else{
        printf("Error opening file");
    }
    
    exit(0);
    return 0;
}