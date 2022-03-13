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

    char line[MAX_STRING_SIZE], msg [20];
    char *p_dni;
    char *p_token;
    int mark = 0, mark_needed = 10, num_students = 0, total_mark = 0;
    double average_mark = 0.0;

    if ((file = fopen(FILENAME, "r"))!= NULL){
        while (fgets(line, sizeof(line), file) != NULL) {
            sleep(1);
            p_dni = strtok(line, " ");
            p_token = strtok(NULL, " ");
            p_token = strtok(NULL, " ");
            mark = mark_needed - atoi(p_token);
            num_students++;
            total_mark += atoi(p_token);
            createFile(mark, p_dni);
        }
    fclose(file);
    average_mark = ((double)total_mark)/((double)num_students);
    sprintf(msg, "%f", average_mark);
    write(atoi(argv[0]), msg, strlen(msg)+1);
    }
        
    else{
        printf("Error opening file");
    }
    return EXIT_SUCCESS;
}

int createFile(int mark, char *dni){

    FILE *file;
    char dest_file [MAX_STRING_SIZE];
    sprintf(dest_file, "./estudiantes/%s/nota.txt", dni);
    if((file = fopen(dest_file, "w")) == NULL)
        return EXIT_FAILURE;
    
    fprintf(file, "La nota que debes obtener en este nuevo examen para superar la prueba es %d.\n", mark);
    fclose(file);
    return EXIT_SUCCESS;
}