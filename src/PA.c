/* This is the implementation of PA which is in charge of 
creating the directories for all the students */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>


#define MAX_STRING_SIZE 100
#define FILENAME "./estudiantes_p1.text" /*File where students are located */

int main(int argc, char *argv[]){
    /* Main method of PA */
    FILE *file;

    char line[MAX_STRING_SIZE];
    char *p_dni;
    char dir_path [MAX_STRING_SIZE];
    struct stat st = {0};

    if (stat("./estudiantes", &st) == -1) { /*checks if directory exists, if not it creates it */
        mkdir("./estudiantes", 0700);
    }

    if ((file = fopen(FILENAME, "r")) != NULL){
        while (fgets(line, sizeof(line), file) != NULL) {
            p_dni = strtok(line, " "); 
            sprintf(dir_path, "./estudiantes/%s", p_dni);
            if (stat(dir_path, &st) == -1) {
                mkdir(dir_path, 0700);       /* Opens the file and creates directories for each student */
            }
        }
    fclose(file);
    }
        
    else{
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}