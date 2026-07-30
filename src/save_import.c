#include "gui.h"

static void on_save_file_finish(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
    t_app_context *ctx = (t_app_context *)user_data;
    GError *error = NULL;

    // Παίρνουμε το αρχείο που επέλεξε/έγραψε ο χρήστης
    GFile *file = gtk_file_dialog_save_finish(dialog, res, &error);

    if (file != NULL)
    {
        // Μετατροπή του GFile σε string μονοπάτι (π.χ. "/Users/USER/Desktop/koufoma.win")
        char *path = g_file_get_path(file);

        if (path)
        {
            save_window_to_win(path, ctx);
            g_print("Project saved successfully to: %s\n", path);
            g_free(path);
        }
        g_object_unref(file);
    }
    else
    {
        if (error)
        {
            g_print("Save cancelled or error: %s\n", error->message);
            g_clear_error(&error);
        }
    }
}

void on_save_project_clicked(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;

    // 1. Δημιουργία του File Dialog
    GtkFileDialog *dialog = gtk_file_dialog_new();
    
    // 2. Τίτλος παραθύρου & Προτεινόμενο όνομα
    gtk_file_dialog_set_title(dialog, "Save Window Project");
    gtk_file_dialog_set_initial_name(dialog, "my_window.win");

    // 3. Εμφάνιση του Native Save Dialog
    gtk_file_dialog_save(
        dialog,
        GTK_WINDOW(ctx->main_window), // Το κύριο παράθυρο
        NULL,                          // GCancellable
        on_save_file_finish,           // Callback όταν τελειώσει
        ctx                            // user_data
    );

    // Το dialog ελευθερώνεται αυτόματα μετά τη χρήση
    g_object_unref(dialog);
}


static void on_import_response(GObject *source, GAsyncResult *res, gpointer user_data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    t_app_context *ctx = (t_app_context *)user_data;
    GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

    if (file)
	{
        char *path = g_file_get_path(file);
        if (load_project_from_win(path, ctx))
		{
            /* Synchronize UI elements with loaded state */
            gtk_spin_button_set_value(ctx->spin_width, ctx->win.width);
            gtk_spin_button_set_value(ctx->spin_height, ctx->win.height);
            gtk_drop_down_set_selected(ctx->drop_direction, ctx->win.opening_dir);
            gtk_drop_down_set_selected(ctx->drop_mechanism, ctx->win.mechanism);

            calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);
            gtk_widget_queue_draw(ctx->area);
        }
        g_free(path);
        g_object_unref(file);
    }
}

void on_import_clicked(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;
    GtkFileDialog *dialog = gtk_file_dialog_new();
    
    gtk_file_dialog_set_title(dialog, "Import Window Project");
    
    /* File Filter Setup */
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Window Project (*.win)");
    gtk_file_filter_add_pattern(filter, "*.win");
    
    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, filter);
    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));

    gtk_file_dialog_open(dialog, GTK_WINDOW(ctx->main_window), NULL, on_import_response, ctx);
    g_object_unref(dialog);
}
