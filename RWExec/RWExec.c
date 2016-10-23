// clang RWExec.c -o RWExec && ./RWExec Test L.txt

#include "Buffer.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

/***************************************************************************/

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("! ERROR: Need more arguments\n");
        return INT_ERROR;
    }
    char* pathForExec = argv[1];
    char* fileName = argv[2];
    
    umask(0);
    int check = OK;
    
    close(1);
    int fileDes = open(fileName, O_RDWR | O_CREAT, 0777);
    if (fileDes == INT_ERROR)
    {
        perror("! OPEN ERROR");
        return INT_ERROR;
    }

    check = bufferedWrite(fileDes, "List of files:\n");
    if (check == INT_ERROR)
    {
        perror("! ERROR: function 'bufferedWrite' returned INT_ERROR\n");
        close(fileDes);
        return INT_ERROR;
    }
    
    //dup2(fileDes, 1);
    //close(fileDes);
    
    if (execl("/bin/ls", "ls", pathForExec, NULL) == INT_ERROR)
    {
        perror("! EXECL ERROR");
        return INT_ERROR;
    }
    
    return OK;
}