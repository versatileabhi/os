#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int temp;
    int n;

    printf("Enter number of elements: ");
    scanf("%d", &n);

    int num[n];

    printf("Enter %d elements:\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &num[i]);
    }

    printf("\nOriginal Array:\n");

    for (int i = 0; i < n; i++)
    {
        printf("Element at index %d: %d\n", i, num[i]);
    }

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        printf("Fork failed!\n");
        exit(1);
    }

    if (pid == 0)
    {
        // Child Process
        printf("\n--- Child Process ---\n");
        printf("Hi from child\n");
        printf("Value assigned by fork to child = %d\n", pid);
        printf("My PID = %d\n", getpid());
        printf("My Parent's PID = %d\n", getppid());

        // Selection Sort - Descending Order
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (num[i] < num[j])
                {
                    temp = num[i];
                    num[i] = num[j];
                    num[j] = temp;
                }
            }
        }

        printf("Descending order: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", num[i]);
        }
        printf("\n");

        exit(0);
    }
    else
    {
        // Parent Process
        printf("\n--- Parent Process ---\n");
        printf("Hi from parent\n");
        printf("Value assigned by fork to parent = %d\n", pid);
        printf("My PID = %d\n", getpid());
        printf("My Parent's PID = %d\n", getppid());

        // Selection Sort - Ascending Order
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (num[i] > num[j])
                {
                    temp = num[i];
                    num[i] = num[j];
                    num[j] = temp;
                }
            }
        }

        printf("Ascending order: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", num[i]);
        }
        printf("\n");

        // Parent waits for child process
        wait(NULL);

        printf("Parent: Child process has completed.\n");
    }

    return 0;
}