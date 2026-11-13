#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int n;
    puts("Server");
    char strMessage[5000];

    int fd = open("fifo6.txt", O_WRONLY);
    int fd2 = open("fifo7.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("cannot open fifo6");
        return EXIT_FAILURE;
    }
    if (fd2 == -1)
    {
        perror("cannot open fifo7");
        return EXIT_FAILURE;
    }

    puts("FIFO OPEN");

    char stringBuffer[5000];
    memset(stringBuffer, 0, 5000);
    int res;
    char Len;

    {
        printf("\n\n\t\tEnter the Message to be passed: ");
        fgets(strMessage, 100, stdin);
        char L = (char) strlen(strMessage);
        
        write(fd, &L, 1);
        write(fd, strMessage, strlen(strMessage));
        
        strMessage[0] = 0; // Resetting the character array

        int len2;
        res = read(fd2, &len2, 1);
        
        // Clear buffer before reading response
        memset(stringBuffer, 0, 5000);
        read(fd2, stringBuffer, 5000); // Read String Characters

        // Null-terminate *before* printing
        stringBuffer[(int)len2] = 0;
        printf("\nServer Received: %s\n", stringBuffer);
    }

    close(fd);
    close(fd2);
    return 0;
}