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

void open_profile_settings_dialog(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;
    if (!ctx) return;

    // Main Window Setup
    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Profile & System Setup");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(ctx->main_window));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 1400,950);

    // Root Horizontal Layout Box (Left: Diagram | Right: Form Inputs)
    GtkWidget *main_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    set_margin(main_hbox, 16);


    // LEFT COLUMN: SVG Reference Diagram
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_widget_set_hexpand(left_box, TRUE);
    gtk_widget_set_vexpand(left_box, TRUE);

    GtkWidget *lbl_diagram = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_diagram), "<b>Profile Cross-Section Diagram</b>");
    gtk_box_append(GTK_BOX(left_box), lbl_diagram);

    // Use GtkPicture so the SVG resizes smoothly without pixelation
    GtkWidget *picture = gtk_picture_new_for_filename("section.svg");
    gtk_picture_set_can_shrink(GTK_PICTURE(picture), TRUE);
    gtk_widget_set_vexpand(picture, TRUE);
    gtk_box_append(GTK_BOX(left_box), picture);

    gtk_box_append(GTK_BOX(main_hbox), left_box);

    // Separator between image and controls
    GtkWidget *v_sep = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_append(GTK_BOX(main_hbox), v_sep);


    // RIGHT COLUMN: Scrollable Form Controls
    GtkWidget *scroll = gtk_scrolled_window_new();
    gtk_widget_set_hexpand(scroll, TRUE);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    set_margin(right_box, 8);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), right_box);

    // FRAME SECTION
    GtkWidget *lbl_frame = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_frame), "<b>Frame Specifications (mm)</b>");
    gtk_widget_set_halign(lbl_frame, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right_box), lbl_frame);

    ctx->setting_spins.frame_width        = add_setting_row("A - Frame Face Width:", 40, 150, 1, ctx->cfg.frame.width, right_box);
    ctx->setting_spins.frame_depth        = add_setting_row("B - Frame Depth:", 40, 150, 1, ctx->cfg.frame.depth, right_box);
    ctx->setting_spins.frame_rebate_width = add_setting_row("C - Frame Rebate Width:", 10, 60, 1, ctx->cfg.frame.rebate_width, right_box);
    ctx->setting_spins.frame_rebate_depth = add_setting_row("D - Frame Rebate Depth:", 5, 30, 1, ctx->cfg.frame.rebate_depth, right_box);
    ctx->setting_spins.frame_sill_rebate  = add_setting_row("E - Sill Rebate:", 0, 50, 1, ctx->cfg.frame.sill_rebate, right_box);

    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(right_box), sep1);

    // SASH SECTION
    GtkWidget *lbl_sash = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_sash), "<b>Sash Specifications (mm)</b>");
    gtk_widget_set_halign(lbl_sash, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right_box), lbl_sash);

    ctx->setting_spins.sash_width          = add_setting_row("F - Sash Face Width:", 40, 150, 1, ctx->cfg.sash.width, right_box);
    ctx->setting_spins.sash_depth          = add_setting_row("G - Sash Depth:", 40, 150, 1, ctx->cfg.sash.depth, right_box);
    ctx->setting_spins.sash_rebate_width   = add_setting_row("H - Sash Rebate Width:", 5, 40, 1, ctx->cfg.sash.rebate_width, right_box);
    ctx->setting_spins.sash_rebate_depth   = add_setting_row("I - Sash Rebate Depth:", 5, 30, 1, ctx->cfg.sash.rebate_depth, right_box);
    ctx->setting_spins.sash_overlap        = add_setting_row("J - Sash Overlap:", 0, 30, 1, ctx->cfg.sash.overlap, right_box);
    ctx->setting_spins.sash_glass_rebate_w = add_setting_row("K - Glass Rebate Width:", 5, 40, 1, ctx->cfg.sash.glass_rebate_w, right_box);
    ctx->setting_spins.sash_bead_width     = add_setting_row("L - Glazing Bead Width:", 5, 30, 1, ctx->cfg.sash.bead_width, right_box);

    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(right_box), sep2);

    // GAPS SECTION
    GtkWidget *lbl_gaps = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_gaps), "<b>Gaps and Clearances (mm)</b>");
    gtk_widget_set_halign(lbl_gaps, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(right_box), lbl_gaps);

   	ctx->setting_spins.gaps_air_gap        = add_setting_row("M - Air Clearance Gap:", 1, 20, 1, ctx->cfg.gaps.air_gap, right_box);
    ctx->setting_spins.gaps_glass_clearance = add_setting_row("N - Glass Clearance Gap:", 1, 15, 1, ctx->cfg.gaps.glass_clearance, right_box);

    // SAVE BUTTON
    GtkWidget *btn_save = gtk_button_new_with_label("Save System Defaults");
    gtk_widget_set_margin_top(btn_save, 14);
    gtk_box_append(GTK_BOX(right_box), btn_save);

    g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_settings_clicked), ctx);

    gtk_box_append(GTK_BOX(main_hbox), scroll);

    // Set layout as window child
    gtk_window_set_child(GTK_WINDOW(dialog), main_hbox);
    gtk_window_present(GTK_WINDOW(dialog));
}