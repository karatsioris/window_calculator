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
    GtkWidget	*label;
    GtkWidget	*spin;
    GtkWidget	*pair;

    label = gtk_label_new(label_text);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    
    spin = gtk_spin_button_new_with_range(350, 3000, 5);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), (double)initial_value);
	
    if (out_spin)
        *out_spin = GTK_SPIN_BUTTON(spin);

    pair = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(pair), label);
    gtk_box_append(GTK_BOX(pair), spin);
    
    return pair;
}

static GtkWidget *build_labeled_dropdown(const char *label_text, const char **options, guint selected_index, GtkDropDown **out_dropdown)
{
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *dropdown;

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    label = gtk_label_new(label_text);
    
    gtk_widget_set_halign(label, GTK_ALIGN_START);

    dropdown = gtk_drop_down_new_from_strings(options);
    gtk_drop_down_set_selected(GTK_DROP_DOWN(dropdown), selected_index);

    if (out_dropdown)
        *out_dropdown = GTK_DROP_DOWN(dropdown);

    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), dropdown);

    return box;
}

static void    refresh_application(t_app_context *ctx)
{
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

    if (ctx->area)
    	gtk_widget_queue_draw(ctx->area);
    update_cut_list_ui(ctx); 
}

static void on_dimension_spin_changed(GtkSpinButton *spin, gpointer user_data)
{
    (void)spin;
    t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx) return;

   
    ctx->win.width  = (float)gtk_spin_button_get_value(ctx->spin_width);
    ctx->win.height = (float)gtk_spin_button_get_value(ctx->spin_height);

    refresh_application(ctx);
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

    refresh_application(ctx);
}

static GtkWidget *create_section_label(const char *text, GtkWidget *parent_box)
{
    if (!text || !parent_box)
        return NULL;

    char *markup = g_markup_printf_escaped("<b>%s</b>", text);
    GtkWidget *label = gtk_label_new(NULL);

    gtk_label_set_markup(GTK_LABEL(label), markup);
    gtk_widget_set_halign(label, GTK_ALIGN_START);

    g_free(markup);

    if (parent_box)
        gtk_box_append(GTK_BOX(parent_box), label);

    return label;
}

static void build_dimensions_section(t_app_context *ctx, GtkWidget *sidebar)
{
    create_section_label("Frame dimension", sidebar);

    GtkWidget *w_pair = build_labeled_spinbutton("Width:", ctx->win.width, &ctx->spin_width);
    GtkWidget *h_pair = build_labeled_spinbutton("Height:", ctx->win.height, &ctx->spin_height);

    if (ctx->spin_width)
        g_signal_connect(ctx->spin_width, "value-changed", G_CALLBACK(on_dimension_spin_changed), ctx);
    if (ctx->spin_height)
        g_signal_connect(ctx->spin_height, "value-changed", G_CALLBACK(on_dimension_spin_changed), ctx);

    gtk_box_append(GTK_BOX(sidebar), w_pair);
    gtk_box_append(GTK_BOX(sidebar), h_pair);
}

static void build_mechanism_section(t_app_context *ctx, GtkWidget *sidebar)
{
    GtkWidget *dir_box;
    GtkWidget *mech_box;
    
    create_section_label("Opening and Mechanism", sidebar);

    // Direction Dropdown
    const char *dirs[] = {"Left", "Right", NULL};
    guint dir_idx = (ctx->win.opening_dir == OPEN_LEFT) ? 0 : 1;
    dir_box = build_labeled_dropdown("Opening Direction:", dirs, dir_idx, &ctx->drop_direction);

    g_signal_connect(ctx->drop_direction, "notify::selected", G_CALLBACK(on_direction_changed), ctx);
    gtk_box_append(GTK_BOX(sidebar), dir_box);

    // Mechanism Dropdown
    const char *mechs[] = {"Turn", "Tilt & Turn", "Fixed", NULL};
    guint mech_idx = 0;
    if (ctx->win.mechanism == MECH_TILT_AND_TURN) 
        mech_idx = 1;
    else if (ctx->win.mechanism == MECH_FIXED) 
        mech_idx = 2;

    mech_box = build_labeled_dropdown("Type of Mechanism:", mechs, mech_idx, &ctx->drop_mechanism);

    g_signal_connect(ctx->drop_mechanism, "notify::selected", G_CALLBACK(on_mechanism_changed), ctx);
    gtk_box_append(GTK_BOX(sidebar), mech_box);
}

GtkWidget *build_sidebar(t_app_context *ctx)
{
    GtkWidget *sidebar;

    if (!ctx)
        return NULL;

    sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_widget_set_size_request(sidebar, 100, -1);
    set_margin(sidebar, 40);

    build_dimensions_section(ctx, sidebar);
    gtk_box_append(GTK_BOX(sidebar), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL));
    build_mechanism_section(ctx, sidebar);

    return sidebar;
}