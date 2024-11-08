// banker_logic.c
#include "banker_logic.h"

bool isSafe(int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES],
            int need[][MAX_RESOURCES], int n, int m, FILE *outputFile)
{
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};
    int count = 0;

    for (int i = 0; i < m; i++)
    {
        work[i] = available[i];
    }

    while (count < n)
    {
        bool found = false;

        for (int p = 0; p < n; p++)
        {
            if (!finish[p])
            {
                int j;
                for (j = 0; j < m; j++)
                {
                    if (need[p][j] > work[j])
                        break;
                }

                if (j == m)
                {
                    for (int k = 0; k < m; k++)
                    {
                        work[k] += allocation[p][k];
                    }
                    finish[p] = true;
                    fprintf(outputFile, "%d ", p);
                    printf("%d ", p);
                    count++;
                    found = true;
                }
            }
        }

        if (!found)
        {
            fprintf(outputFile, "\nHệ thống không an toàn.\n");
            printf("\nHệ thống không an toàn.\n");
            return false;
        }
    }

    fprintf(outputFile, "\nHệ thống an toàn.\n");
    printf("\nHệ thống an toàn.\n");
    return true;
}

void createOutputFilename(char *filename)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(filename, 20, "%d%m%y%H%M%S.txt", tm_info);
}

bool inputData(const char *filename, int *n, int *m,
               int allocation[MAX_PROCESSES][MAX_RESOURCES],
               int max[MAX_PROCESSES][MAX_RESOURCES],
               int available[MAX_RESOURCES])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Không thể mở file %s\n", filename);
        return false;
    }

    fscanf(file, "%d %d", n, m);

    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *m; j++)
            fscanf(file, "%d", &allocation[i][j]);

    for (int i = 0; i < *n; i++)
        for (int j = 0; j < *m; j++)
            fscanf(file, "%d", &max[i][j]);

    for (int i = 0; i < *m; i++)
        fscanf(file, "%d", &available[i]);

    fclose(file);
    return true;
}

void cal_need(int max[][MAX_RESOURCES],
              int allocation[][MAX_RESOURCES],
              int need[][MAX_RESOURCES], int n, int m)
{

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}
