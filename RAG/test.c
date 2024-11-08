#include <gtk/gtk.h>
#include <cairo.h>
#include "graph_logic.h"
#include <string.h>  // Để dùng strcat

// Tọa độ các tiến trình và tài nguyên
int process_positions[MAX_PROCESSES][2]; //(x, y)
int resource_positions[MAX_RESOURCES][2];
char inputFilename[256];  


// Tính toán vị trí các tiến trình và tài nguyên
void calculate_positions() {
    for (int i = 0; i < numProcesses; i++) {
        process_positions[i][0] = 70;               // x
        process_positions[i][1] = 100 + i * 120;    // y
    }
    for (int i = 0; i < numResources; i++) {
        resource_positions[i][0] = 350;             // x
        resource_positions[i][1] = 100 + i * 120;   // y
    }
}

// Vẽ đồ thị
static void draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Tiến trình
    for (int i = 0; i < numProcesses; i++) {
        cairo_set_source_rgb(cr, 0.2, 0.6, 0.8); // Màu xanh biển
        cairo_arc(cr, process_positions[i][0], process_positions[i][1], 30, 0, 2 * 3.14);
        cairo_fill(cr);

        // Viết chữ
        cairo_move_to(cr, process_positions[i][0] - 10, process_positions[i][1] + 5);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_show_text(cr, g_strdup_printf("P%d", i));
    }

    // Tài nguyên
    for (int i = 0; i < numResources; i++) {
        cairo_set_source_rgb(cr, 0.4, 0.8, 0.4); // Màu xanh lá
        cairo_rectangle(cr, resource_positions[i][0] - 30, resource_positions[i][1] - 30, 60, 60);
        cairo_fill(cr);

        // Viết chữ
        cairo_move_to(cr, resource_positions[i][0] - 10, resource_positions[i][1] + 5);
        cairo_set_source_rgb(cr, 1, 1, 1);
        cairo_show_text(cr, g_strdup_printf("R%d", i));
    }

    // Vẽ các cạnh phụ thuộc từ tiến trình đến tài nguyên (Màu đỏ)
    cairo_set_source_rgb(cr, 1, 0, 0);
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            if (graph[i][j + numProcesses] == 1) {
                double controlX = (process_positions[i][0] + resource_positions[j][0]) / 2;
                double controlY = (process_positions[i][1] + resource_positions[j][1]) / 2 - 50;

                cairo_move_to(cr, process_positions[i][0], process_positions[i][1]);
                cairo_curve_to(cr, controlX, controlY, controlX, controlY, resource_positions[j][0], resource_positions[j][1]);
                cairo_stroke(cr);
            }
        }
    }

    // Vẽ các cạnh từ tài nguyên đến tiến trình (Màu xanh)
    cairo_set_source_rgb(cr, 0, 0, 1);
    for (int j = 0; j < numResources; j++) {
        for (int i = 0; i < numProcesses; i++) {
            if (graph[j + numProcesses][i] == 1) {
                double controlX = (resource_positions[j][0] + process_positions[i][0]) / 2;
                double controlY = (resource_positions[j][1] + process_positions[i][1]) / 2 - 50;

                cairo_move_to(cr, resource_positions[j][0], resource_positions[j][1]);
                cairo_curve_to(cr, controlX, controlY, controlX, controlY, process_positions[i][0], process_positions[i][1]);
                cairo_stroke(cr);
            }
        }
    }
}

// Xử lý sự kiện khi người dùng chọn file upload
void on_file_selected(GtkWidget *widget, gpointer data) {
    GtkFileChooser *chooser = GTK_FILE_CHOOSER(widget);
    char *filename = gtk_file_chooser_get_filename(chooser);

    if (filename) {
        strncpy(inputFilename, filename, sizeof(inputFilename) - 1); // Lưu tên file vào biến toàn cục
        inputGraph(inputFilename);  // Đọc dữ liệu từ file

        bool hasDeadlock = checkDeadlock();

        char outputFilename[20];
        createOutputFilename(outputFilename);

        char path[100] = "/home/conanwinner/RAG/Output/";
        strcat(path, outputFilename);

        writeOutput(path, hasDeadlock);

        calculate_positions(); // Tính toán vị trí
        gtk_widget_queue_draw(GTK_WIDGET(data));  // Vẽ lại giao diện
        g_free(filename);
    }
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Tạo cửa sổ chính
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Đồ thị phân bổ tài nguyên");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 800);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Khu vực vẽ đồ thị
    GtkWidget *drawing_area = gtk_drawing_area_new();

    GtkWidget *button = gtk_file_chooser_button_new("Chọn file đầu vào", GTK_FILE_CHOOSER_ACTION_OPEN);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 5);

    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Kết nối sự kiện
    g_signal_connect(G_OBJECT(button), "file-set", G_CALLBACK(on_file_selected), drawing_area);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_graph), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
