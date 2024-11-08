#ifndef GRAPH_LOGIC_H
#define GRAPH_LOGIC_H

// Định nghĩa số tối đa của tiến trình và tài nguyên
#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Khai báo biến toàn cục
extern int graph[MAX_PROCESSES + MAX_RESOURCES][MAX_PROCESSES + MAX_RESOURCES];
extern int numProcesses;
extern int numResources;


void inputGraph(const char *filename);
bool checkDeadlock();
void createOutputFilename(char *filename);
void writeOutput(const char *filename, bool hasDeadlock);
bool isCyclic(int node);

#endif
