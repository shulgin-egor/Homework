// clang RWExec_read.c -o RWExec_read && ./RWExec_read L.txt

#define STANDART_OUTPUT_FD 1

/***************************************************************************/

#include "Buffer.h"

#include <fcntl.h>
#include <sys/stat.h>

/***************************************************************************/

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("! ERROR: Need more arguments\n");
        return INT_ERROR;
    }
    char* fileName = argv[1];
    
    umask(0);
    int check = OK;
    
    int fileDes = open(fileName, O_RDONLY);
    if (fileDes == INT_ERROR)
    {
        perror("! OPEN ERROR");
        return INT_ERROR;
    }
    
    char* fileContent = bufferedRead(fileDes);
    if (fileContent == PTR_ERROR)
    {
        perror("! ERROR: function 'bufferedRead' returned NULL\n");
        close(fileDes);
        return INT_ERROR;
    }
    close(fileDes);
    
    check = bufferedWrite(STANDART_OUTPUT_FD, fileContent);
    if (check == INT_ERROR)
    {
        perror("! ERROR: function 'bufferedWrite' returned INT_ERROR\n");
        close(fileDes);
        return INT_ERROR;
    }
    free(fileContent);
    
    if (execl("/bin/rm", "rm", fileName, NULL) == INT_ERROR)
    {
        perror("! EXECL ERROR");
        return INT_ERROR;
    }
    
    return OK;
}
