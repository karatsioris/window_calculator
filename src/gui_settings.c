#include "gui.h"


static GtkSpinButton *create_setting_spinbutton(double min, double max, double step, double current_val, GtkWidget *box)
{
    GtkWidget *sb = gtk_spin_button_new_with_range(min, max, step);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb), current_val);
    gtk_box_append(GTK_BOX(box), sb);
    return GTK_SPIN_BUTTON(sb);
}

static GtkSpinButton *add_setting_row(const char *label_text, double min, double max, double step, double val, GtkWidget *parent_box)
{
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *label = gtk_label_new(label_text);
    gtk_widget_set_hexpand(label, TRUE);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
	gtk_box_append(GTK_BOX(hbox), label);

    GtkSpinButton *sb = create_setting_spinbutton(min, max, step, val, hbox);

    gtk_box_append(GTK_BOX(parent_box), hbox);
    return sb;
}

static void on_save_settings_clicked(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx) return;

    // Frame Parameters
    ctx->cfg.frame.depth        = (float)gtk_spin_button_get_value(ctx->setting_spins.frame_depth);
    ctx->cfg.frame.width        = (float)gtk_spin_button_get_value(ctx->setting_spins.frame_width);
    ctx->cfg.frame.rebate_width = (float)gtk_spin_button_get_value(ctx->setting_spins.frame_rebate_width);
	ctx->cfg.frame.rebate_depth = (float)gtk_spin_button_get_value(ctx->setting_spins.frame_rebate_depth);
    ctx->cfg.frame.sill_rebate  = (float)gtk_spin_button_get_value(ctx->setting_spins.frame_sill_rebate);

    // Sash Parameters
    ctx->cfg.sash.depth          = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_depth);
    ctx->cfg.sash.width          = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_width);
	ctx->cfg.sash.rebate_width   = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_rebate_width);
    ctx->cfg.sash.rebate_depth   = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_rebate_depth);
    ctx->cfg.sash.overlap        = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_overlap);
    ctx->cfg.sash.glass_rebate_w = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_glass_rebate_w);
    ctx->cfg.sash.bead_width     = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_bead_width);

    // Gaps / Air Clearance
    ctx->cfg.gaps.air_gap        = (float)gtk_spin_button_get_value(ctx->setting_spins.gaps_air_gap);
    ctx->cfg.gaps.glass_clearance = (float)gtk_spin_button_get_value(ctx->setting_spins.gaps_glass_clearance);

    save_config_to_ini("config.ini", &ctx->cfg);

    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);
    update_cut_list_ui(ctx);
    if (ctx->area)
	{
        gtk_widget_queue_draw(ctx->area);
    }
}


static void add_section_header(const char *title, GtkWidget *box)
{
    GtkWidget *sep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(box), sep);

    GtkWidget *label = gtk_label_new(NULL);
    char markup[128];
    snprintf(markup, sizeof(markup), "<b>%s</b>", title);
    gtk_label_set_markup(GTK_LABEL(label), markup);
    gtk_widget_set_halign(label, GTK_ALIGN_START);
    gtk_widget_set_margin_top(label, 6);
    gtk_widget_set_margin_bottom(label, 4);

    gtk_box_append(GTK_BOX(box), label);
}

static GtkWidget *build_diagram_panel(const char *svg_path)
{
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_hexpand(left_box, TRUE);
    gtk_widget_set_vexpand(left_box, TRUE);

    GtkWidget *lbl_diagram = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_diagram), "<b>Profile Cross-Section Diagram</b>");
    gtk_box_append(GTK_BOX(left_box), lbl_diagram);

    GtkWidget *picture = gtk_picture_new_for_filename(svg_path);
    gtk_picture_set_can_shrink(GTK_PICTURE(picture), TRUE);
    gtk_widget_set_vexpand(picture, TRUE);
    gtk_box_append(GTK_BOX(left_box), picture);

    return left_box;
}
static void build_frame_section(t_app_context *ctx, GtkWidget *box)
{
    add_section_header("Frame Specifications (mm)", box);
    ctx->setting_spins.frame_width        = add_setting_row("A - Frame Face Width:", 40, 150, 1, ctx->cfg.frame.width, box);
    ctx->setting_spins.frame_depth        = add_setting_row("B - Frame Depth:", 40, 150, 1, ctx->cfg.frame.depth, box);
    ctx->setting_spins.frame_rebate_width = add_setting_row("C - Frame Rebate Width:", 10, 60, 1, ctx->cfg.frame.rebate_width, box);
    ctx->setting_spins.frame_rebate_depth = add_setting_row("D - Frame Rebate Depth:", 5, 30, 1, ctx->cfg.frame.rebate_depth, box);
    ctx->setting_spins.frame_sill_rebate  = add_setting_row("E - Sill Rebate:", 0, 50, 1, ctx->cfg.frame.sill_rebate, box);
}

static void build_sash_section(t_app_context *ctx, GtkWidget *box)
{
    add_section_header("Sash Specifications (mm)", box);
    ctx->setting_spins.sash_width          = add_setting_row("F - Sash Face Width:", 40, 150, 1, ctx->cfg.sash.width, box);
    ctx->setting_spins.sash_depth          = add_setting_row("G - Sash Depth:", 40, 150, 1, ctx->cfg.sash.depth, box);
    ctx->setting_spins.sash_rebate_width   = add_setting_row("H - Sash Rebate Width:", 5, 40, 1, ctx->cfg.sash.rebate_width, box);
    ctx->setting_spins.sash_rebate_depth   = add_setting_row("I - Sash Rebate Depth:", 5, 30, 1, ctx->cfg.sash.rebate_depth, box);
    ctx->setting_spins.sash_overlap        = add_setting_row("J - Sash Overlap:", 0, 30, 1, ctx->cfg.sash.overlap, box);
    ctx->setting_spins.sash_glass_rebate_w = add_setting_row("K - Glass Rebate Width:", 5, 40, 1, ctx->cfg.sash.glass_rebate_w, box);
    ctx->setting_spins.sash_bead_width     = add_setting_row("L - Glazing Bead Width:", 5, 30, 1, ctx->cfg.sash.bead_width, box);
}

static void build_gaps_section(t_app_context *ctx, GtkWidget *box)
{
    add_section_header("Gaps and Clearances (mm)", box);
    ctx->setting_spins.gaps_air_gap         = add_setting_row("M - Air Clearance Gap:", 1, 20, 1, ctx->cfg.gaps.air_gap, box);
    ctx->setting_spins.gaps_glass_clearance = add_setting_row("N - Glass Clearance Gap:", 1, 15, 1, ctx->cfg.gaps.glass_clearance, box);
}

void on_settings_clicked(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx) return;

    // Dialog Setup
    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Profile & System Setup");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(ctx->main_window));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 1400, 950);

    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    set_margin(main_hbox, 16);

    // Left Side (Diagram Panel)
    GtkWidget *left_panel = build_diagram_panel("section.svg");
    gtk_box_append(GTK_BOX(main_hbox), left_panel);

    // Separator
    gtk_box_append(GTK_BOX(main_hbox), gtk_separator_new(GTK_ORIENTATION_VERTICAL));

    // Right Side (Settings Controls)
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(scroll, TRUE);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    set_margin(right_box, 8);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), right_box);

    // Build Form Sections
    build_frame_section(ctx, right_box);
    build_sash_section(ctx, right_box);
    build_gaps_section(ctx, right_box);

    // Save Button
    GtkWidget *btn_save = gtk_button_new_with_label("Save System Defaults");
    gtk_widget_set_margin_top(btn_save, 14);
    g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_settings_clicked), ctx);
    gtk_box_append(GTK_BOX(right_box), btn_save);

    gtk_box_append(GTK_BOX(main_hbox), scroll);

    // Present Window
    gtk_window_set_child(GTK_WINDOW(dialog), main_hbox);
    gtk_window_present(GTK_WINDOW(dialog));
}