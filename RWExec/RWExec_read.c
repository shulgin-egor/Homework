//clang RWExec_read.c -o RWExec && ./RWExec Test

#include "RWExec.h"

#include <fcntl.h>
#include <sys/stat.h>

/***************************************************************************/

int main(int argc, char* argv[])
{
    
    umask(0);
    
    int fileDes = open("L.txt", O_EXCL | O_RDONLY);
    if (fileDes == INT_ERROR)
    {
        perror("! OPEN ERROR");
        return INT_ERROR;
    }
    
    char* fileContent = bufferedRead(fileDes);
    if (fileContent == PTR_ERROR)
    {
        printf("! ERROR: function 'bufferedRead' returned NULL\n");
        return INT_ERROR;
    }
    close(fileDes);
    
    printf("%s", fileContent);
    
    if (execl("/bin/rm", "rm", "L.txt", NULL) == INT_ERROR)
    {
        perror("! EXECL ERROR");
        return INT_ERROR;
    }
    
    return OK;
}
