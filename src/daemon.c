/* This is the implementation of daemon wwhich runs
in the background and does security copies every minute */

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <ftw.h>
#include <string.h>
#include <libgen.h>

#define MAX_STRING_SIZE 4096

static void skeleton_daemon()
{
    pid_t pid;
    
    /* Fork off the parent process */
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
     /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
    /* Catch, ignore and handle signals */
    /*TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    
    /* Fork off for the second time*/
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* Set new file permissions */
    umask(0);
    
    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
}

void create_file_path_dirs(char *file_path) {
    /* Method to create all files in a directory */

    char *dir_path = (char *) malloc(strlen(file_path) + 1);
    char *next_sep = strchr(file_path, '/');
    while (next_sep != NULL) {
        int dir_path_len = next_sep - file_path;
        memcpy(dir_path, file_path, dir_path_len);
        dir_path[dir_path_len] = '\0';
        mkdir(dir_path, S_IRWXU|S_IRWXG|S_IROTH);
        next_sep = strchr(next_sep + 1, '/');
  }
  free(dir_path);
}

static int copyFiles(const char *pathname, const struct stat *buffer, int type, struct FTW *ftwb)
    /* Method that copies files */
{
    FILE *source, *destination;
    char destFile [MAX_STRING_SIZE];
    char path [] = "../securityCopy/";
    sprintf(destFile,"%s%s",path, pathname);
    if(type != FTW_F)
        return EXIT_SUCCESS;    /* If it's a directory, we skip */
    
    create_file_path_dirs(destFile);
    if((source = fopen(pathname,"r")) == NULL)
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

int securityCopy(){
    /* Creates a security copy of the files */
    if (nftw(".", copyFiles,10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS) < 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int main(){
    /* Main method of daemon */

    skeleton_daemon();
    
    while (1)
    {
        securityCopy();
        sleep (60);
    }
    
    return EXIT_SUCCESS;
}
