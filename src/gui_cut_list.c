#include "types.h"
#include "calculator.h"
#include "gui.h"


//	Log over dimentions lenght before processing
#define LENGTH_ALLOWANCE_MM 50.0f

// Log width before processing
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
    GtkWidget *vsep;
    GtkWidget *lbl_log_group;
    GtkWidget *lbl_fin_group;
	GtkWidget *export_btn;
	GtkWidget *title_row;
	GtkWidget *export_box;
    int row = 0;

    panel = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(panel, 200, -1);
    set_margin(panel, 40);
	
	// --- Title row: "Cutting List" label + Export button, spaced apart ---
 	title_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  	gtk_widget_set_margin_bottom(title_row, 20);

    title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title), "<b>Cutting List</b>");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_widget_set_margin_bottom(title, 20);
    gtk_box_append(GTK_BOX(panel), title);

	export_btn = gtk_button_new();
    export_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_box_append(GTK_BOX(export_box), gtk_image_new_from_file("icons/export.svg"));
    gtk_box_append(GTK_BOX(export_box), gtk_label_new("Export CSV"));
    gtk_button_set_child(GTK_BUTTON(export_btn), export_box);
    gtk_widget_set_tooltip_text(export_btn, "Export cutting list to CSV");
    g_signal_connect(export_btn, "clicked", G_CALLBACK(on_export_csv_clicked), ctx);
    gtk_box_append(GTK_BOX(title_row), export_btn);

	gtk_box_append(GTK_BOX(panel), title_row); 

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 12);

    lbl_log_group = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_log_group), "<b>Log</b>");
    gtk_grid_attach(GTK_GRID(grid), lbl_log_group, 4, row, 1, 1);

    lbl_fin_group = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_fin_group), "<b>Finished</b>");
    gtk_grid_attach(GTK_GRID(grid), lbl_fin_group, 5, row, 1, 1);
    row++;

    add_grid_cell(grid, "Qty", 0, row, TRUE);
    add_grid_cell(grid, "Item", 1, row, TRUE);
    add_grid_cell(grid, "Material", 2, row, TRUE);
    add_grid_cell(grid, "Over dimention(mm)", 4, row, TRUE);
    add_grid_cell(grid, "Finish dimention(mm)", 5, row, TRUE);
    row++;

    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 6, 1);

	// FRAME
    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Frame Head", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_head_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_frame_head_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Frame Sill", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_sill_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_frame_sill_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Frame Left Jamb", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_left_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_frame_left_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Frame Right Jamb", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_frame_right_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_frame_right_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 6, 1);

	// SASH
    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Sash Top Rail", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_top_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_sash_top_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Sash Bottom Rail", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_bottom_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_sash_bottom_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Sash Left Stile", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_left_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_sash_left_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Sash Right Stile", 1, row, FALSE);
    add_grid_cell(grid, "Oak", 2, row, FALSE);
    ctx->cut_list_ui.lbl_sash_right_raw = add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_sash_right_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(GTK_GRID(grid), sep, 0, row++, 6, 1);

    // GLASS
    add_grid_cell(grid, "1x", 0, row, FALSE);
    add_grid_cell(grid, "Glass Pane", 1, row, FALSE);
    add_grid_cell(grid, "Glass", 2, row, FALSE);
    add_grid_cell(grid, "-", 4, row, FALSE);
    ctx->cut_list_ui.lbl_glass_net = add_grid_cell(grid, "-", 5, row, FALSE);
    row++;

    vsep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_widget_set_size_request(vsep, 1, -1);
    gtk_grid_attach(GTK_GRID(grid), vsep, 3, 0, 1, row);

    update_cut_list_ui(ctx);

    scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), grid);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_box_append(GTK_BOX(panel), scroll);

    return panel;
}

static void set_label_colored(GtkWidget *lbl, const char *text, const char *color_hex)
{
    char *markup;

    if (!GTK_IS_LABEL(lbl))
        return;
    markup = g_markup_printf_escaped(
        "<span foreground=\"%s\" weight=\"bold\">%s</span>",
        color_hex, text);
    gtk_label_set_markup(GTK_LABEL(lbl), markup);
    g_free(markup);
}

void update_cut_list_ui(t_app_context *ctx)
{
    if (!ctx)
        return;

    char raw_str[64];
    char net_str[64];
	
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);
	

    // Horizontal FRAME
    snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
	ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_horizontal_cut + LENGTH_ALLOWANCE_MM);
    snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
	ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_horizontal_cut);
	
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_raw))
	    gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_net))
	    gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_net), net_str);
	set_label_colored(ctx->cut_list_ui.lbl_frame_head_net, net_str, "#5C3D2E");

    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_net))
    {
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_net), net_str);
    }
	set_label_colored(ctx->cut_list_ui.lbl_frame_sill_net, net_str, "#5C3D2E");

    // VERTICAL FRAME
    snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
             ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM, ctx->res.frame_vertical_cut + LENGTH_ALLOWANCE_MM);
    snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
             ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_vertical_cut);

    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_left_raw))
    {
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_left_raw), raw_str);
    }
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_left_net))
    {
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_left_net), net_str);
    }
	set_label_colored(ctx->cut_list_ui.lbl_frame_left_net, net_str, "#5C3D2E");

    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_right_raw))
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_right_raw), raw_str);
    if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_right_net))
    {
        gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_right_net), net_str);
    }
	set_label_colored(ctx->cut_list_ui.lbl_frame_right_net, net_str, "#5C3D2E");

 

    if (ctx->win.mechanism != MECH_FIXED)
    {
        // Horizontal Sash
        snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_horizontal_cut + LENGTH_ALLOWANCE_MM);
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_horizontal_cut);

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_top_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_top_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_top_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_top_net), net_str);
		set_label_colored(ctx->cut_list_ui.lbl_sash_top_net, net_str, "#173925");

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_bottom_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_bottom_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_bottom_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_bottom_net), net_str);
		set_label_colored(ctx->cut_list_ui.lbl_sash_bottom_net, net_str, "#173925");

        // Vertical Sash
        snprintf(raw_str, sizeof(raw_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM, ctx->res.sash_height + LENGTH_ALLOWANCE_MM);
        snprintf(net_str, sizeof(net_str), "%.0f x %.0f x %.0f",
                 ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_height);

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_left_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_left_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_left_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_left_net), net_str);
			set_label_colored(ctx->cut_list_ui.lbl_sash_left_net, net_str, "#173925");

        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_right_raw))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_right_raw), raw_str);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_sash_right_net))
            gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_sash_right_net), net_str);
			set_label_colored(ctx->cut_list_ui.lbl_sash_right_net, net_str, "#173925");
    }
    else
    {
        const char *dash = "-";
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_raw))    gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_raw), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_head_net))    gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_head_net), dash);
        if (GTK_IS_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw)) 	gtk_label_set_text(GTK_LABEL(ctx->cut_list_ui.lbl_frame_sill_raw), dash);
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
		set_label_colored(ctx->cut_list_ui.lbl_glass_net, net_str, "#2e405c");
    }
}