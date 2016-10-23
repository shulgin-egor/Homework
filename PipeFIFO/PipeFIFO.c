// clang PipeFIFO.c -o PipeFifo && ./PipeFIFO Test fifofile

#include "Buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/***************************************************************************/

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("! ERROR: Need more arguments\n");
        return INT_ERROR;
    }
    char* pathForLs = argv[1];
    char* fifoFileName = argv[2];
    
    umask (0);
    int check = OK;
    
    //close(1);
    int fileDes[2] = {0, 0};           // [0] - output, [1] - input
    check = pipe(fileDes);
    if (check == INT_ERROR)
    {
        perror("! PIPE ERROR");
        return INT_ERROR;
    }
    
    pid_t pid = fork();
    
    if (pid == 0)                                   // child
    {
        char* fileContent = bufferedRead(fileDes[0]);
        if (fileContent == PTR_ERROR)
        {
            perror("! ERROR: function 'bufferedRead' returned NULL\n");
            close(fileDes[0]);
            return INT_ERROR;
        }
        close(fileDes[0]);
        
        int check = mkfifo(fifoFileName, 0777);
        if (check == INT_ERROR)
        {
            perror("! MKFIFO ERROR");
            free(fileContent);
            return INT_ERROR;
        }
        
        int fifoFileDes = open(fifoFileName, O_WRONLY);
        if (fifoFileDes == INT_ERROR)
        {
            perror("! OPEN ERROR");
            free(fileContent);
            return INT_ERROR;
        }
        
        check = bufferedWrite(fifoFileDes, fileContent);
        if (check == INT_ERROR)
        {
            perror("! ERROR: function 'bufferedWrite' returned INT_ERROR\n");
            close(fifoFileDes);
            free(fileContent);
            return INT_ERROR;
        }
        free(fileContent);
        close(fifoFileDes);
    }
    else if (pid > 0)                              // parent
    {
        dup2(fileDes[1], 1);
        close(fileDes[1]);

        if (execl("/bin/ls", "ls", pathForLs, NULL) == INT_ERROR)
        {
            perror("! EXECL ERROR");
            return INT_ERROR;
        }
        
    }
    else
    {
        perror("! FORK ERROR");
        return INT_ERROR;
    }
    
    return OK;
}