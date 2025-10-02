#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "logic.h"

GtkWidget ***entries = NULL;
Sudoku current_sudoku;
int solving = 0; // dice si el programa está resolviendo un sudoku
int delay_ms = 50;

static GtkWidget *IDTimeCounter;
static guint timer_id = 0;
static int elapsed_seconds = 0;

void update_interface() {
    while (gtk_events_pending())
        gtk_main_iteration();
    
    if (delay_ms > 0)
        usleep(delay_ms * 200);
}

void update_cell_ui(int row, int col, int value) {
    if (entries && entries[row] && entries[row][col]) {
        if (value == 0) {
            gtk_entry_set_text(GTK_ENTRY(entries[row][col]), "");
        } else {
            char text[2];
            snprintf(text, sizeof(text), "%d", value);
            gtk_entry_set_text(GTK_ENTRY(entries[row][col]), text);
        }
        update_interface();
    }
}

int solve_sudoku_visual(Sudoku *s) {
    if (!solving) return 0;
    
    Cell current;
    
    if (!findEmptyCell(s, &current))
        return 1;
    
    int row = current.row;
    int col = current.col;
    
    for (int num = 1; num <= 9 && solving; num++) {
        if (promising(s, row, col, num)) {
            s->sudoku[row][col] = num;
            update_cell_ui(row, col, num);
            
            if (solve_sudoku_visual(s))
                return 1;
            
            s->sudoku[row][col] = 0;
            update_cell_ui(row, col, 0);
        }
    }
    
    return 0;
}

static gboolean timerCallback() {
    if (solving) {
        elapsed_seconds++;
        int minutes = elapsed_seconds / 60;
        int seconds = elapsed_seconds % 60;
        char time_str[16];
        snprintf(time_str, sizeof(time_str), "%02d:%02d", minutes, seconds);
        gtk_label_set_text(GTK_LABEL(IDTimeCounter), time_str);
    }
    return G_SOURCE_CONTINUE;
}

void startTimer() {
    elapsed_seconds = 0;  // Reiniciar contador
    gtk_label_set_text(GTK_LABEL(IDTimeCounter), "00:00");
    
    // Iniciar timer si no está activo
    if (timer_id == 0)
        timer_id = g_timeout_add(1000, timerCallback, NULL);
}

void on_solve_button_clicked(GtkButton *button) {
    (void)button;
    
    if (solving) return;
    solving = 1;

    startTimer();
    
    // Obtener sudoku desde la interfaz
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            const char *text = gtk_entry_get_text(GTK_ENTRY(entries[row][col]));
            if (text && strlen(text) > 0 && isdigit(text[0]))
                current_sudoku.sudoku[row][col] = atoi(text);
            else
                current_sudoku.sudoku[row][col] = 0;
        }
    }
    
    // bloquea la UI temporalmente, cuando termina de resolverse el 
    // sudoku, hace la acción si se le dio click
    solve_sudoku_visual(&current_sudoku);
    solving = 0;
}

// Función callback para validar entrada numérica (solo 1-9)
void on_entry_insert_text(GtkEditable *editable, gchar *new_text, gint new_text_length, 
                         gint *position, gpointer user_data) {
    (void)user_data; // Evitar warning
    (void)position; // Evitar warning
    
    // Solo permitir un carácter
    if (gtk_entry_get_text_length(GTK_ENTRY(editable)) >= 1) {
        g_signal_stop_emission_by_name(editable, "insert-text");
        return;
    }
    
    // Validar que sea un número del 1 al 9
    for (int i = 0; i < new_text_length; i++) {
        if (!isdigit(new_text[i]) || new_text[i] == '0') {
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

// Función para cargar un sudoku desde archivo
void on_load_button_clicked(GtkButton *button) {
    (void)button;
    
    // Crear diálogo para abrir archivo
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Load Sudoku",
                                                   GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_FILE_CHOOSER_ACTION_OPEN,
                                                   "Cancel", GTK_RESPONSE_CANCEL,
                                                   "Load", GTK_RESPONSE_ACCEPT,
                                                   NULL);
    
    // Filtrar por archivos binarios
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Sudoku files (*.bin)");
    gtk_file_filter_add_pattern(filter, "*.bin");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    
    // Mostrar diálogo
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (result == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        
        if (cargarSudoku(filename, &current_sudoku)) {
            // Actualizar la interfaz con el sudoku cargado
            for (int row = 0; row < SIZE; row++) {
                for (int col = 0; col < SIZE; col++) {
                    if (current_sudoku.sudoku[row][col] != 0) {
                        char text[2];
                        snprintf(text, sizeof(text), "%d", current_sudoku.sudoku[row][col]);
                        gtk_entry_set_text(GTK_ENTRY(entries[row][col]), text);
                    } else {
                        gtk_entry_set_text(GTK_ENTRY(entries[row][col]), "");
                    }
                }
            }
        } else {
            // Mostrar mensaje de error
            GtkWidget *error_dialog = gtk_message_dialog_new(
                GTK_WINDOW(dialog),
                GTK_DIALOG_MODAL,
                GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK,
                "Error opening the file: %s", filename);
            gtk_widget_destroy(error_dialog);
        }
        
        g_free(filename);
    }
    
    gtk_widget_destroy(dialog);
}

// Función para guardar un sudoku a archivo
void on_save_button_clicked(GtkButton *button, gpointer user_data) {
    (void)button;
    (void)user_data;
    
    // Primero obtener el sudoku actual desde la interfaz
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            const char *text = gtk_entry_get_text(GTK_ENTRY(entries[row][col]));
            if (text && strlen(text) > 0 && isdigit(text[0])) {
                current_sudoku.sudoku[row][col] = atoi(text);
            } else {
                current_sudoku.sudoku[row][col] = 0;
            }
        }
    }
    
    // Crear diálogo para guardar archivo
    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save Sudoku",
                                                   GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(button))),
                                                   GTK_FILE_CHOOSER_ACTION_SAVE,
                                                   "Cancel", GTK_RESPONSE_CANCEL,
                                                   "Save", GTK_RESPONSE_ACCEPT,
                                                   NULL);
    
    // Sugerir nombre por defecto
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "sudoku.bin");
    
    // Filtrar por archivos binarios
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Sudoku files (*.bin)");
    gtk_file_filter_add_pattern(filter, "*.bin");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
    
    // Mostrar diálogo
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    if (result == GTK_RESPONSE_ACCEPT) {
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        
        // Asegurar extensión .bin
        if (!g_str_has_suffix(filename, ".bin")) {
            char *new_filename = g_strdup_printf("%s.bin", filename);
            g_free(filename);
            filename = new_filename;
        }
        
        if (!guardarSudoku(filename, &current_sudoku)) {
        // Mostrar mensaje de error
        GtkWidget *error_dialog = gtk_message_dialog_new(
            GTK_WINDOW(dialog),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Error saving file: %s", filename);
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        }
        
        g_free(filename);
    }
    
    gtk_widget_destroy(dialog);
}

// Función para crear y configurar entries en el grid principal del Sudoku (9x9)
void setup_sudoku_grid(GtkBuilder *builder) {
    // Obtener el grid principal
    GtkWidget *main_grid = GTK_WIDGET(gtk_builder_get_object(builder, "IDGrid"));
    if (!main_grid || !GTK_IS_GRID(main_grid)) {
        return;
    }
    
    // inicializar la matriz de entires
    entries = g_malloc(9 * sizeof(GtkWidget**));
    for (int i = 0; i < 9; i++) {
        entries[i] = g_malloc(9 * sizeof(GtkWidget*));
    }

    // Crear 81 entries directamente en el grid principal (9x9)
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            // Crear nuevo entry
            GtkWidget *entry = gtk_entry_new();
            
            // Configurar propiedades del entry
            gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
            gtk_entry_set_width_chars(GTK_ENTRY(entry), 1);
            gtk_widget_set_size_request(entry, 50, 50);
            gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5);
            
            // Configurar expansión
            gtk_widget_set_hexpand(entry, TRUE);
            gtk_widget_set_vexpand(entry, TRUE);
            gtk_widget_set_halign(entry, GTK_ALIGN_FILL);
            gtk_widget_set_valign(entry, GTK_ALIGN_FILL);
            
            // Conectar señal de validación
            g_signal_connect(entry, "insert-text", G_CALLBACK(on_entry_insert_text), NULL);
            
            // Aplicar clase CSS y bordes de submatriz
            GtkStyleContext *context = gtk_widget_get_style_context(entry);
            gtk_style_context_add_class(context, "sudoku-cell");
            
            // Determinar posición en la submatriz 3x3
            int pos_in_block_row = row % 3;
            int pos_in_block_col = col % 3;
            
            // Aplicar clases CSS según posición en submatriz
            if (pos_in_block_row == 0) { // Primera fila de submatriz
                gtk_style_context_add_class(context, "block-top");
            }
            if (pos_in_block_row == 2) { // Última fila de submatriz
                gtk_style_context_add_class(context, "block-bottom");
            }
            if (pos_in_block_col == 0) { // Primera columna de submatriz
                gtk_style_context_add_class(context, "block-left");
            }
            if (pos_in_block_col == 2) { // Última columna de submatriz
                gtk_style_context_add_class(context, "block-right");
            }
            
            // Agregar entry al grid principal
            gtk_grid_attach(GTK_GRID(main_grid), entry, col, row, 1, 1);
            
            // guarda la referencia al entry
            entries[row][col] = entry;
        }
    }
    //printf("Grid principal configurado con 81 entries (9x9)\n");
}

// Función para manejar el cierre de la ventana
void on_window_destroy(GtkWidget *widget, gpointer data) {
    // Marcar parámetros como no utilizados para evitar warnings
    (void)widget;
    (void)data;
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;
    // inicializar sudoku
    memset(&current_sudoku, 0, sizeof(Sudoku));
    // Inicializar GTK con los argumentos de línea de comandos
    gtk_init(&argc, &argv);
    
    // Cargar CSS para estilos personalizados
    GtkCssProvider *css_provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen(display);
    
    // Cargar el archivo CSS
    if (gtk_css_provider_load_from_path(css_provider, "ui/sudoku_styles.css", &error)) {
        gtk_style_context_add_provider_for_screen(screen, 
                                                 GTK_STYLE_PROVIDER(css_provider),
                                                 GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        //printf("CSS cargado exitosamente\n");
    } else {
        //printf("Error cargando CSS: %s\n", error->message);
        g_clear_error(&error);
    }
    
    // Liberar el provider CSS
    g_object_unref(css_provider);
    
    // Crear el builder
    builder = gtk_builder_new();
    
    // Cargar archivo Glade desde la ruta especificada
    gboolean valid = gtk_builder_add_from_file(builder, "ui/Suduku_Glade.glade", &error);
    
    // Validar si la carga fue exitosa
    if (!valid) {
        //printf("Error cargando archivo Glade: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    
    // Obtener la ventana principal del archivo Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "IDWindow"));
    if (!window) {
        //printf("Error: No se pudo encontrar la ventana 'IDWindow' en el archivo Glade\n");
        return 1;
    }
    
    // Configurar el grid con entries para el Sudoku
    setup_sudoku_grid(builder);

    GtkWidget *IDSolve = GTK_WIDGET(gtk_builder_get_object(builder, "IDSolve"));
    GtkWidget *IDSave = GTK_WIDGET(gtk_builder_get_object(builder, "IDSave"));
    GtkWidget *IDOpen = GTK_WIDGET(gtk_builder_get_object(builder, "IDOpen"));
    GtkWidget *IDExit = GTK_WIDGET(gtk_builder_get_object(builder, "IDExit"));
    IDTimeCounter = GTK_WIDGET(gtk_builder_get_object(builder, "IDTimeCounter"));

    if (IDSolve)
        g_signal_connect(IDSolve, "clicked", G_CALLBACK(on_solve_button_clicked), NULL);
    if (IDOpen)
        g_signal_connect(IDOpen, "clicked", G_CALLBACK(on_load_button_clicked), NULL);
    if (IDSave)
        g_signal_connect(IDSave, "clicked", G_CALLBACK(on_save_button_clicked), NULL);
    if (IDExit)
        g_signal_connect(IDExit, "clicked", G_CALLBACK(on_window_destroy), NULL);
    if (IDTimeCounter)
        gtk_label_set_text(GTK_LABEL(IDTimeCounter), "00:00");

    // Conectar la señal de cierre de ventana
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    
    // Conectar todas las señales automáticamente (si las hay definidas en el Glade)
    gtk_builder_connect_signals(builder, NULL);
    
    // Liberar el builder
    g_object_unref(builder);
    
    // Mostrar la ventana
    gtk_widget_show_all(window);

    timer_id = g_timeout_add(1000, timerCallback, NULL);
    
    // Iniciar el bucle principal de GTK
    gtk_main();
    
    return 0;
}