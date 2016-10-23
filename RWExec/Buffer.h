#ifndef Buffer_h
#define Buffer_h

#define PTR_ERROR NULL
#define INT_ERROR -1
#define OK 0
#define SIZE_OF_BUFFER 512

/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

/***************************************************************************/

char* bufferedRead(const int fileDes);
int bufferedWrite(const int fileDes, const char* data);

/***************************************************************************/

int bufferedWrite(const int fileDes, const char* data)
{
    if (fileDes < 0)
    {
        errno = EINVAL;
        return INT_ERROR;
    }
    if (data == NULL)
    {
        errno = EINVAL;
        return INT_ERROR;
    }
    
    int nBytes = SIZE_OF_BUFFER;
    int ptrShift = 0;
    char buffer[SIZE_OF_BUFFER + 1] = "";
    
    while (nBytes == SIZE_OF_BUFFER)
    {
        strncpy(buffer, &data[ptrShift], SIZE_OF_BUFFER);
     
        nBytes = write(fileDes, buffer, strlen(buffer));
        if (nBytes == INT_ERROR)
        {
            errno = EIO;
            return INT_ERROR;
        }
        ptrShift += nBytes;
    }
    
    return OK;
}

//===========================================================================

char* bufferedRead(const int fileDes)
{
    if (fileDes < 0)
    {
        errno = EINVAL;
        return PTR_ERROR;
    }
    
    int nBytes = SIZE_OF_BUFFER;
    char buffer[SIZE_OF_BUFFER] = "";
    char* fileContent = (char*)calloc(1, sizeof(char));
    
    while (nBytes == SIZE_OF_BUFFER)
    {
        nBytes = read(fileDes, buffer, SIZE_OF_BUFFER);
        if (nBytes == INT_ERROR)
        {
            errno = EIO;
            return PTR_ERROR;
        }
        realloc(fileContent, strlen(fileContent) + nBytes);
        strncat(fileContent, buffer, nBytes);
    }
    
    return fileContent;
}

#endif /* Buffer_h */
