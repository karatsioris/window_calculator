#include "types.h"
#include "calculator.h"
#include "gui.h"


// Περιθώριο μήκους για αρχικό κόψιμο ξύλου (mm)
#define LENGTH_ALLOWANCE_MM 50.0f
// Αρχικό πλάτος πρίσματος πριν το πλάνισμα/διαμόρφωση (mm)
#define RAW_FRAME_WIDTH_MM  80.0f
#define RAW_SASH_WIDTH_MM   80.0f

static GtkWidget	*add_grid_cell(GtkWidget *grid, const char *text, int col, int row, gboolean is_header)
{
	GtkWidget *label = gtk_label_new(NULL);
	char buf[256];

	if(is_header)
		snprintf(buf, sizeof(buf), "<b>%s</b>", text);
	else
		snprintf(buf, sizeof(buf), "%s", text);

	gtk_label_set_markup(GTK_LABEL(label), buf);
	
	if(is_header)
    {
		gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
	}
    else if(col == 0)
    {
		gtk_widget_set_halign(label, GTK_ALIGN_START);
	}
    else
    {
		gtk_widget_set_halign(label, GTK_ALIGN_CENTER);
	}
	
	gtk_widget_set_margin_start(label, 6);
	gtk_widget_set_margin_end(label, 6);
	gtk_widget_set_margin_top(label, 4);
	gtk_widget_set_margin_bottom(label, 4);

	gtk_grid_attach(GTK_GRID(grid), label, col, row, 1, 1);
	return label;
}

GtkWidget *build_cut_list_panel(t_app_context *ctx)
{
    GtkWidget *panel;
    GtkWidget *title;
    GtkWidget *scroll;
    GtkWidget *grid;
    GtkWidget *sep;
    int row = 0;

    panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(panel, 460, -1);
    set_margin(panel, 40);

    // Title
    title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<b>Cutting List</b>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(title, 20);
    gtk_box_append(GTK_BOX(panel), title);

    // Grid Container
    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);

    // --- HEADERS (4 Columns) ---
    add_grid_cell(grid, "Item", 0, row, TRUE);
    add_grid_cell(grid, "Material", 1, row, TRUE);
    add_grid_cell(grid, "Qty", 2, row, TRUE);
    add_grid_cell(grid, "Over dim(mm)",  3, row, TRUE);
    add_grid_cell(grid, "Finish dim(mm)", 4, row, TRUE);
    row++;

    // Separator
    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 5, 1);

    // --- 1. FRAME (Unpaired) ---
    // Frame Head (Horizontal)
    add_grid_cell(grid, "Frame Head", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_head_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_frame_head_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Frame Sill (Horizontal)
    add_grid_cell(grid, "Frame Sill", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_sill_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_frame_sill_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Frame Left Jamb (Vertical)
    add_grid_cell(grid, "Frame Left Jamb", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_left_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_frame_left_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Frame Right Jamb (Vertical)
    add_grid_cell(grid, "Frame Right Jamb", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_right_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_frame_right_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Separator before Sash
    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 5, 1);

    // --- 2. SASH (Unpaired) ---
    // Sash Top Rail
    add_grid_cell(grid, "Sash Top Rail", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_top_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_sash_top_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Sash Bottom Rail
    add_grid_cell(grid, "Sash Bottom Rail", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_bottom_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_sash_bottom_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Sash Left Stile
    add_grid_cell(grid, "Sash Left Stile", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_left_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_sash_left_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Sash Right Stile
    add_grid_cell(grid, "Sash Right Stile", 0, row, FALSE);
    add_grid_cell(grid, "Oak", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_right_raw = add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_sash_right_net = add_grid_cell(grid, "-", 4, row, FALSE);
    row++;

    // Separator before Glass
    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 5, 1);

    // --- 3. GLASS ---
    add_grid_cell(grid, "Glass Pane", 0, row, FALSE);
    add_grid_cell(grid, "Glass", 1, row, FALSE);
    add_grid_cell(grid, "1x", 2, row, FALSE);
    add_grid_cell(grid, "-", 3, row, FALSE);
    ctx->cut_list_ui.lbl_glass_net = add_grid_cell(grid, "-", 4, row, FALSE);

    // Initial update
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
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

	if(ctx->win.mechanism == MECH_FIXED)
	{
		
	}

    // Head (Horizontal cut)
    snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
             ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_horizontal_cut + LENGTH_ALLOWANCE_MM);
    snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
             ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_horizontal_cut);

    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_raw))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_net))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_net), net_str);

    // Sill (Horizontal cut - allows separate dimensions in asymmetric setups later)
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_net))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_net), net_str);

    // Left Jamb (Vertical cut)
    snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
             ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_vertical_cut + LENGTH_ALLOWANCE_MM);
    snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
             ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_vertical_cut);

    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_left_raw))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_left_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_left_net))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_left_net), net_str);

    // Right Jamb (Vertical cut)
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_right_raw))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_right_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_right_net))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_right_net), net_str);

 

    if (ctx->win.mechanism != MECH_FIXED)
    {
        // Horizontal Sash Members (Top & Bottom Rails)
        snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_horizontal_cut + LENGTH_ALLOWANCE_MM);
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_horizontal_cut);

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_top_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_top_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_top_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_top_net), net_str);

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_bottom_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_bottom_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_bottom_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_bottom_net), net_str);

        // Vertical Sash Members (Left & Right Stiles)
        snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_height + LENGTH_ALLOWANCE_MM);
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_height);

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_left_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_left_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_left_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_left_net), net_str);

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_right_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_right_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_right_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_right_net), net_str);
    }
    else
    {
        // Fixed window has no sash elements
        const char *dash = "-";
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_raw))    gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_raw), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_net))    gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_net), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw)) gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_net))   gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_net), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_left_raw)) gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_left_raw), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_left_net))   gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_left_net), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_right_raw))  gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_right_raw), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_right_net))  gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_right_net), dash);
    }

  
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_glass_net))
    {
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f mm", ctx->res.glass_width, ctx->res.glass_height);
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_glass_net), net_str);
    }
}