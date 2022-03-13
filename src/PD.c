#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <ftw.h>

static int rmFiles(const char *pathname, const struct stat *buffer, int type, struct FTW *ftwb)
{
    if(remove(pathname) < 0)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
    if (nftw("./estudiantes", rmFiles,10, FTW_DEPTH|FTW_MOUNT|FTW_PHYS) < 0)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}