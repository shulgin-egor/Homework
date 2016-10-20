//clang RWExec.c -o RWExec && ./RWExec Test

#include "RWExec.h"

#include <fcntl.h>
#include <sys/stat.h>

/***************************************************************************/

int main(int argc, char* argv[])
{
    int check = OK;

    if (argc < 2)
    {
        printf("! ERROR: Need more arguments\n");
        return INT_ERROR;
    }
    
    char* path = argv[1];
    
    umask(0);
    
    int fileDes = open("./L.txt", /*O_EXCL | */O_WRONLY | O_CREAT | O_TRUNC);
    if (fileDes == INT_ERROR)
    {
        perror("! OPEN ERROR");
        return INT_ERROR;
    }

    check = bufferedWrite(fileDes, "List of files\n");
    if (check == INT_ERROR)
    {
        printf("! ERROR: function 'bufferedWrite' returned INT_ERROR\n");
        return INT_ERROR;
    }
    
    
    dup2(fileDes, 1);
    if (execl("/bin/ls", "ls", path, NULL) == INT_ERROR)
    {
        perror("! EXECL ERROR");
        return INT_ERROR;
    }
    close(fileDes);
    
    return OK;
}