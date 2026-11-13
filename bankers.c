#include <stdio.h>
#include <stdbool.h>

int main() {
    int n, m;

    // 1. Get the number of processes and resource types
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    printf("Enter the number of resource types: ");
    scanf("%d", &m);

    int alloc[n][m], max[n][m], need[n][m], avail[m];

    // 2. Input the Allocation Matrix
    printf("\nEnter the Allocation Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &alloc[i][j]);
        }
    }

    // 3. Input the Max Matrix
    printf("\nEnter the Max Matrix:\n");
    for (int i = 0; i < n; i++) {
        printf("Process P%d: ", i);
        for (int j = 0; j < m; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // 4. Input the Available Resources Vector
    printf("\nEnter the Available Resources:\n");
    for (int i = 0; i < m; i++) {
        scanf("%d", &avail[i]);
    }

    // 5. Calculate the Need Matrix (Need = Max - Allocation)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    // 6. Safety Algorithm Tracking Arrays
    int work[m];
    bool finish[n];
    int safe_sequence[n];
    int count = 0;

    // Initialize Work array with Available resources, and Finish array to false
    for (int i = 0; i < m; i++) {
        work[i] = avail[i];
    }
    for (int i = 0; i < n; i++) {
        finish[i] = false;
    }

    // 7. Find a safe sequence
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (finish[i] == false) {
                bool can_allocate = true;
                // Check if Need <= Work for all resources
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        can_allocate = false;
                        break;
                    }
                }

                // If resources can be allocated, simulate process completion
                if (can_allocate) {
                    safe_sequence[count++] = i;
                    finish[i] = true;
                    // Release allocated resources back to the pool
                    for (int j = 0; j < m; j++) {
                        work[j] += alloc[i][j];
                    }
                }
            }
        }
    }

    // 8. Output results based on system safety state
    bool flag = true;
    for (int i = 0; i < n; i++) {
        if (finish[i] == false) {
            flag = false;
            printf("\nSystem is in an UNSAFE state! (Deadlock detected or potential deadlock ahead)\n");
            break;
        }
    }

    if (flag) {
        printf("\nSystem is in a SAFE state.\nFollowing is the Safe Sequence: ");
        for (int i = 0; i < n - 1; i++) {
            printf("P%d -> ", safe_sequence[i]);
        }
        printf("P%d\n", safe_sequence[n - 1]);
    }

    return 0;
}