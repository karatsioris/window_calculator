#include "gui.h"
#include "renderer.h"
#include "calculator.h"
#include "config.h"
#include "gui_cut_list.h"


void	set_margin(GtkWidget *place, int num)
{
	gtk_widget_set_margin_start(place, num);
	gtk_widget_set_margin_end(place, num);
	gtk_widget_set_margin_top(place, num);
	gtk_widget_set_margin_bottom(place, num);
}


static void on_dimension_spin_changed(GtkSpinButton *spin, gpointer user_data)
{
    (void)spin;
    t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx) return;

    // 1. Ενημέρωση των τιμών στο win από τα SpinButtons
    if (ctx->spin_width)
        ctx->win.width  = (float)gtk_spin_button_get_value(ctx->spin_width);
    if (ctx->spin_height)
        ctx->win.height = (float)gtk_spin_button_get_value(ctx->spin_height);

    // 2. Επανυπολογισμός των κοπών
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);

    // 3. Επανασχεδίαση του Cairo DrawingArea
    if (ctx->area)
        gtk_widget_queue_draw(ctx->area);

    // 4. Ενημέρωση της Λίστας Κοπών
    update_cut_list_ui(ctx); 
}

static GtkWidget *build_labeled_spinbutton(const char *label_text, float initial_value, GtkSpinButton **out_spin)
{
    GtkWidget *label = gtk_label_new(label_text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    
    GtkWidget *spin = gtk_spin_button_new_with_range(300, 3000, 5);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), (double)initial_value);
    // g_signal_connect(spin, "value-changed", G_CALLBACK(on_spinbutton_changed), sd);
	
    if (out_spin)
        *out_spin = GTK_SPIN_BUTTON(spin);

    GtkWidget *pair = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(pair), label);
    gtk_box_append(GTK_BOX(pair), spin);
    
    return pair;
}

static void on_direction_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer data)
{
	(void)pspec;
    t_app_context *ctx = (t_app_context *)data;
    if (!ctx || !ctx->area)
        return;

    guint selected = gtk_drop_down_get_selected(dropdown);
    
	if(selected == 0)
		ctx->win.opening_dir = OPEN_LEFT;
	else if (selected == 1)
		ctx->win.opening_dir = OPEN_RIGHT;
	   
    gtk_widget_queue_draw(ctx->area);
}


void on_mechanism_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data)
{
    (void)pspec;

	t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx || !ctx->area)
        return;

    guint selected = gtk_drop_down_get_selected(dropdown);

    if (selected == 0)
        ctx->win.mechanism = MECH_TURN_ONLY;
    else if (selected == 1)
        ctx->win.mechanism = MECH_TILT_AND_TURN;
    else if (selected == 2)
        ctx->win.mechanism = MECH_FIXED;

    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);

    gtk_widget_queue_draw(ctx->area);
	update_cut_list_ui(ctx);
}














GtkWidget *build_sidebar(t_app_context *ctx)
{
    if (!ctx)
        return NULL;

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    set_margin(sidebar, 10);

    // --- SECTION 1: ΔΙΑΣΤΑΣΕΙΣ (SPIN BUTTONS) ---
    GtkWidget *dim_label = gtk_label_new("<b>Frame dimension</b>");
    gtk_label_set_use_markup(GTK_LABEL(dim_label), TRUE);
    gtk_widget_set_halign(dim_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(sidebar), dim_label);

  // Δημιουργία Spin Button Πλάτους & Αποθήκευση στο ctx->spin_width
    GtkWidget *pair_width = build_labeled_spinbutton("Width:", ctx->win.width, &ctx->spin_width);
    
    // Δημιουργία Spin Button Ύψους & Αποθήκευση στο ctx->spin_height
    GtkWidget *pair_height = build_labeled_spinbutton("Height:", ctx->win.height, &ctx->spin_height);

    // Σύνδεση των signals ΜΕΤΑ τη δημιουργία των SpinButtons
    g_signal_connect(ctx->spin_width, "value-changed", G_CALLBACK(on_dimension_spin_changed), ctx);
    g_signal_connect(ctx->spin_height, "value-changed", G_CALLBACK(on_dimension_spin_changed), ctx);

    gtk_box_append(GTK_BOX(sidebar), pair_width);
    gtk_box_append(GTK_BOX(sidebar), pair_height);

    // Separator
    gtk_box_append(GTK_BOX(sidebar), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

    // --- SECTION 2: ΦΟΡΑ & ΜΗΧΑΝΙΣΜΟΣ (DROPDOWNS) ---
    GtkWidget *mech_title = gtk_label_new("<b>Opening and Mechanism</b>");
    gtk_label_set_use_markup(GTK_LABEL(mech_title), TRUE);
    gtk_widget_set_halign(mech_title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(sidebar), mech_title);

    // 1. Dropdown Φοράς
    const char *dirs[] = {"Left", "Right", NULL};
    GtkWidget *dir_label = gtk_label_new("Opening Direction:");
    gtk_widget_set_halign(dir_label, GTK_ALIGN_START);

    GtkWidget *dir_dropdown = gtk_drop_down_new_from_strings(dirs);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dir_dropdown), 
                                (ctx->win.opening_dir == OPEN_LEFT) ? 0 : 1);

    ctx->drop_direction = GTK_DROP_DOWN(dir_dropdown);
    g_signal_connect(dir_dropdown, "notify::selected", G_CALLBACK(on_direction_changed), ctx);

    gtk_box_append(GTK_BOX(sidebar), dir_label);
    gtk_box_append(GTK_BOX(sidebar), dir_dropdown);

    // 2. Dropdown Μηχανισμού
    const char *mechs[] = {"Turn", "Tilt & Turn", "Fixed", NULL};
    GtkWidget *mech_label = gtk_label_new("Type of Mechanism:");
    gtk_widget_set_halign(mech_label, GTK_ALIGN_START);

    GtkWidget *mech_dropdown = gtk_drop_down_new_from_strings(mechs);
    
    guint init_mech = 0;
    if (ctx->win.mechanism == MECH_TILT_AND_TURN) 
        init_mech = 1;
    else if (ctx->win.mechanism == MECH_FIXED) 
        init_mech = 2;
    
    gtk_drop_down_set_selected(GTK_DROP_DOWN(mech_dropdown), init_mech);

    ctx->drop_mechanism = GTK_DROP_DOWN(mech_dropdown);
    g_signal_connect(mech_dropdown, "notify::selected", G_CALLBACK(on_mechanism_changed), ctx);

    gtk_box_append(GTK_BOX(sidebar), mech_label);
    gtk_box_append(GTK_BOX(sidebar), mech_dropdown);

    return sidebar;
}





// with header for the buttons

void activate(GtkApplication *app, gpointer user_data)
{
    t_app_context *ctx = (t_app_context *)user_data;
    
    ctx->main_window = gtk_application_window_new(app);
    ctx->area = gtk_drawing_area_new();
    
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);

    // --- Create Header Bar ---
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(ctx->main_window), header_bar);
    
    
    // Create a horizontal box container with 10px spacing between children
    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    set_margin(btn_box, 10);

    // Insert Button
    GtkWidget *insert_btn = gtk_button_new_with_label("Insert");
    g_signal_connect(insert_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);

    // Save Button
    GtkWidget *save_btn = gtk_button_new_with_label("Save");
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);

    // Append buttons to the box container
    gtk_box_append(GTK_BOX(btn_box), insert_btn);
    gtk_box_append(GTK_BOX(btn_box), save_btn);




    // Pack the entire button box into the HeaderBar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_box);








    // // Insert Button (Start / Left side of titlebar)
    // GtkWidget *insert_btn = gtk_button_new_with_label("Insert");
    // g_signal_connect(insert_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);
    // gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), insert_btn);
    

    // // Save Button (Start / Left side of titlebar next to Insert)
    // GtkWidget *save_btn = gtk_button_new_with_label("Save");
    // g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);
    // gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), save_btn);

    // Build Layout...
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *sidebar = build_sidebar(ctx);
    GtkWidget *right_panel = build_cut_list_panel(ctx);

    gtk_widget_set_size_request(sidebar, 250, -1);
    set_margin(sidebar, 40);
    
    gtk_widget_set_hexpand(ctx->area, TRUE);
    gtk_widget_set_vexpand(ctx->area, TRUE);
    
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ctx->area), draw_function, ctx, NULL);
    
    gtk_box_append(GTK_BOX(box), sidebar);
    gtk_box_append(GTK_BOX(box), ctx->area);
    gtk_box_append(GTK_BOX(box), right_panel);
    
    gtk_window_set_title(GTK_WINDOW(ctx->main_window), "Window calculator");
    gtk_window_set_child(GTK_WINDOW(ctx->main_window), box);

    gtk_window_maximize(GTK_WINDOW(ctx->main_window));
    gtk_window_present(GTK_WINDOW(ctx->main_window));
}


// void activate(GtkApplication *app, gpointer user_data)
// {
// 	t_app_context *ctx = (t_app_context *)user_data;
	
//     ctx->main_window = gtk_application_window_new(app);
//     ctx->area = gtk_drawing_area_new();
	
// 	calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);
	
//     GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
// 	GtkWidget	*sidebar = build_sidebar(ctx);
// 	GtkWidget	*right_panel = build_cut_list_panel(ctx);
	

//     gtk_widget_set_size_request(sidebar, 250, -1);
// 	set_margin(sidebar, 40);
	
// 	gtk_widget_set_hexpand(ctx->area, TRUE);
//     gtk_widget_set_vexpand(ctx->area, TRUE);
	
// 	// Σύνδεση Cairo Render Function
//     gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ctx->area), draw_function, ctx, NULL);
	
// 	// Save button
//     GtkWidget *save_btn = gtk_button_new_with_label("Save");
//     g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);
	
// 	//Insert button
// 	GtkWidget *insert_btn = gtk_button_new_with_label("Insert");
// 	g_signal_connect(insert_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);
	
// 	gtk_box_append(GTK_BOX(sidebar), insert_btn);
//     gtk_box_append(GTK_BOX(sidebar), save_btn);
//     gtk_box_append(GTK_BOX(box), sidebar);
//     gtk_box_append(GTK_BOX(box), ctx->area);
//     gtk_box_append(GTK_BOX(box), right_panel);
	
//     gtk_window_set_title(GTK_WINDOW(ctx->main_window), "Window calculator");
//     gtk_window_set_child(GTK_WINDOW(ctx->main_window), box);

// 	gtk_window_maximize(GTK_WINDOW(ctx->main_window));
//     gtk_window_present(GTK_WINDOW(ctx->main_window));

// }

void	on_app_shutdown(GtkApplication *app, gpointer user_data)
{
	(void)app;
	t_app_context *ctx = (t_app_context *)user_data;

	if(ctx)
	{
		save_config_to_ini("config.ini", &ctx->cfg);
        g_print("Config saved successfully on exit.\n");
	}
}


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

            calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);
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




void open_profile_settings_dialog(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;

    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Profile & Hardware Setup (config.ini)");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(ctx->main_window));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 500);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);
    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);

    /* Frame Section */
    GtkWidget *lbl_frame = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_frame), "<b>Frame Specifications (mm)</b>");
    gtk_widget_set_halign(lbl_frame, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), lbl_frame);

    GtkWidget *sb_f_width = gtk_spin_button_new_with_range(40, 150, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_f_width), ctx->cfg.frame.width);
    gtk_box_append(GTK_BOX(box), sb_f_width);

    GtkWidget *sb_f_rebate = gtk_spin_button_new_with_range(10, 60, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_f_rebate), ctx->cfg.frame.rebate_width);
    gtk_box_append(GTK_BOX(box), sb_f_rebate);

    /* Save & Close Button */
    GtkWidget *btn_save = gtk_button_new_with_label("Save System Defaults");
    gtk_box_append(GTK_BOX(box), btn_save);

    gtk_window_set_child(GTK_WINDOW(dialog), box);
    gtk_window_present(GTK_WINDOW(dialog));
}


// GtkWidget *create_main_layout(t_app_context *ctx)
// {
//     GtkWidget *main_hbox;
//     GtkWidget *sidebar;
//     GtkWidget *right_panel;

//     if (!ctx)
//         return (NULL);

//     // Κύριο οριζόντιο Box
//     main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

//     // 1. ΑΡΙΣΤΕΡΑ: Inputs Sidebar
//     sidebar = build_sidebar(ctx);
//     gtk_box_append(GTK_BOX(main_hbox), sidebar);

//     // 2. ΚΕΝΤΡΟ: Cairo Canvas
//     ctx->area = gtk_drawing_area_new();
//     gtk_drawing_area_set_draw_func(
//         GTK_DRAWING_AREA(ctx->area),
//         draw_function,
//         ctx,
//         NULL
//     );
//     gtk_widget_set_hexpand(ctx->area, TRUE);
//     gtk_widget_set_vexpand(ctx->area, TRUE);
//     gtk_box_append(GTK_BOX(main_hbox), ctx->area);

//     // 3. ΔΕΞΙΑ: Λίστα Κοπών Panel
//     right_panel = build_cut_list_panel(ctx);
//     gtk_box_append(GTK_BOX(main_hbox), right_panel);

//     return (main_hbox);
// }


// resizeable panel
// GtkWidget *create_main_layout(t_app_context *ctx)
// {
//     GtkWidget *main_hbox;
//     GtkWidget *center_right_paned;
//     GtkWidget *sidebar;
//     GtkWidget *right_panel;

//     if (!ctx)
//         return (NULL);

//     main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

//     // 1. ΑΡΙΣΤΕΡΑ: Sidebar Inputs
//     sidebar = build_sidebar(ctx);
//     gtk_box_append(GTK_BOX(main_hbox), sidebar);

//     // 2. ΚΕΝΤΡΟ + ΔΕΞΙΑ: GtkPaned για Resizable Splitter
//     center_right_paned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
//     gtk_widget_set_hexpand(center_right_paned, TRUE);
//     gtk_widget_set_vexpand(center_right_paned, TRUE);

//     // Canvas στο αριστερό μέρος του Paned
//     ctx->area = gtk_drawing_area_new();
//     gtk_drawing_area_set_draw_func(
//         GTK_DRAWING_AREA(ctx->area),
//         draw_function,
//         ctx,
//         NULL
//     );
//     gtk_widget_set_hexpand(ctx->area, TRUE);
//     gtk_widget_set_vexpand(ctx->area, TRUE);
//     gtk_paned_set_start_child(GTK_PANED(center_right_paned), ctx->area);

//     // Δεξί Panel στο δεξί μέρος του Paned
//     right_panel = build_cut_list_panel(ctx);
//     gtk_paned_set_end_child(GTK_PANED(center_right_paned), right_panel);

//     // Αρχική θέση του splitter (π.χ. 350px από τα δεξιά)
//     gtk_paned_set_shrink_start_child(GTK_PANED(center_right_paned), FALSE);
//     gtk_paned_set_shrink_end_child(GTK_PANED(center_right_paned), FALSE);

//     gtk_box_append(GTK_BOX(main_hbox), center_right_paned);

//     return (main_hbox);
// }

