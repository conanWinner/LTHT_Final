#include "graph_logic.h"
#include <time.h>

bool visited[MAX_PROCESSES + MAX_RESOURCES];
bool recStack[MAX_PROCESSES + MAX_RESOURCES];
int graph[MAX_PROCESSES + MAX_RESOURCES][MAX_PROCESSES + MAX_RESOURCES];
int numProcesses;
int numResources;

// Hàm đọc dữ liệu từ file
void inputGraph(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Không thể mở file");
        exit(EXIT_FAILURE);
    }

    // Đọc số lượng tiến trình và tài nguyên
    fscanf(file, "%d %d", &numProcesses, &numResources);

    // Đọc ma trận biểu diễn các cạnh
    for (int i = 0; i < numProcesses + numResources; i++) {
        for (int j = 0; j < numProcesses + numResources; j++) {
            fscanf(file, "%d", &graph[i][j]);
        }
    }
    fclose(file);
}


// Hàm kiểm tra chu kỳ (cycle detection) trong đồ thị
bool isCyclic(int node) {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;

        // Duyệt qua các đỉnh kề của đỉnh node
        for (int i = 0; i < numProcesses + numResources; i++) {
            if (graph[node][i]) {
                if (!visited[i] && isCyclic(i)) {
                    return true;
                } else if (recStack[i]) {
                    return true;
                }
            }
        }
    }
    recStack[node] = false;
    return false;
}

// Hàm kiểm tra deadlock
bool checkDeadlock() {
    for (int i = 0; i < numProcesses + numResources; i++) {
        visited[i] = false;
        recStack[i] = false;
    }

    // Duyệt qua tất cả các đỉnh để phát hiện chu kỳ
    for (int i = 0; i < numProcesses + numResources; i++) {
        if (isCyclic(i)) {
            return true;
        }
    }
    return false;
}


// Hàm tạo tên file output dựa trên thời gian hiện tại
void createOutputFilename(char *filename) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Tạo tên file với định dạng ddmmyyhhmm.txt
    strftime(filename, 20, "%d%m%y%H%M%S.txt", tm_info);
}


// Hàm ghi kết quả vào file
void writeOutput(const char *filename, bool hasDeadlock) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Không thể tạo file %s\n", filename);
        return;
    }

    if (hasDeadlock) {
        fprintf(file, "Có deadlock trong hệ thống!\n");
        printf("Có deadlock trong hệ thống!\n");
    } else {
        fprintf(file, "Không có deadlock trong hệ thống.\n");
        printf("Không có deadlock trong hệ thống.\n");
    }

    fclose(file);
}

