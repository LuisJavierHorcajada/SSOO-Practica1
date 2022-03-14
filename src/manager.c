/* This is the implementation of manager which is in charge of 
creating and managing the other processes */

#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>

static pid_t pid_a = 0;
static pid_t pid_b = 0;
static pid_t pid_c = 0;
static FILE *log;

pid_t createProcess(char *);
int createDirectory();
int createProcesses();
double getAverageMark(pid_t, int *);
void installSignalHandler();
void signal_handler();

int main(int argc, char *argv[]){
    /* Main method of manager */

    log = fopen("./log.txt", "a");
    installSignalHandler();
    fprintf(log, "\n******** Log del sistema ********\n");
    createDirectory();
    createProcesses();
    fprintf(log, "FIN DEL PROGRAMA\n");
    fclose(log);
    return EXIT_SUCCESS; 
}

pid_t createProcess(char * string){
    /* Method that creates a process with the name given as input */

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
    /* Method that creates a process with the name given as input and assigns it a write-only pipe*/

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

int createDirectory(){
    /* Method that creates PA which creates the directories */

    pid_a = createProcess("./exec/pa");
    int state;
    int cause;
    waitpid(pid_a, &state, 0);
    pid_a = 0;
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

int createProcesses(){
    /* Method that creates PB and PC concurrently */

    int state, state2;
    int cause = 1, cause2 = 1;
    bool isDone = false, isDone2 = false;
    double average_mark = 0.0;
    int pipe2 [2];
    char wr_pipe [256];
    pipe(pipe2);
    pid_t pb_hold, pc_hold;

    sprintf(wr_pipe, "%d", pipe2[1]);

    pid_b = createProcess("./exec/pb");
    pid_c = createProcessPipe("./exec/pc", wr_pipe);

   do{
       pb_hold = waitpid(pid_b, &state, WNOHANG);
       pc_hold = waitpid(pid_c, &state2, WNOHANG);

       if(pb_hold != 0){
            pid_b = 0;
            if(cause != 0){
                if(!isDone){
                    fprintf(log, "Copia de modelos de examen, finalizada.\n");
                    isDone = true;
                }
            }
            else{
                fprintf(log, "Copia de modelos de examen, ha tenido un error\n");
                return EXIT_FAILURE;
            }
        }
        if(pc_hold != 0){
            pid_c = 0;
            if(cause2 != 0){
                if(!isDone2){
                    fprintf(log, "Creación de archivos con nota necesaria para alcanzar la nota de corte, finalizada.\n");
                    average_mark = getAverageMark(pid_c, pipe2);
                    fprintf(log, "La nota media de la clase es: %0.2f .\n", average_mark);
                    isDone2 = true;
                }
            }
            else{
                fprintf(log, "Creación de archivos con nota necesaria para alcanzar la nota de corte, ha tenido un error.\n");
                return EXIT_FAILURE;
            }
        }
   }while(pb_hold == 0 || pc_hold == 0);

   return EXIT_SUCCESS;
}

double getAverageMark(pid_t pid, int *pipe){
    /* Method that gets the average mark from a pipe shared with PC */

    char readbuffer [50], *ptr;
    close(pipe[1]);
    double average_mark = 0.0;
    read(pipe[0], readbuffer, sizeof(readbuffer));

    average_mark = strtod(readbuffer, &ptr);
    return average_mark;
}

void installSignalHandler() {
    /*Method that installas a signal handler and assigns it to a function */

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "Error installing signal handler.\n");   
        exit(EXIT_FAILURE);
    }
}

void signal_handler(int signo) {
    /* Method that kills all active processes and creates process PD when SIGINT */

    pid_t pid;
    int state;
    if(pid_a != 0)
        kill(pid_a, SIGINT);
    if(pid_b != 0)
        kill(pid_b, SIGINT);
    if(pid_c != 0)
        kill(pid_c, SIGINT);
    fprintf(log, "Señal Ctrl + C recibida, el programa ha sido parado. \n");
    fflush(log);
    fclose(log);
    pid = createProcess("./exec/pd");
    waitpid(pid, &state, 0);
    exit(EXIT_SUCCESS);
}