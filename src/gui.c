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
    calculate_dimensions(d->win, d->res);
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
    t_spin_data	*d = (t_spin_data *)data;
	if (!d || !d->win || !d->area) return; // Guard clause

    guint selected = gtk_drop_down_get_selected(dropdown);
    
    d->win->opening_dir = (selected == 0) ? OPEN_LEFT : OPEN_RIGHT;
    gtk_widget_queue_draw(d->area);
}

static void on_mechanism_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer data)
{
	(void)pspec;
    t_spin_data *d = (t_spin_data *)data;
    guint selected = gtk_drop_down_get_selected(dropdown);
    
    if (selected == 0) d->win->mechanism = MECH_TURN_ONLY;
    else if (selected == 1) d->win->mechanism = MECH_TILT_AND_TURN;
    else d->win->mechanism = MECH_FIXED;

	calculate_dimensions(d->win, d->res);
    gtk_widget_queue_draw(d->area);
}

// src/gui.c

GtkWidget *build_sidebar(t_window *win, t_result *res,t_profile_config *cfg, GtkWidget *area)
{
    // 1. Δημιουργία ενός ΚΟΙΝΟΥ Context Data για όλα τα controls
    // (ή περνάς ένα ήδη υπάρχον context που έφτιαξες στην activate)
    t_spin_data *app_data = g_new0(t_spin_data, 1);
    app_data->win = win;
	app_data->cfg = cfg;
    app_data->res = res;
    app_data->area = area;

    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
   set_margin(sidebar, 10);

    // --- SECTION 1: ΔΙΑΣΤΑΣΕΙΣ (SPIN BUTTONS) ---
    GtkWidget *dim_label = gtk_label_new("<b>Frame dimention</b>");
    gtk_label_set_use_markup(GTK_LABEL(dim_label), TRUE);
    gtk_widget_set_halign(dim_label, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(sidebar), dim_label);

	// Spin button pointers για Width & Height
    t_spin_data *w_data = g_new0(t_spin_data, 1);
    *w_data = *app_data;
    w_data->field = &win->width;

    t_spin_data *h_data = g_new0(t_spin_data, 1);
    *h_data = *app_data;
    h_data->field = &win->height;

    GtkWidget *pair_width = build_labeled_spinbutton("Width:", win->width, w_data);
    GtkWidget *pair_height = build_labeled_spinbutton("Height:", win->height, h_data);

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
                                (win->opening_dir == OPEN_LEFT) ? 0 : 1);

    // Σύνδεση με το callback
    g_signal_connect(dir_dropdown, "notify::selected", G_CALLBACK(on_direction_changed), app_data);

    gtk_box_append(GTK_BOX(sidebar), dir_label);
    gtk_box_append(GTK_BOX(sidebar), dir_dropdown);

    // 2. Dropdown Μηχανισμού
    const char *mechs[] = {"Turn", "Tilt & Turn", "Fixed", NULL};
    GtkWidget *mech_label = gtk_label_new("Type of Mechanism:");
    gtk_widget_set_halign(mech_label, GTK_ALIGN_START);

    GtkWidget *mech_dropdown = gtk_drop_down_new_from_strings(mechs);
    
    // Αρχικοποίηση επιλογής
    guint init_mech = 0;
    if (win->mechanism == MECH_TILT_AND_TURN) init_mech = 1;
    else if (win->mechanism == MECH_FIXED) init_mech = 2;
    
    gtk_drop_down_set_selected(GTK_DROP_DOWN(mech_dropdown), init_mech);

    // Σύνδεση με το callback
    g_signal_connect(mech_dropdown, "notify::selected", G_CALLBACK(on_mechanism_changed), app_data);

    gtk_box_append(GTK_BOX(sidebar), mech_label);
    gtk_box_append(GTK_BOX(sidebar), mech_dropdown);

    return sidebar;
}

void activate(GtkApplication *app, gpointer data)
{
	// ✅ 1. Ανάκτηση του app_data που στάλθηκε από τη main()
   	t_spin_data *app_data = (t_spin_data *)data;
	t_window    *win = app_data->win;
    t_profile_config *cfg = app_data->cfg;
	
	// ✅ 2. Δημιουργία των αποτελεσμάτων
	t_result	*res = g_new0(t_result, 1);
	app_data->res = res;

	// ✅ 3. Δημιουργία Canvas & Windows
    GtkWidget	*window = gtk_application_window_new(app);
    GtkWidget	*area = gtk_drawing_area_new();
	app_data->area = area;

    GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget	*sidebar = build_sidebar(win, res, cfg, area);
	
	// ✅ 4. Υπολογισμός πρώτων διαστάσεων
	calculate_dimensions(win, res);
	
    gtk_widget_set_size_request(sidebar, 250, -1);
	set_margin(sidebar, 20);
	
	gtk_widget_set_hexpand(area, TRUE);
    gtk_widget_set_vexpand(area, TRUE);
	
	// ✅ 5. Σύνδεση Cairo Render Function
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, app_data, NULL);
	
	// 🔘 Δημιουργία & Σύνδεση Κουμπιού Αποθήκευσης μέσα στο Sidebar
    GtkWidget *save_btn = gtk_button_new_with_label("💾 Save Project (.win)");
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), app_data); // ✅ Μπήκε εδώ
    gtk_box_append(GTK_BOX(sidebar), save_btn);

    gtk_box_append(GTK_BOX(box), sidebar);
    gtk_box_append(GTK_BOX(box), area);
	
    gtk_window_set_title(GTK_WINDOW(window), "Window calculator");
    // gtk_window_set_default_size(GTK_WINDOW(window), 1200, 1200);
    gtk_window_set_child(GTK_WINDOW(window), box);

	gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_present(GTK_WINDOW(window));

}

void	on_app_shutdown(GtkApplication *app, gpointer user_data)
{
	(void)app;
	t_spin_data	*data = (t_spin_data *)user_data;

	if(data && data->win && data->cfg)
	{
		// data->cfg->opening_dir = data->win->opening_dir;
        // data->cfg->mechanism   = data->win->mechanism;

		save_config_to_ini("config.ini", data->cfg);
        g_print("Config saved successfully on exit.\n");
	}
}




gboolean save_window_to_win(const char *filename, const t_window *win)
{
    if (!win)
        return FALSE;

    GKeyFile *keyfile = g_key_file_new();

    // 1. [Window] Section
    g_key_file_set_double(keyfile, "Window", "width", win->width);
    g_key_file_set_double(keyfile, "Window", "height", win->height);
    g_key_file_set_integer(keyfile, "Window", "mechanism", (int)win->mechanism);
    g_key_file_set_integer(keyfile, "Window", "opening_dir", (int)win->opening_dir);

    // 2. [Frame] Section
    g_key_file_set_double(keyfile, "Frame", "width", win->frame.width);
    g_key_file_set_double(keyfile, "Frame", "rebate_width", win->frame.rebate_width);

    // 3. [Sash] Section
    g_key_file_set_double(keyfile, "Sash", "width", win->sash.width);
    g_key_file_set_double(keyfile, "Sash", "overlap", win->sash.overlap);

    GError *error = NULL;
    gboolean success = g_key_file_save_to_file(keyfile, filename, &error);

    if (!success) {
        g_printerr("Error saving WIN file: %s\n", error->message);
        g_error_free(error);
    }

    g_key_file_free(keyfile);
    return success;
}










void	on_save_project_clicked(GtkButton *btn, gpointer user_data)
{
	(void)btn;
	t_spin_data	*d = (t_spin_data *)user_data;

	if(!d || !d->win)
		return;
	
    if (save_window_to_win("my_window.win", d->win))
    {
        g_print("Project saved successfully to my_window.win!\n");
    }
}