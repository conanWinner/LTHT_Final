#include <stdio.h>
#include <stdbool.h>

#define MAX 4  // Tổng cộng 2 tiến trình và 2 tài nguyên

int graph[MAX][MAX];  // Ma trận biểu diễn đồ thị RAG (Resource Allocation Graph)
bool visited[MAX];    // Đánh dấu các nút đã được thăm
bool recStack[MAX];   // Ngăn xếp đệ quy để phát hiện chu trình

// Hàm thêm cạnh vào đồ thị
void addEdge(int from, int to) {
    graph[from][to] = 1;
}

// Hàm kiểm tra chu trình sử dụng DFS
bool isCyclicUtil(int node, int n) {
    if (!visited[node]) {
        visited[node] = true;
        recStack[node] = true;

        for (int neighbor = 0; neighbor < n; neighbor++) {
            if (graph[node][neighbor]) {  // Có cạnh nối
                if (!visited[neighbor] && isCyclicUtil(neighbor, n))
                    return true;
                else if (recStack[neighbor])
                    return true;  // Phát hiện chu trình
            }
        }
    }
    recStack[node] = false;  // Loại khỏi ngăn xếp đệ quy
    return false;
}

// Hàm phát hiện deadlock
bool isCyclic(int n) {
    for (int i = 0; i < n; i++) {
        if (!visited[i] && isCyclicUtil(i, n))
            return true;
    }
    return false;
}

int main() {
    // Đặt tên rõ ràng cho các đỉnh
    enum { P1 = 0, R1 = 1, P2 = 2, R2 = 3 };
    int n = 4;  // Số lượng nút (2 tiến trình + 2 tài nguyên)

    // Khởi tạo ma trận đồ thị với tất cả phần tử bằng 0
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
    }

    // Thêm các cạnh: P1 yêu cầu R1, R1 cấp cho P2, P2 yêu cầu R2, R2 cấp cho P1 (Chu trình)
    addEdge(P1, R1);  // P1 -> R1
    addEdge(R1, P2);  // R1 -> P2
    addEdge(P2, R2);  // P2 -> R2
    // addEdge(R2, P1);  // R2 -> P1 (Tạo chu trình)

    // Kiểm tra deadlock
    if (isCyclic(n))
        printf("Deadlock detected!\n");
    else
        printf("No deadlock detected.\n");

    return 0;
}
