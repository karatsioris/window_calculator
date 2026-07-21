#include "calculator.h"

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
    
    GtkWidget *spin = gtk_spin_button_new_with_range(400, 3000, 10);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), initial_value);
    g_signal_connect(spin, "value-changed", G_CALLBACK(on_spinbutton_changed), sd);
    
    GtkWidget *pair = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(pair), label);
    gtk_box_append(GTK_BOX(pair), spin);
    
    return pair;
}

static GtkWidget *build_sidebar(t_window *win, t_result *res, GtkWidget *area)
{
    GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);

    t_spin_data *width_data = g_new0(t_spin_data, 1);
    width_data->field = &win->width;
    width_data->win = win;
    width_data->res = res;
    width_data->area = area;

    t_spin_data *height_data = g_new0(t_spin_data, 1);
    height_data->field = &win->height;
    height_data->win = win;
    height_data->res = res;
    height_data->area = area;

    GtkWidget *pair_width = build_labeled_spinbutton("Frame width (mm)", win->width, width_data);
    GtkWidget *pair_height = build_labeled_spinbutton("Frame height (mm)", win->height, height_data);

    gtk_box_append(GTK_BOX(sidebar), pair_width);
    gtk_box_append(GTK_BOX(sidebar), pair_height);

    return sidebar;
}

static void	print_dim_for_check(t_result *res)
{
	printf("Frame width: %f\n", res->frame_width);
    printf("Frame height: %f\n", res->frame_height);

	printf("Sash width: %f\n", res->sash_width);
    printf("Sash height: %f\n", res->sash_height);
    
	printf("Frame Jambs: %f\n", res->frame_vertical_cut);
    printf("Frame Head/Sill: %f\n", res->frame_horizontal_cut);
}



void draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
{
	t_result *res = (t_result *) data;
	(void)res;
	
  	float width_frame = res->frame_width;
	float height_frame = res->frame_height;

	float width_sash = res->sash_width;
	float height_sash = res->sash_height;
  	(void)area;

	print_dim_for_check(res);
	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* white */
	cairo_paint (cr);

	cairo_set_line_width (cr, 3.0);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); /* black */
	cairo_rectangle (cr,
					width/2.0 - width_frame/2,
					height/2.0 - height_frame/2,
					width_frame,
					height_frame);
	cairo_stroke (cr);

	cairo_set_line_width (cr, 1.5);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_rectangle (cr,
					width/2.0 - width_frame/2 + 41,
					height/2.0 - height_frame/2 + 41,
					width_sash,
					height_sash);
	cairo_stroke (cr);

	int x1 = 50;
	int y1 = 50;
	int x2 = 200;
	int y2 = 200;
	// Κύρια γραμμή
	cairo_move_to(cr, x1, y1);
	cairo_line_to(cr, x2, y2);
	cairo_stroke(cr);

	// Μύτη βέλους στο (x2, y2) — δύο μικρές διαγώνιες γραμμές
	double angle = atan2(y2 - y1, x2 - x1);  // κατεύθυνση της κύριας γραμμής
	double arrow_len = 10.0;
	double arrow_angle = 0.4; // ακτίνια, πόσο "ανοιχτή" είναι η μύτη

	cairo_move_to(cr, x2, y2);
	cairo_line_to(cr,
		x2 - arrow_len * cos(angle - arrow_angle),
		y2 - arrow_len * sin(angle - arrow_angle));
	cairo_stroke(cr);

	cairo_move_to(cr, x2, y2);
	cairo_line_to(cr,
		x2 - arrow_len * cos(angle + arrow_angle),
		y2 - arrow_len * sin(angle + arrow_angle));
	cairo_stroke(cr);
}

void activate(GtkApplication *app, gpointer data)
{
   
	t_window	*win = (t_window *) data;
	t_result	*res = g_new0(t_result, 1);

    GtkWidget	*window = gtk_application_window_new(app);
    GtkWidget	*area = gtk_drawing_area_new();
    GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	GtkWidget	*sidebar = build_sidebar(win, res, area);

    calculate_dimensions(win, res);

    gtk_widget_set_size_request(sidebar, 250, -1);
	set_margin(sidebar, 40);

	gtk_widget_set_hexpand(area, TRUE);
    gtk_widget_set_vexpand(area, TRUE);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, res, NULL);

    gtk_box_append(GTK_BOX(box), sidebar);
    gtk_box_append(GTK_BOX(box), area);

    gtk_window_set_title(GTK_WINDOW(window), "Window calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 1200);
    gtk_window_set_child(GTK_WINDOW(window), box);
    gtk_window_present(GTK_WINDOW(window));

}
