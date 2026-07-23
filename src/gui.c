#include "gui.h"
#include "renderer.h"
#include "calculator.h"

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
    
    GtkWidget *spin = gtk_spin_button_new_with_range(400, 3000, 10);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), initial_value);
    g_signal_connect(spin, "value-changed", G_CALLBACK(on_spinbutton_changed), sd);
    
    GtkWidget *pair = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_append(GTK_BOX(pair), label);
    gtk_box_append(GTK_BOX(pair), spin);
    
    return pair;
}

GtkWidget *build_sidebar(t_window *win, t_result *res, GtkWidget *area)
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


void activate(GtkApplication *app, gpointer data)
{
   
    t_spin_data *draw_data = g_new0(t_spin_data, 1);
	t_window	*win = (t_window *) data ;
	t_result	*res = g_new0(t_result, 1);
   
    draw_data->win = win;
    draw_data->res = res;
    // draw_data->area = area;

	
    GtkWidget	*window = gtk_application_window_new(app);
    GtkWidget	*area = gtk_drawing_area_new();
    GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
	GtkWidget	*sidebar = build_sidebar(win, res, area);
	
	
	calculate_dimensions(win, res);
	
    gtk_widget_set_size_request(sidebar, 250, -1);
	set_margin(sidebar, 40);
	
	gtk_widget_set_hexpand(area, TRUE);
    gtk_widget_set_vexpand(area, TRUE);
	
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, draw_data, g_free);
	
    gtk_box_append(GTK_BOX(box), sidebar);
    gtk_box_append(GTK_BOX(box), area);
	
    gtk_window_set_title(GTK_WINDOW(window), "Window calculator");
    // gtk_window_set_default_size(GTK_WINDOW(window), 1200, 1200);
    gtk_window_set_child(GTK_WINDOW(window), box);
	
	RsvgHandle *handle = rsvg_handle_new_from_file("A.svg", NULL);
    rsvg_handle_render_cairo(handle, window);

	gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_present(GTK_WINDOW(window));

}
