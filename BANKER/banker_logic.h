#ifndef BANKER_LOGIC_H
#define BANKER_LOGIC_H

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Khai báo các hàm
bool isSafe(int available[], int max[][MAX_RESOURCES], int allocation[][MAX_RESOURCES],
            int need[][MAX_RESOURCES], int n, int m, FILE *outputFile);

void createOutputFilename(char *filename);

bool inputData(const char *filename, int *n, int *m,
               int allocation[MAX_PROCESSES][MAX_RESOURCES],
               int max[MAX_PROCESSES][MAX_RESOURCES],
               int available[MAX_RESOURCES]);

void cal_need(int allocation[MAX_PROCESSES][MAX_RESOURCES],
              int max[MAX_PROCESSES][MAX_RESOURCES],
              int need[MAX_PROCESSES][MAX_RESOURCES], int n, int m);

#endif
