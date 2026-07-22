#include "calculator.h"
#include "stdio.h"
#include "stdlib.h"
#include "gtk/gtk.h"


// int main(int argc, char **argv)
// {


//     t_window    *win = NULL;
//     t_result    *res = NULL;

//     win = malloc(sizeof (t_window));
//     res = malloc(sizeof (t_result));
//     initialization(win);
//     if(argc >= 2)
//     {
//         win->width = atof(argv[1]);
//         win->height = atof(argv[2]);

//         printf("%f\n", win->width);
//         printf("%f\n", win->height);
//         printf("%f\n",  win->frame.width);
//         printf("%f\n",  win->frame.rebate);
//         printf("%f\n",  win->frame.sill_rebate);
//         printf("%f\n", win->join.overlap);


//         calculate_dimensions(win, res);
//         printf("Sash width: %f\n", res->sash_width);
//         printf("Sash height: %f\n", res->sash_height);
//         printf("Frame Jambs: %f\n", res->frame_vertical_cut);
//         printf("Frame Head/Sill: %f\n", res->frame_horizontal_cut);
//         free(win);
//         free(res);
//     }
//     return(0);
// }

// static void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
// {
//     GdkRGBA color;
//     (void)data;

//   cairo_arc (cr,
//              width / 2.0, height / 2.0,
//              MIN (width, height) / 2.0,
//              0, 2 * G_PI);

//   gtk_widget_get_color (GTK_WIDGET (area),
//                         &color);
//   gdk_cairo_set_source_rgba (cr, &color);

//   cairo_fill (cr);
// }


// static void draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
// {
// 	// t_result *win = (t_window *) data;
// 	t_result *res = (t_result *) data;
// 	(void)res;
	
//   	int width_frame = 500;
// 	int height_frame = 500;

// 	int width_sash = 300;
// 	int height_sash = 300;
//   	(void)area;


// 	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* white */
// 	cairo_paint (cr);

// 	cairo_set_line_width (cr, 3.0);
// 	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); /* black */
// 	cairo_rectangle (cr,
// 					width/2.0 - width_frame/2,
// 					height/2.0 - height_frame/2,
// 					width_frame,
// 					height_frame);
// 	cairo_stroke (cr);

// 	cairo_set_line_width (cr, 1.5);
// 	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
// 	cairo_rectangle (cr,
// 					width/2.0 - width_frame/2,
// 					height/2.0 - height_frame/2,
// 					width_sash,
// 					height_sash);
// 	cairo_stroke (cr);

// 	int x1 = 50;
// 	int y1 = 50;
// 	int x2 = 200;
// 	int y2 = 200;
// 	// Κύρια γραμμή
// 	cairo_move_to(cr, x1, y1);
// 	cairo_line_to(cr, x2, y2);
// 	cairo_stroke(cr);

// 	// Μύτη βέλους στο (x2, y2) — δύο μικρές διαγώνιες γραμμές
// 	double angle = atan2(y2 - y1, x2 - x1);  // κατεύθυνση της κύριας γραμμής
// 	double arrow_len = 10.0;
// 	double arrow_angle = 0.4; // ακτίνια, πόσο "ανοιχτή" είναι η μύτη

// 	cairo_move_to(cr, x2, y2);
// 	cairo_line_to(cr,
// 		x2 - arrow_len * cos(angle - arrow_angle),
// 		y2 - arrow_len * sin(angle - arrow_angle));
// 	cairo_stroke(cr);

// 	cairo_move_to(cr, x2, y2);
// 	cairo_line_to(cr,
// 		x2 - arrow_len * cos(angle + arrow_angle),
// 		y2 - arrow_len * sin(angle + arrow_angle));
// 	cairo_stroke(cr);
// }


// static void on_spinbutton_changed(GtkSpinButton *spin, gpointer data)
// {
//     // εδώ πρέπει να ξέρεις: αυτή η αλλαγή είναι για το width ή το height;
// 	float *field = (float *) data;
//     *field = (float) gtk_spin_button_get_value(spin);
// }

// static void	set_margin(GtkWidget *place, int num)
// {
// 	gtk_widget_set_margin_start(place, num);
// 	gtk_widget_set_margin_end(place, num);
// 	gtk_widget_set_margin_top(place, num);
// 	gtk_widget_set_margin_bottom(place, num);
// }

// static void activate(GtkApplication *app, gpointer data)
// {
//     t_window 	*win = (t_window *) data;
//     GtkWidget 	*window = gtk_application_window_new(app);
//     GtkWidget  	*area = gtk_drawing_area_new();
// 	GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
// 	GtkWidget	*spin_width = gtk_spin_button_new_with_range(400,3000,10);
// 	GtkWidget	*spin_height = gtk_spin_button_new_with_range(400, 3000, 10);
// 	// GtkWidget	*sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
// 	GtkWidget	*label_width = gtk_label_new("Πλάτος κάσας (mm)");
// 	GtkWidget	*label_height = gtk_label_new("Υψος κάσας (mm)");


// 		// *****************************
// 	// elenxo ton kainwn me omadopoihsh
// 	// Ζευγάρι 1: width
// 	GtkWidget *pair_width = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // 2px: label κοντά στο spinbutton
// 	gtk_box_append(GTK_BOX(pair_width), label_width);
// 	gtk_box_append(GTK_BOX(pair_width), spin_width);

// 	// Ζευγάρι 2: height
// 	GtkWidget *pair_height = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
// 	gtk_box_append(GTK_BOX(pair_height), label_height);
// 	gtk_box_append(GTK_BOX(pair_height), spin_height);

// 	// Sidebar: μεγαλύτερο κενό ανάμεσα στα ζευγάρια
// 	GtkWidget *sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 100); // 20px: ζευγάρι από ζευγάρι
// 	gtk_box_append(GTK_BOX(sidebar), pair_width);
// 	gtk_box_append(GTK_BOX(sidebar), pair_height);
// 	// ******************************


// 	gtk_widget_set_size_request(sidebar, 250, -1);
// 	set_margin(sidebar, 40);
//     gtk_init ();

// 	// // title for width
// 	gtk_widget_set_halign(label_width, GTK_ALIGN_START);  // στοίχιση αριστερά, όχι κέντρο
// 	// gtk_box_append(GTK_BOX(sidebar), label_width);
// 	// gtk_box_append(GTK_BOX(sidebar), spin_width);
	
// 	// // title for height
// 	gtk_widget_set_halign(label_height, GTK_ALIGN_START);  // στοίχιση αριστερά, όχι κέντρο
// 	// gtk_box_append(GTK_BOX(sidebar), label_height);
// 	// gtk_box_append(GTK_BOX(sidebar), spin_width);


	
// 	// button
//     gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_width), win->width);
// 	g_signal_connect(spin_width, "value-changed", G_CALLBACK(on_spinbutton_changed), &win->width);
	
	
	
// 	// button
// 	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_height), win->height);
// 	g_signal_connect(spin_height, "value-changed", G_CALLBACK(on_spinbutton_changed), &win->height);

// 	// gtk_box_append(GTK_BOX(sidebar), spin_width);
// 	// gtk_box_append(GTK_BOX(sidebar), spin_height);

// 	gtk_widget_set_vexpand(area, TRUE);   // να απορροφά τον υπόλοιπο κάθετο χώρο
// 	gtk_widget_set_hexpand(area, TRUE);

// 	// gtk_box_append(GTK_BOX(box), spin_width);
// 	// gtk_box_append(GTK_BOX(box), spin_height);
// 	gtk_box_append(GTK_BOX(box), sidebar);  // αριστερά: inputs
// 	gtk_box_append(GTK_BOX(box), area);
	
// 	// Ορισμός τίτλου και αρχικού μεγέθους (Πλάτος, Ύψος)
//     gtk_window_set_title(GTK_WINDOW(window), "Window calculator");
//     gtk_window_set_default_size(GTK_WINDOW(window), 1200, 1200);

//     gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (area), draw_function, win, NULL);



//     // Ελαχιστες τιμες για το παραθυρο
//     // gtk_drawing_area_set_content_width(GTK_DRAWING_AREA (area), 1200);
//     // gtk_drawing_area_set_content_height(GTK_DRAWING_AREA (area), 1200);

//     gtk_window_set_child (GTK_WINDOW (window), box);

//     // Εμφάνιση του παραθύρου στην οθόνη
//     gtk_window_present(GTK_WINDOW(window));
// }


void    initialization(t_window *win)
{
    win->width = 500;
    win->height = 500;
    win->sash.width = 75;
    win->frame.width = 79;
    win->frame.rebate = 32;
    win->frame.sill_rebate = 20;
    win->join.overlap = 6;
}


int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;
    t_window    *win = NULL;

    win = malloc(sizeof (t_window));
  
    initialization(win);

    // ******* Δημιουργία της εφαρμογής με ένα μοναδικό ID (απαραίτητο στο GTK4) **********
    app = gtk_application_new("com.konstantinos.windowcalc", G_APPLICATION_DEFAULT_FLAGS);
    // app = gtk_application_new("com.konstantinos.windowcalc", G_APPLICATION_NON_UNIQUE);


    // Σύνδεση του "activate" signal με τη συνάρτηση activate που γράψαμε παραπάνω
    g_signal_connect(app, "activate", G_CALLBACK(activate), win);
    
    // Εκκίνηση της εφαρμογής
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    // Καθαρισμός μνήμης της εφαρμογής
    g_object_unref(app);

   

	free(win);
    return status;

}