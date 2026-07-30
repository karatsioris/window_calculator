#include "gui.h"

void	set_margin(GtkWidget *place, int num)
{
	gtk_widget_set_margin_start(place, num);
	gtk_widget_set_margin_end(place, num);
	gtk_widget_set_margin_top(place, num);
	gtk_widget_set_margin_bottom(place, num);
}

static GtkWidget *build_labeled_spinbutton(const char *label_text, float initial_value, GtkSpinButton **out_spin)
{
    GtkWidget *label = gtk_label_new(label_text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    
    GtkWidget *spin = gtk_spin_button_new_with_range(350, 3000, 5);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), (double)initial_value);
	
    if (out_spin)
        *out_spin = GTK_SPIN_BUTTON(spin);

    GtkWidget *pair = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(pair), label);
    gtk_box_append(GTK_BOX(pair), spin);
    
    return pair;
}


static void on_dimension_spin_changed(GtkSpinButton *spin, gpointer user_data)
{
    (void)spin;
    t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx) return;

   
    ctx->win.width  = (float)gtk_spin_button_get_value(ctx->spin_width);
    ctx->win.height = (float)gtk_spin_button_get_value(ctx->spin_height);

    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

    if (ctx->area)
    	gtk_widget_queue_draw(ctx->area);
    update_cut_list_ui(ctx); 
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

    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

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