#ifndef RWExec_h
#define RWExec_h

#define PTR_ERROR NULL
#define INT_ERROR -1
#define OK 0
#define SIZE_OF_BUFFER 9

/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

/***************************************************************************/

char* bufferedRead(const int fileDes);
int bufferedWrite(const int fileDes, const char* data);

/***************************************************************************/

int bufferedWrite(const int fileDes, const char* data)
{
    assert(fileDes >= 0);
    assert(data);
    
    int nBytes = SIZE_OF_BUFFER;
    int ptrShift = 0;
    char buffer[SIZE_OF_BUFFER + 1] = "";
    
    while (nBytes == SIZE_OF_BUFFER)
    {
        strncpy(buffer, &data[ptrShift], SIZE_OF_BUFFER);
     
        nBytes = write(fileDes, buffer, strlen(buffer));
        if (nBytes == INT_ERROR)
        {
            perror("! WRITE ERROR");
            return INT_ERROR;
        }
        ptrShift += nBytes;
    }
    
    return OK;
}

//===========================================================================

char* bufferedRead(const int fileDes)
{
    assert(fileDes >= 0);
    
    int nBytes = SIZE_OF_BUFFER;
    char buffer[SIZE_OF_BUFFER] = "";
    char* fileContent = (char*)calloc(1, sizeof(char));
    
    while (nBytes == SIZE_OF_BUFFER)
    {
        nBytes = read(fileDes, buffer, SIZE_OF_BUFFER);
        if (nBytes == INT_ERROR)
        {
            perror("! READ ERROR");
            return PTR_ERROR;
        }
        
        realloc(fileContent, strlen(fileContent) + nBytes);
        strncat(fileContent, buffer, nBytes);
    }
    
    return fileContent;
}

#endif /* RWExec_h */
