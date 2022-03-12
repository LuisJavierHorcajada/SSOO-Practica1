#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int createDirectory();
pid_t createProcess(char *);
int createProcesses();

int main(int argc, char *argv[]){

    FILE *log;
    log = fopen("./log.txt", "a");
    fprintf(log, "\n******** Log del sistema ********\n");
    createDirectory();
    fprintf(log, "Creación de directorios finalizada.\n");
    createProcesses(log);
    fclose(log);
    return EXIT_SUCCESS; 
}

int createDirectory(){

    pid_t pid = createProcess("./exec/pa");
    int state;
    int cause;
    waitpid(pid, &state, 0);
    cause = WIFEXITED(state);
    if (cause != 0){
        printf("Process PA finalized : EXIT CODE = %d \n", cause);
        return EXIT_SUCCESS;
    }
    else{
        printf("Error executing process PA\n");
        return EXIT_FAILURE;
    }
}

int createProcesses(FILE *log){

    pid_t pid, pid2;
    int state, state2;
    int cause, cause2;

    pid = createProcess("./exec/pb");
    pid2 = createProcess("./exec/pc");

    waitpid(pid, &state, 0);
    fprintf(log, "Copia de modelos de examen, finalizada.\n");
    waitpid(pid2, &state2, 0);
    fprintf(log, "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n");
    cause = WIFEXITED(state);
    cause2 = WIFEXITED(state2);

    if (cause != 0 && cause2 != 0){
        printf("Processes PB and PC finalized properly: EXIT CODE = %d and %d\n", cause, cause2);
        return EXIT_SUCCESS;
    }
    else{
        printf("Error executing processes PB or PC\n");
        return EXIT_FAILURE;
    }
}

pid_t createProcess(char * string){

    pid_t pid;

    if((pid = fork()) == -1)
        perror("Error creating child process\n");
    else if (pid == 0){
        char *arg_list[] = {NULL};
        char *var_list[] = {NULL};
        execve(string, arg_list, var_list);
        printf("Error executing execve ");
        printf("%s\n", string);
        return -1;
    }
    return pid;
}