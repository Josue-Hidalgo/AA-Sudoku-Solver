#include <gtk/gtk.h>
#include <string.h>
#include <ctype.h>

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

// Función para crear y configurar entries en el grid principal del Sudoku (9x9)
void setup_sudoku_grid(GtkBuilder *builder) {
    // Obtener el grid principal
    GtkWidget *main_grid = GTK_WIDGET(gtk_builder_get_object(builder, "IDGrid"));
    if (!main_grid || !GTK_IS_GRID(main_grid)) {
        printf("Error: No se pudo obtener el grid principal IDGrid\n");
        return;
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
        }
    }
    
    printf("Grid principal configurado con 81 entries (9x9)\n");
}

// Función para manejar el cierre de la ventana
void on_window_destroy(GtkWidget *widget, gpointer data) {
    // Marcar parámetros como no utilizados para evitar warnings
    (void)widget;
    (void)data;
    gtk_main_quit();
}

int main(int argc, char *argv[]) {

    // Declaraciones de variables (son necesarias para GTK)
    GtkBuilder *builder;
    GtkWidget *window;
    GError *error = NULL;
    
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
        printf("CSS cargado exitosamente\n");
    } else {
        printf("Error cargando CSS: %s\n", error->message);
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
        printf("Error cargando archivo Glade: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }
    
    // Obtener la ventana principal del archivo Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "IDWindow"));
    if (!window) {
        printf("Error: No se pudo encontrar la ventana 'IDWindow' en el archivo Glade\n");
        return 1;
    }
    
    // Configurar el grid con entries para el Sudoku
    setup_sudoku_grid(builder);
    
    // Conectar la señal de cierre de ventana
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), NULL);
    
    // Conectar todas las señales automáticamente (si las hay definidas en el Glade)
    gtk_builder_connect_signals(builder, NULL);
    
    // Liberar el builder
    g_object_unref(builder);
    
    // Mostrar la ventana
    gtk_widget_show_all(window);
    
    // Iniciar el bucle principal de GTK
    gtk_main();
    
    return 0;
}