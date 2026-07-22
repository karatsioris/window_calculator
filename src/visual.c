#include "calculator.h"

#define REFERENCE_MAX_MM 6000.0

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

// static void	print_dim_for_check(t_result *res)
// {
// 	printf("Frame width: %f\n", res->frame_width);
//     printf("Frame height: %f\n", res->frame_height);

// 	printf("Sash width: %f\n", res->sash_width);
//     printf("Sash height: %f\n", res->sash_height);
    
// 	printf("Frame Jambs: %f\n", res->frame_vertical_cut);
//     printf("Frame Head/Sill: %f\n", res->frame_horizontal_cut);
// }



// void draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
// {
// 	printf("WIDTH: %d", width);
// 	printf("HEIGHT :%d", height);
// 	t_result *res = (t_result *) data;
// 	(void)res;
	
// 	float scale = (float)width / REFERENCE_MAX_MM;
	


//   	float width_frame = res->frame_width;
// 	float height_frame = res->frame_height;

// 	float width_sash = res->sash_width;
// 	float height_sash = res->sash_height;
//   	(void)area;

// 	float scaled_frame_w = width_frame * scale;
// 	float scaled_frame_h = height_frame * scale;

// 	float scaled_sash_w = width_sash * scale;
// 	float scaled_sash_h = height_sash * scale;

// 	print_dim_for_check(res);

// 	cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* white */
// 	cairo_paint (cr);

// 	cairo_set_line_width (cr, 1.0);
// 	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); /* black */
// 	cairo_rectangle (cr,
// 					width/2.0 - scaled_frame_w/2,
// 					height/2.0 - scaled_frame_h/2,
// 					scaled_frame_w,
// 					scaled_frame_h);
// 	cairo_stroke (cr);

// 	cairo_set_line_width (cr, 0.5);
// 	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
// 	cairo_rectangle (cr,
// 					width/2.0 - scaled_sash_w/2,
// 					height/2.0 - scaled_sash_h/2,
// 					scaled_sash_w,
// 					scaled_sash_h);
// 	cairo_stroke (cr);

// 	// int x1 = 50;
// 	// int y1 = 50;
// 	// int x2 = 200;
// 	// int y2 = 200;
// 	// // Κύρια γραμμή
// 	// cairo_move_to(cr, x1, y1);
// 	// cairo_line_to(cr, x2, y2);
// 	// cairo_stroke(cr);

// 	// Μύτη βέλους στο (x2, y2) — δύο μικρές διαγώνιες γραμμές
// // 	double angle = atan2(y2 - y1, x2 - x1);  // κατεύθυνση της κύριας γραμμής
// // 	double arrow_len = 10.0;
// // 	double arrow_angle = 0.4; // ακτίνια, πόσο "ανοιχτή" είναι η μύτη

// // 	cairo_move_to(cr, x2, y2);
// // 	cairo_line_to(cr,
// // 		x2 - arrow_len * cos(angle - arrow_angle),
// // 		y2 - arrow_len * sin(angle - arrow_angle));
// // 	cairo_stroke(cr);

// // 	cairo_move_to(cr, x2, y2);
// // 	cairo_line_to(cr,
// // 		x2 - arrow_len * cos(angle + arrow_angle),
// // 		y2 - arrow_len * sin(angle + arrow_angle));
// // 	cairo_stroke(cr);
// }

void draw_dimension_text(cairo_t *cr, double x, double y, const char *text, double angle_rad)
{
    cairo_text_extents_t extents;

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12.0);
    cairo_text_extents(cr, text, &extents);

    cairo_save(cr);
    
    // Move origin to the target center point
    cairo_translate(cr, x, y);
    
    // Rotate text if needed (e.g., for vertical dimensions)
    if (angle_rad != 0.0) {
        cairo_rotate(cr, angle_rad);
    }

    // Center the text bounding box on (0, 0)
    double text_x = -(extents.width / 2.0 + extents.x_bearing);
    double text_y = -(extents.height / 2.0 + extents.y_bearing);

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, text);

    cairo_restore(cr);
}


void draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
{
    t_result *res = (t_result *) data;
    (void)area;

    float scale = (float)width / REFERENCE_MAX_MM;

    // --- 1. Μετατροπές σε scaled pixels ---
    float scaled_frame_w = res->frame_width * scale;
    float scaled_frame_h = res->frame_height * scale;

    float scaled_sash_w  = res->sash_width * scale;
    float scaled_sash_h  = res->sash_height * scale;

    // Ορίζουμε τα offsets σε mm (μπορείς να τα πάρεις και από το res αν υπάρχουν εκεί)
    float offset_top_mm    = 41.0f; 
    // float offset_bottom_mm = 53.0f; // Επιβεβαίωση: 41 + sash_h + 53 == frame_h

    float scaled_offset_top = offset_top_mm * scale;

    // --- 2. Υπολογισμός Θέσης Frame (Κεντραρισμένο στο canvas) ---
    float frame_x = (width  / 2.0f) - (scaled_frame_w / 2.0f);
    float frame_y = (height / 2.0f) - (scaled_frame_h / 2.0f);

    // --- 3. Υπολογισμός Θέσης Sash (Σε σχέση με το Frame) ---
    // X: Αν είναι κεντραρισμένο οριζόντια στο frame:
    float sash_x = frame_x + (scaled_frame_w - scaled_sash_w) / 2.0f;
    
    // Y: Τοποθετείται ακριβώς offset_top pixels κάτω από το πάνω μέρος του frame!
    float sash_y = frame_y + scaled_offset_top;

    // --- 4. Σχεδίαση ---
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0); /* Background */
    cairo_paint (cr);

    /* Draw Frame */
    cairo_set_line_width (cr, 1.0);
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_rectangle (cr, frame_x, frame_y, scaled_frame_w, scaled_frame_h);
    cairo_stroke (cr);

    /* Draw Sash */
    cairo_set_line_width (cr, 0.5);
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_rectangle (cr, sash_x, sash_y, scaled_sash_w, scaled_sash_h);
    cairo_stroke (cr);

	 /* Draw Sash */
   // 1. Υπολογίζουμε το offset των 60 mm σε scaled pixels
	float inner_offset = 60.0f * scale;

	// 2. Σχεδιάζουμε το εσωτερικό τετράγωνο/παράθυρο
	cairo_set_line_width (cr, 0.5);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

	cairo_rectangle (cr, 
					sash_x + inner_offset,                 // Metatopi'zi tin arches X dexia kata 60 mm
					sash_y + inner_offset,                 // Metatopi'zi tin arches Y kato kata 60 mm
					scaled_sash_w - (2.0f * inner_offset), // Mei'oni to plato's kata 120 mm (60 apo aristera + 60 apo dexia)
					scaled_sash_h - (2.0f * inner_offset)  // Mei'oni to y'psos kata 120 mm (60 apo pano + 60 apo kato)
	);

	cairo_stroke (cr);

	float inner_pixaki_offset = 75.0f * scale;

	// 2. Σχεδιάζουμε το εσωτερικό τετράγωνο/παράθυρο
	cairo_set_line_width (cr, 0.5);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

	cairo_rectangle (cr,
					sash_x + inner_pixaki_offset,
					sash_y + inner_pixaki_offset,
					scaled_sash_w - (2.0f * inner_pixaki_offset),
					scaled_sash_h - (2.0f * inner_pixaki_offset)
	);

	cairo_stroke (cr);


	cairo_set_line_width (cr, 0.5);
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

	/* 1. Πάνω-Αριστερά Γωνία */
	cairo_move_to(cr, sash_x + inner_offset, sash_y + inner_offset);
	cairo_line_to(cr, sash_x + inner_pixaki_offset, sash_y + inner_pixaki_offset);

	/* 2. Πάνω-Δεξιά Γωνία */
	cairo_move_to(cr, sash_x + scaled_sash_w - inner_offset, sash_y + inner_offset);
	cairo_line_to(cr, sash_x + scaled_sash_w - inner_pixaki_offset, sash_y + inner_pixaki_offset);

	/* 3. Κάτω-Αριστερά Γωνία */
	cairo_move_to(cr, sash_x + inner_offset, sash_y + scaled_sash_h - inner_offset);
	cairo_line_to(cr, sash_x + inner_pixaki_offset, sash_y + scaled_sash_h - inner_pixaki_offset);

	/* 4. Κάτω-Δεξιά Γωνία */
	cairo_move_to(cr, sash_x + scaled_sash_w - inner_offset, sash_y + scaled_sash_h - inner_offset);
	cairo_line_to(cr, sash_x + scaled_sash_w - inner_pixaki_offset, sash_y + scaled_sash_h - inner_pixaki_offset);




	
	/* --- ΑΡΙΣΤΕΡΗ ΠΛΕΥΡΑ  --- */
	// Πάνω αριστερά
	cairo_move_to(cr, sash_x + inner_pixaki_offset, sash_y + inner_offset);
	cairo_line_to(cr, sash_x + inner_pixaki_offset, sash_y);

	// Κάτω αριστερά
	cairo_move_to(cr, sash_x + inner_pixaki_offset, sash_y + scaled_sash_h - inner_offset);
	cairo_line_to(cr, sash_x + inner_pixaki_offset, sash_y + scaled_sash_h);


	/* --- ΔΕΞΙΑ ΠΛΕΥΡΑ --- */
	// Πάνω δεξιά
	cairo_move_to(cr, sash_x + scaled_sash_w - inner_pixaki_offset, sash_y + inner_offset);
	cairo_line_to(cr, sash_x + scaled_sash_w - inner_pixaki_offset, sash_y);

	// Κάτω δεξιά
	cairo_move_to(cr, sash_x + scaled_sash_w - inner_pixaki_offset, sash_y + scaled_sash_h - inner_offset);
	cairo_line_to(cr, sash_x + scaled_sash_w - inner_pixaki_offset, sash_y + scaled_sash_h);

	cairo_stroke(cr);




	// /**************8Draw dimention up ***************/
	// // left 
	// cairo_move_to(cr, frame_x, frame_y - 40);
	// cairo_line_to(cr,  frame_x,  frame_y - 10);
	// cairo_stroke(cr);

	// /* Draw dimension line - Top Right Corner */
	// cairo_move_to(cr, frame_x + scaled_frame_w, frame_y - 40);
	// cairo_line_to(cr, frame_x + scaled_frame_w, frame_y - 10);
	// cairo_stroke(cr);
	
	// /* 3. Horizontal dimension line connecting them */
	// cairo_move_to(cr, frame_x, frame_y - 25);
	// cairo_line_to(cr, frame_x + scaled_frame_w , frame_y - 25);
	// cairo_stroke(cr);

	// /*******************/

	// float dim_offset_x = frame_x + scaled_frame_w; // Right edge of the frame

	// /* 1. Top-Right extension line (extends horizontally to the right) */
	// cairo_move_to(cr, dim_offset_x + 10, frame_y);
	// cairo_line_to(cr, dim_offset_x + 40, frame_y);

	// /* 2. Bottom-Right extension line (extends horizontally to the right) */
	// cairo_move_to(cr, dim_offset_x + 10, frame_y + scaled_frame_h);
	// cairo_line_to(cr, dim_offset_x + 40, frame_y + scaled_frame_h);

	// /* 3. Vertical dimension line connecting them */
	// cairo_move_to(cr, dim_offset_x + 25, frame_y);
	// cairo_line_to(cr, dim_offset_x + 25, frame_y + scaled_frame_h);

	// cairo_stroke(cr);

	//#####################################################

	char width_str[32];
	char height_str[32];
	snprintf(width_str, sizeof(width_str), "%.0f mm", res->frame_width);
	snprintf(height_str, sizeof(height_str), "%.0f mm", res->frame_height);

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); /* Black color for dimensions */
	cairo_set_line_width(cr, 1.0);

	/* --- TOP DIMENSION (WIDTH) --- */
	double top_dim_y = frame_y - 25; // Height of horizontal line

	// Extension lines
	cairo_move_to(cr, frame_x, frame_y - 35);
	cairo_line_to(cr, frame_x, frame_y - 5);
	cairo_move_to(cr, frame_x + scaled_frame_w, frame_y - 35);
	cairo_line_to(cr, frame_x + scaled_frame_w, frame_y - 5);

	// Main dimension line
	cairo_move_to(cr, frame_x, top_dim_y);
	cairo_line_to(cr, frame_x + scaled_frame_w, top_dim_y);
	cairo_stroke(cr);

	// Draw Width Text (Centered above/on the top line)
	draw_dimension_text(cr, 
						frame_x + (scaled_frame_w / 2.0), // Center X
						top_dim_y - 12,                    // Slightly above the line
						width_str, 
						0.0);                             // Horizontal text


	/* --- RIGHT DIMENSION (HEIGHT) --- */
	double right_dim_x = frame_x + scaled_frame_w + 25; // X position of vertical line

	// Extension lines
	cairo_move_to(cr, frame_x + scaled_frame_w + 5, frame_y);
	cairo_line_to(cr, frame_x + scaled_frame_w + 35, frame_y);
	cairo_move_to(cr, frame_x + scaled_frame_w + 5, frame_y + scaled_frame_h);
	cairo_line_to(cr, frame_x + scaled_frame_w + 35, frame_y + scaled_frame_h);

	// Main dimension line
	cairo_move_to(cr, right_dim_x, frame_y);
	cairo_line_to(cr, right_dim_x, frame_y + scaled_frame_h);
	cairo_stroke(cr);

	// Draw Height Text (Rotated 90 degrees along the vertical line)
	draw_dimension_text(cr, 
						right_dim_x + 12,                  // Slightly to the right of line
						frame_y + (scaled_frame_h / 2.0),  // Center Y
						height_str, 
						-G_PI / 2.0);                      // Rotated -90° (reads bottom-to-top)

}


void activate(GtkApplication *app, gpointer data)
{
   
	t_window	*win = (t_window *) data;
	t_result	*res = g_new0(t_result, 1);

	
    GtkWidget	*window = gtk_application_window_new(app);
    GtkWidget	*area = gtk_drawing_area_new();
    GtkWidget	*box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
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
    // gtk_window_set_default_size(GTK_WINDOW(window), 1200, 1200);
    gtk_window_set_child(GTK_WINDOW(window), box);
	
	
	gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_present(GTK_WINDOW(window));

}
