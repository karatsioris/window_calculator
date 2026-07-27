#include "types.h"
#include "calculator.h"
#include "gui.h"


// Περιθώριο μήκους για αρχικό κόψιμο ξύλου (mm)
#define LENGTH_ALLOWANCE_MM 50.0f
// Αρχικό πλάτος πρίσματος πριν το πλάνισμα/διαμόρφωση (mm)
#define RAW_FRAME_WIDTH_MM  85.0f
#define RAW_SASH_WIDTH_MM   80.0f


// static void add_grid_cell(GtkWidget *grid, const char *text, int col, int row, gboolean is_header)
// {
// 	GtkWidget *label = gtk_label_new(NULL);
// 	char buf[256];

// 	if (is_header)
// 		snprintf(buf, sizeof(buf), "<b>%s</b>", text);
// 	else
// 		snprintf(buf, sizeof(buf), "%s", text);

// 	gtk_label_set_markup(GTK_LABEL(label), buf);
// 	if (is_header)
//         gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
//     else if (col == 0)
//         gtk_widget_set_halign(label, GTK_ALIGN_START);
//     else
//         gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
	
// 	gtk_widget_set_margin_start(label, 6);
// 	gtk_widget_set_margin_end(label, 6);
// 	gtk_widget_set_margin_top(label, 4);
// 	gtk_widget_set_margin_bottom(label, 4);

// 	gtk_grid_attach(GTK_GRID(grid), label, col, row, 1, 1);
// }


static GtkWidget	*add_grid_cell(GtkWidget *grid, const char *text, int col, int row, gboolean is_header)
{
	GtkWidget *label = gtk_label_new(NULL);
	char buf[256];

	if (is_header)
		snprintf(buf, sizeof(buf), "<b>%s</b>", text);
	else
		snprintf(buf, sizeof(buf), "%s", text);

	gtk_label_set_markup(GTK_LABEL(label), buf);
	
	if (is_header)
        gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
    else if (col == 0)
        gtk_widget_set_halign(label, GTK_ALIGN_START);
    else
        gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
	
	gtk_widget_set_margin_start(label, 6);
	gtk_widget_set_margin_end(label, 6);
	gtk_widget_set_margin_top(label, 4);
	gtk_widget_set_margin_bottom(label, 4);

	gtk_grid_attach(GTK_GRID(grid), label, col, row, 1, 1);
	return label;
}


// GtkWidget *build_cut_list_panel(t_app_context *ctx)
// {
//     GtkWidget *panel;
//     GtkWidget *title;
//     GtkWidget *scroll;
//     GtkWidget *grid;
//     int row = 0;

//     panel = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
//     gtk_widget_set_size_request(panel, 500, -1);
// 	set_margin(panel, 10);

//     // Τίτλος
//     title = gtk_label_new(NULL);
//     gtk_label_set_markup(GTK_LABEL(title), "<b>Cutting List</b>");
//     gtk_widget_set_halign(title, GTK_ALIGN_START);
//     gtk_widget_set_margin_bottom(title, 5);
// 	// set_margin(title, 10);
//     gtk_box_append(GTK_BOX(panel), title);

//     // GtkGrid για μορφή Πίνακα
//     grid = gtk_grid_new();
//     gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
//     gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
// 	// set_margin(grid, 15);

//    // --- ΕΠΙΚΕΦΑΛΙΔΕΣ (5 Στήλες) ---
//     add_grid_cell(grid, "Τεμάχιο",          0, row, TRUE);
//     add_grid_cell(grid, "Ποσ.",             1, row, TRUE);
//     add_grid_cell(grid, "Αρχικό Ξύλο (mm)",  2, row, TRUE); // Gross [T x W x L]
//     add_grid_cell(grid, "Τελικό Προφίλ (mm)", 3, row, TRUE); // Net [T x W x L]
//     row++;

//     // Διαχωριστική γραμμή
//     GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
//     gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 4, 1);

//     char raw_str[64];
//     char net_str[64];

//     // --- 1. ΚΑΣΑ (FRAME) ---
//     // Ορθοστάτες Κάσας (Vertical)
//     snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f", 
//              ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_vertical_cut + LENGTH_ALLOWANCE_MM);
//     snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f", 
//              ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_vertical_cut);

//     add_grid_cell(grid, "Κάσα (Κάθετη)", 0, row, FALSE);
//     add_grid_cell(grid, "2x", 1, row, FALSE);
//     add_grid_cell(grid, raw_str, 2, row, FALSE);
//     add_grid_cell(grid, net_str, 3, row, FALSE);
//     row++;

//     // Τραβέρσες Κάσας (Horizontal)
//     snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f", 
//              ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_horizontal_cut + LENGTH_ALLOWANCE_MM);
//     snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f", 
//              ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_horizontal_cut);

//     add_grid_cell(grid, "Κάσα (Οριζόντια)", 0, row, FALSE);
//     add_grid_cell(grid, "2x", 1, row, FALSE);
//     add_grid_cell(grid, raw_str, 2, row, FALSE);
//     add_grid_cell(grid, net_str, 3, row, FALSE);

//     row++;

//     // --- 2. ΦΥΛΛΟ (SASH) ---
//     if (ctx->win.mechanism != MECH_FIXED)
//     {
//         // Μπόγια Φύλλου (Vertical)
//         snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f", 
//                  ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_height + LENGTH_ALLOWANCE_MM);
//         snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f", 
//                  ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_height);

//         add_grid_cell(grid, "Φύλλο (Μπόγι)", 0, row, FALSE);
//         add_grid_cell(grid, "2x", 1, row, FALSE);
//         add_grid_cell(grid, raw_str, 2, row, FALSE);
//         add_grid_cell(grid, net_str, 3, row, FALSE);
//         row++;

//         // Τραβέρσες Φύλλου (Horizontal)
//         snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f", 
//                  ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_width + LENGTH_ALLOWANCE_MM);
//         snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f", 
//                  ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_width);

//         add_grid_cell(grid, "Φύλλο (Τραβέρσα)", 0, row, FALSE);
//         add_grid_cell(grid, "2x", 1, row, FALSE);
//         add_grid_cell(grid, raw_str, 2, row, FALSE);
//         add_grid_cell(grid, net_str, 3, row, FALSE);
//         row++;
//     }

// 	// Διαχωριστική γραμμή πριν το Τζάμι
//     sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
//     gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 5, 1);

//     // --- 3. ΥΑΛΟΠΙΝΑΚΑΣ (GLASS) ---
//     snprintf(net_str, sizeof(net_str), "%.0f x %.0f mm", ctx->res.glass_width, ctx->res.glass_height);
//     add_grid_cell(grid, "Υαλοπίνακας", 0, row, FALSE);
//     add_grid_cell(grid, "1x", 1, row, FALSE);
//     add_grid_cell(grid, "-", 2, row, FALSE);
//     add_grid_cell(grid, net_str, 3, row, FALSE);

//     // Scrolled Window
//     scroll = gtk_scrolled_window_new();
//     gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), grid);
//     gtk_widget_set_vexpand(scroll, TRUE);
//     gtk_box_append(GTK_BOX(panel), scroll);

//     return (panel);
// }

GtkWidget *build_cut_list_panel(t_app_context *ctx)
{
    GtkWidget *panel;
    GtkWidget *title;
    GtkWidget *scroll;
    GtkWidget *grid;
    GtkWidget *sep;
    int row = 0;

    panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(panel, 450, -1);

    // Title
    title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<b>Cutting List</b>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(title, 10);
    gtk_box_append(GTK_BOX(panel), title);

    // Grid Container
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);

    // --- HEADERS (4 Columns) ---
    add_grid_cell(grid, "Τεμάχιο",          0, row, TRUE);
    add_grid_cell(grid, "Ποσ.",             1, row, TRUE);
    add_grid_cell(grid, "Αρχικό Ξύλο (mm)",  2, row, TRUE); // Gross
    add_grid_cell(grid, "Τελικό Προφίλ (mm)", 3, row, TRUE); // Net
    row++;

    // Separator
    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 4, 1);

    // --- 1. FRAME ---
    // Vertical Frame
    add_grid_cell(grid, "Κάσα (Κάθετη)", 0, row, FALSE);
    add_grid_cell(grid, "2x", 1, row, FALSE);
    ctx->cut_list_ui.lbl_frame_v_raw = add_grid_cell(grid, "-", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_v_net = add_grid_cell(grid, "-", 3, row, FALSE);
    row++;

    // Horizontal Frame
    add_grid_cell(grid, "Κάσα (Οριζόντια)", 0, row, FALSE);
    add_grid_cell(grid, "2x", 1, row, FALSE);
    ctx->cut_list_ui.lbl_frame_h_raw = add_grid_cell(grid, "-", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_h_net = add_grid_cell(grid, "-", 3, row, FALSE);
    row++;

    // --- 2. SASH ---
    // Always attach sash row widgets so pointers are valid GTK widgets
    add_grid_cell(grid, "Φύλλο (Μπόγι)", 0, row, FALSE);
    add_grid_cell(grid, "2x", 1, row, FALSE);
    ctx->cut_list_ui.lbl_sash_v_raw = add_grid_cell(grid, "-", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_v_net = add_grid_cell(grid, "-", 3, row, FALSE);
    row++;

    add_grid_cell(grid, "Φύλλο (Τραβέρσα)", 0, row, FALSE);
    add_grid_cell(grid, "2x", 1, row, FALSE);
    ctx->cut_list_ui.lbl_sash_h_raw = add_grid_cell(grid, "-", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_h_net = add_grid_cell(grid, "-", 3, row, FALSE);
    row++;

    // Separator before Glass
    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 4, 1);

    // --- 3. GLASS ---
    add_grid_cell(grid, "Υαλοπίνακας", 0, row, FALSE);
    add_grid_cell(grid, "1x", 1, row, FALSE);
    add_grid_cell(grid, "-", 2, row, FALSE);
    ctx->cut_list_ui.lbl_glass_net = add_grid_cell(grid, "-", 3, row, FALSE);

    // Initial update of text values
    update_cut_list_ui(ctx);

    // Scrolled Window
    scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), grid);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_append(GTK_BOX(panel), scroll);

    return panel;
}


void update_cut_list_ui(t_app_context *ctx)
{
    if (!ctx)
        return;

    char raw_str[64];
    char net_str[64];

    // 1. Recalculate dimensions in memory
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg);

    // --- FRAME VERTICAL ---
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_v_raw)) {
        snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_vertical_cut + LENGTH_ALLOWANCE_MM);
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_v_raw), raw_str);
    }
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_v_net)) {
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_vertical_cut);
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_v_net), net_str);
    }

    // --- FRAME HORIZONTAL ---
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_h_raw)) {
        snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_horizontal_cut + LENGTH_ALLOWANCE_MM);
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_h_raw), raw_str);
    }
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_h_net)) {
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_horizontal_cut);
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_h_net), net_str);
    }

    // --- SASH (VERTICAL & HORIZONTAL) ---
    if (ctx->win.mechanism != MECH_FIXED)
    {
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_v_raw)) {
            snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                     ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_height + LENGTH_ALLOWANCE_MM);
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_v_raw), raw_str);
        }
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_v_net)) {
            snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                     ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_height);
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_v_net), net_str);
        }

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_h_raw)) {
            snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                     ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_width + LENGTH_ALLOWANCE_MM);
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_h_raw), raw_str);
        }
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_h_net)) {
            snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                     ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_width);
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_h_net), net_str);
        }
    }
    else
    {
        // For fixed windows, clear or display "-" for sash dimensions
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_v_raw)) gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_v_raw), "-");
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_v_net)) gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_v_net), "-");
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_h_raw)) gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_h_raw), "-");
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_h_net)) gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_h_net), "-");
    }

    // --- GLASS ---
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_glass_net)) {
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f mm", ctx->res.glass_width, ctx->res.glass_height);
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_glass_net), net_str);
    }
}