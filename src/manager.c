#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

static pid_t pid_a;
static pid_t pid_b;
static pid_t pid_c;

pid_t createProcess(char *);
int createDirectory(FILE *);
int createCopy(FILE *);
int createExamRequirement(FILE *);
double getAverageMark(pid_t, int *);
void installSignalHandler();
void signal_handler();

int main(int argc, char *argv[]){

    FILE *log;
    log = fopen("./log.txt", "a");
    installSignalHandler();
    fprintf(log, "\n******** Log del sistema ********\n");
    createDirectory(log);
    createCopy(log);
    createExamRequirement(log);
    fclose(log);
    return EXIT_SUCCESS; 
}

pid_t createProcess(char * string){

    pid_t pid;

    if((pid = fork()) == -1)
        perror("Error creating child process\n");
    else if (pid == 0){
        char *arg_list[] = {NULL};
        char *var_list[] = {NULL};
        execve(string, arg_list, var_list);
        printf("Error executing execve\n");
        printf("%s\n", string);
        return -1;
    }
    return pid;
}

pid_t createProcessPipe(char * name, char * args){

    pid_t pid;

    if((pid = fork()) == -1)
        perror("Error creating child process\n");
    else if (pid == 0){
        execl(name, args, NULL);
        printf("Error executing execl\n ");
        printf("%s\n", name);
        return -1;
    }
    return pid;
}

int createDirectory(FILE *log){

    pid_a = createProcess("./exec/pa");
    int state;
    int cause;
    waitpid(pid_a, &state, 0);
    cause = WIFEXITED(state);
    if (cause != 0){
        fprintf(log, "Creación de directorios finalizada.\n");
        return EXIT_SUCCESS;
    }
    else{
        fprintf(log, "Creación de directorios ha tenido un error.\n");
        return EXIT_FAILURE;
    }
}

int createCopy(FILE *log){
    int state;
    int cause;

    pid_b = createProcess("./exec/pb");
    waitpid(pid_b, &state, 0);
    cause = WIFEXITED(state);
    if(cause != 0){
        fprintf(log, "Copia de modelos de examen, finalizada.\n");
        return EXIT_SUCCESS;
    }
    else{
        fprintf(log, "Copia de modelos de examen, ha tenido un error\n");
        return EXIT_FAILURE;
    }
}

int createExamRequirement(FILE *log){

    int state;
    int cause = 0;
    double average_mark = 0.0;
    int pipe2 [2];
    char wr_pipe [256];
    pipe(pipe2);

    sprintf(wr_pipe, "%d", pipe2[1]);

    pid_c = createProcessPipe("./exec/pc", wr_pipe);

    waitpid(pid_c, &state, 0);
    cause = WIFEXITED(state);
    if(cause != 0)
        fprintf(log, "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n");
    else{
        fprintf(log, "Creación de archivos con nota necesaria para alcanzar la nota de corte, ha tenido un error.\n");
        return EXIT_FAILURE;
    }

    average_mark = getAverageMark(pid_c, pipe2);
    fprintf(log, "La nota media de la clase es: %0.2f .\n", average_mark);

    return EXIT_SUCCESS;
}

double getAverageMark(pid_t pid, int *pipe){

    char readbuffer [50], *ptr;
    close(pipe[1]);
    double average_mark = 0.0;
    read(pipe[0], readbuffer, sizeof(readbuffer));

    average_mark = strtod(readbuffer, &ptr);
    return average_mark;
}

void installSignalHandler() {
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "[MANAGER] Error installing signal handler: %s.\n");   
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    exit(EXIT_SUCCESS);
}