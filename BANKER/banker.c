#include "banker_logic.h"
#include <gtk/gtk.h>
#include <cairo.h>
#include <string.h>

// Tạo bảng số liệu cho mảng 2D
GtkWidget *create_data_grid(int data[][MAX_RESOURCES], int rows, int cols, const char *title)
{
    GtkWidget *grid = gtk_grid_new();
    GtkWidget *label = gtk_label_new(title);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, cols + 1, 1); // Tiêu đề

    // Thêm các ô của bảng
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%d", data[i][j]);
            GtkWidget *cell = gtk_label_new(buffer);
            gtk_grid_attach(GTK_GRID(grid), cell, j + 1, i + 1, 1, 1);
        }
    }

    return grid;
}

// // Sự kiện chọn file
void on_file_selected(GtkWidget *widget, gpointer data)
{
    GtkWidget *vbox = GTK_WIDGET(data);
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
    char *filename = gtk_file_chooser_get_filename(chooser);

    if (filename)
    {
        strncpy(inputFilename, filename, sizeof(inputFilename) - 1);
        inputGraph(inputFilename); // Đọc dữ liệu từ file

        bool hasDeadlock = checkDeadlock(); // Kiểm tra deadlock

        // Tạo tên file đầu ra và ghi kết quả
        char outputFilename[20];
        createOutputFilename(outputFilename);

        char path[100] = "/home/conanwinner/Banker/Output_BANKER";
        strcat(path, outputFilename);
        writeOutput(path, hasDeadlock);

        // Xóa các widget cũ trong vbox để cập nhật bảng
        GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));
        for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
        {
            gtk_widget_destroy(GTK_WIDGET(iter->data));
        }
        g_list_free(children);

        // Tạo và thêm bảng số liệu `allocation` và `max`
        GtkWidget *allocationGrid = create_data_grid(allocation, numProcesses, numResources, "Allocation");
        GtkWidget *maxGrid = create_data_grid(max, numProcesses, numResources, "Max");

        gtk_box_pack_start(GTK_BOX(vbox), allocationGrid, FALSE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(vbox), maxGrid, FALSE, FALSE, 5);

        gtk_widget_show_all(vbox); // Hiển thị tất cả các widget mới
        g_free(filename);
    }
}

int main()
{
    int n, m;
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
        max[MAX_PROCESSES][MAX_RESOURCES],
        available[MAX_RESOURCES],
        need[MAX_PROCESSES][MAX_RESOURCES];

    // Đọc từ file => gán dữ liệu
    if (!inputData("input1.txt", &n, &m, allocation, max, available))
    {
        return 1;
    }
    // Tính Need
    cal_need(max, allocation, need, n, m);

    // Tao file
    char outputFilename[20];
    createOutputFilename(outputFilename);

    FILE *outputFile = fopen(outputFilename, "w");
    if (!outputFile)
    {
        printf("Không thể tạo file %s\n", outputFilename);
        return 1;
    }

    // Kiểm tra có an toàn không
    isSafe(available, max, allocation, need, n, m, outputFile);

    fclose(outputFile);

    // gtk_init(&argc, &argv);

    // GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // gtk_window_set_title(GTK_WINDOW(window), "Thuật toán Banker");
    // gtk_window_set_default_size(GTK_WINDOW(window), 600, 800);
    // gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // GtkWidget *drawing_area = gtk_drawing_area_new();
    // GtkWidget *button = gtk_file_chooser_button_new("Chọn file đầu vào", GTK_FILE_CHOOSER_ACTION_OPEN);

    // GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Chứa các bảng số liệu
    // gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    // gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);

    // gtk_container_add(GTK_CONTAINER(window), vbox);

    // g_signal_connect(button, "file-set", G_CALLBACK(on_file_selected), vbox);
    // g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // g_signal_connect(drawing_area, "draw", G_CALLBACK(draw_graph), NULL);

    // gtk_widget_show_all(window);
    // gtk_main();

    return 0;
}
