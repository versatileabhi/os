#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid;
    char pathname[100];
    char buffer[1024];
    int n;

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1)
    {
        perror("pipe");
        exit(1);
    }

    // Create child process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    // ---------------- PARENT PROCESS ---------------
    if (pid > 0)
    {
        // Close unused ends
        close(pipe1[0]);   // Parent writes to pipe1
        close(pipe2[1]);   // Parent reads from pipe2

        // Enter pathname
        printf("Enter the pathname of the file: ");
        scanf("%s", pathname);

        // Write pathname to first pipe
        write(pipe1[1], pathname, strlen(pathname) + 1);

        // Close writing end
        close(pipe1[1]);

        // Read file contents from second pipe
        printf("\nContents of the file:\n");
        while ((n = read(pipe2[0], buffer, sizeof(buffer) - 1)) > 0)
        {
            buffer[n] = '\0';
            printf("%s", buffer);
        }
        close(pipe2[0]);

        // Wait for child
        wait(NULL);
    }
    // ---------------- CHILD PROCESS ---------------
    else
    {
        FILE *fp;

        // Close unused ends
        close(pipe1[1]);   // Child reads from pipe1
        close(pipe2[0]);   // Child writes to pipe2

        // Read pathname from first pipe
        read(pipe1[0], pathname, sizeof(pathname));
        close(pipe1[0]);

        // Open the file
        fp = fopen(pathname, "r");
        if (fp == NULL)
        {
            char *msg = "Error: File cannot be opened.\n";
            write(pipe2[1], msg, strlen(msg));
            close(pipe2[1]);
            exit(1);
        }

        // Read file and send contents through second pipe
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            write(pipe2[1], buffer, strlen(buffer));
        }

        fclose(fp);
        close(pipe2[1]);
        exit(0);
    }

    return 0;
}