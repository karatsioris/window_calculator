#include "gui.h"
#include "renderer.h"
#include "calculator.h"
#include "config.h"

#define REFERENCE_MAX_MM 4000.0

static void	set_margin(GtkWidget *place, int num)
{
	gtk_widget_set_margin_start(place, num);
	gtk_widget_set_margin_end(place, num);
	gtk_widget_set_margin_top(place, num);
	gtk_widget_set_margin_bottom(place, num);
}


static void on_spinbutton_changed(GtkSpinButton *spin, gpointer data)
{
    t_spin_data *d = data;
    *d->field = (float)gtk_spin_button_get_value(spin);
    calculate_dimensions(d->win, d->res, d->cfg);
    gtk_widget_queue_draw(d->area);
}

static GtkWidget *build_labeled_spinbutton(const char *label_text, float initial_value, t_spin_data *sd)
{
    GtkWidget *label = gtk_label_new(label_text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    
    GtkWidget *spin = gtk_spin_button_new_with_range(300, 3000, 10);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), (double)initial_value);
    g_signal_connect(spin, "value-changed", G_CALLBACK(on_spinbutton_changed), sd);
    
    GtkWidget *pair = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(pair), label);
    gtk_box_append(GTK_BOX(pair), spin);
    
    return pair;
}

// GtkWidget *build_sidebar(t_window *win, t_result *res, GtkWidget *area)
// {
//     GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);

//     t_spin_data *width_data = g_new0(t_spin_data, 1);
//     width_data->field = &win->width;
//     width_data->win = win;
//     width_data->res = res;
//     width_data->area = area;

//     t_spin_data *height_data = g_new0(t_spin_data, 1);
//     height_data->field = &win->height;
//     height_data->win = win;
//     height_data->res = res;
//     height_data->area = area;

//     GtkWidget *pair_width = build_labeled_spinbutton("Frame width (mm)", win->width, width_data);
//     GtkWidget *pair_height = build_labeled_spinbutton("Frame height (mm)", win->height, height_data);

//     gtk_box_append(GTK_BOX(sidebar), pair_width);
//     gtk_box_append(GTK_BOX(sidebar), pair_height);

//     return sidebar;
// }

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
}


// GtkWidget *build_sidebar(t_window *win, t_result *res,t_profile_config *cfg, GtkWidget *area)
// {
//     t_spin_data *app_data = g_new0(t_spin_data, 1);
//     app_data->win = win;
// 	app_data->cfg = cfg;
//     app_data->res = res;
//     app_data->area = area;

//     GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
//    	set_margin(sidebar, 10);

//     // --- SECTION 1: ΔΙΑΣΤΑΣΕΙΣ (SPIN BUTTONS) ---
//     GtkWidget *dim_label = gtk_label_new("<b>Frame dimention</b>");
//     gtk_label_set_use_markup(GTK_LABEL(dim_label), TRUE);
//     gtk_widget_set_halign(dim_label, GTK_ALIGN_START);
//     gtk_box_append(GTK_BOX(sidebar), dim_label);

// 	// Spin button pointers για Width & Height
//     t_spin_data *w_data = g_new0(t_spin_data, 1);
//     *w_data = *app_data;
//     w_data->field = &win->width;

//     t_spin_data *h_data = g_new0(t_spin_data, 1);
//     *h_data = *app_data;
//     h_data->field = &win->height;

//     GtkWidget *pair_width = build_labeled_spinbutton("Width:", win->width, w_data);
//     GtkWidget *pair_height = build_labeled_spinbutton("Height:", win->height, h_data);

//     gtk_box_append(GTK_BOX(sidebar), pair_width);
//     gtk_box_append(GTK_BOX(sidebar), pair_height);

//     // Separator
//     gtk_box_append(GTK_BOX(sidebar), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));

//     // --- SECTION 2: ΦΟΡΑ & ΜΗΧΑΝΙΣΜΟΣ (DROPDOWNS) ---
//     GtkWidget *mech_title = gtk_label_new("<b>Opening and Mechanism</b>");
//     gtk_label_set_use_markup(GTK_LABEL(mech_title), TRUE);
//     gtk_widget_set_halign(mech_title, GTK_ALIGN_START);
//     gtk_box_append(GTK_BOX(sidebar), mech_title);

//     // 1. Dropdown Φοράς
//     const char *dirs[] = {"Left", "Right", NULL};
//     GtkWidget *dir_label = gtk_label_new("Opening Direction:");
//     gtk_widget_set_halign(dir_label, GTK_ALIGN_START);

//     GtkWidget *dir_dropdown = gtk_drop_down_new_from_strings(dirs);
//     gtk_drop_down_set_selected(GTK_DROP_DOWN(dir_dropdown), 
//                                 (win->opening_dir == OPEN_LEFT) ? 0 : 1);

//     // Σύνδεση με το callback
//     g_signal_connect(dir_dropdown, "notify::selected", G_CALLBACK(on_direction_changed), app_data);

//     gtk_box_append(GTK_BOX(sidebar), dir_label);
//     gtk_box_append(GTK_BOX(sidebar), dir_dropdown);

//     // 2. Dropdown Μηχανισμού
//     const char *mechs[] = {"Turn", "Tilt & Turn", "Fixed", NULL};
//     GtkWidget *mech_label = gtk_label_new("Type of Mechanism:");
//     gtk_widget_set_halign(mech_label, GTK_ALIGN_START);

//     GtkWidget *mech_dropdown = gtk_drop_down_new_from_strings(mechs);
    
//     // Αρχικοποίηση επιλογής
//     guint init_mech = 0;
//     if (win->mechanism == MECH_TILT_AND_TURN) init_mech = 1;
//     else if (win->mechanism == MECH_FIXED) init_mech = 2;
    
//     gtk_drop_down_set_selected(GTK_DROP_DOWN(mech_dropdown), init_mech);

//     // Σύνδεση με το callback
//     g_signal_connect(mech_dropdown, "notify::selected", G_CALLBACK(on_mechanism_changed), app_data);

//     gtk_box_append(GTK_BOX(sidebar), mech_label);
//     gtk_box_append(GTK_BOX(sidebar), mech_dropdown);

//     return sidebar;
// }



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

    // Spin button για Πλάτος (Width)
    t_spin_data *w_data = g_new0(t_spin_data, 1);
    w_data->win = &ctx->win;
    w_data->cfg = &ctx->cfg;
    w_data->res = &ctx->res;
    w_data->area = ctx->area;
    w_data->field = &ctx->win.width;

    GtkWidget *pair_width = build_labeled_spinbutton("Width:", ctx->win.width, w_data);
    ctx->spin_width = GTK_SPIN_BUTTON(gtk_widget_get_last_child(pair_width));

    // Spin button για Ύψος (Height)
    t_spin_data *h_data = g_new0(t_spin_data, 1);
    *h_data = *w_data; // Αντιγραφή βασικών δεικτών
    h_data->field = &ctx->win.height;

    GtkWidget *pair_height = build_labeled_spinbutton("Height:", ctx->win.height, h_data);
    ctx->spin_height = GTK_SPIN_BUTTON(gtk_widget_get_last_child(pair_height));

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

    // Σύνδεση με το callback (περνάμε το ctx)
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









void activate(GtkApplication *app, gpointer user_data)
{
	t_app_context *ctx = (t_app_context *)user_data;
	
    ctx->main_window = gtk_application_window_new(app);
    ctx->area = gtk_drawing_area_new();
	

    GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget	*sidebar = build_sidebar(ctx);
	
	// Υπολογισμός πρώτων διαστάσεων
	calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);
	
    gtk_widget_set_size_request(sidebar, 250, -1);
	set_margin(sidebar, 20);
	
	gtk_widget_set_hexpand(ctx->area, TRUE);
    gtk_widget_set_vexpand(ctx->area, TRUE);
	
	// Σύνδεση Cairo Render Function
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ctx->area), draw_function, ctx, NULL);
	
	// Save button
    GtkWidget *save_btn = gtk_button_new_with_label("Save");
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);

	//Insert button
	GtkWidget *insert_btn = gtk_button_new_with_label("Insert");
	g_signal_connect(insert_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);
	
	gtk_box_append(GTK_BOX(sidebar), insert_btn);
    gtk_box_append(GTK_BOX(sidebar), save_btn);
    gtk_box_append(GTK_BOX(box), sidebar);
    gtk_box_append(GTK_BOX(box), ctx->area);
	
    gtk_window_set_title(GTK_WINDOW(ctx->main_window), "Window calculator");
    gtk_window_set_child(GTK_WINDOW(ctx->main_window), box);

	gtk_window_maximize(GTK_WINDOW(ctx->main_window));
    gtk_window_present(GTK_WINDOW(ctx->main_window));

}

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




gboolean save_window_to_win(const char *filename, const t_window *win, const t_profile_config *cfg)
{
    if (!filename || !win || !cfg)
        return FALSE;

    GKeyFile *keyfile = g_key_file_new();

    // 1. [Window] Section
    g_key_file_set_double(keyfile, "Window", "width", win->width);
    g_key_file_set_double(keyfile, "Window", "height", win->height);
    g_key_file_set_integer(keyfile, "Window", "mechanism", (int)win->mechanism);
    g_key_file_set_integer(keyfile, "Window", "opening_dir", (int)win->opening_dir);

    // 2. [Frame] Section
    g_key_file_set_double(keyfile, "Frame", "width", cfg->frame.width);
    g_key_file_set_double(keyfile, "Frame", "rebate_width", cfg->frame.rebate_width);

    // 3. [Sash] Section
    g_key_file_set_double(keyfile, "Sash", "width", cfg->sash.width);
    g_key_file_set_double(keyfile, "Sash", "overlap", cfg->sash.overlap);

    GError *error = NULL;
    gboolean success = g_key_file_save_to_file(keyfile, filename, &error);

    if (!success)
	{
        g_printerr("Error saving WIN file: %s\n", error ? error->message : "Unknown");
        if (error) g_error_free(error);
    }

    g_key_file_free(keyfile);
    return success;
}




void	on_save_project_clicked(GtkButton *btn, gpointer user_data)
{
	(void)btn;
	t_app_context *ctx = (t_app_context *)user_data;

	if(!ctx)
		return;
	
    if (save_window_to_win("my_window.win", &ctx->win , &ctx->cfg))
    {
        g_print("Project saved successfully to my_window.win!\n");
    }
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