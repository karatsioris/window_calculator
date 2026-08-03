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
    ctx->cfg.frame.sill_rebate  = (float)gtk_spin_button_get_value(ctx->setting_spins.frame_sill_rebate);

    // Sash Parameters
    ctx->cfg.sash.depth          = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_depth);
    ctx->cfg.sash.width          = (float)gtk_spin_button_get_value(ctx->setting_spins.sash_width);
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

    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Profile & System Setup");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(ctx->main_window));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 380, 520);

    GtkWidget *scroll = gtk_scrolled_window_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    set_margin(box, 16);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), box);

    /* ---  FRAME SECTION --- */
    GtkWidget *lbl_frame = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_frame), "<b>Frame Specifications (mm)</b>");
    gtk_widget_set_halign(lbl_frame, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), lbl_frame);

    ctx->setting_spins.frame_width = add_setting_row("Frame Face Width:", 40, 150, 1, ctx->cfg.frame.width, box);
    ctx->setting_spins.frame_depth = add_setting_row("Frame Depth:", 40, 150, 1, ctx->cfg.frame.depth, box);
    ctx->setting_spins.frame_rebate_width = add_setting_row("Frame Rebate Width:", 10, 60, 1, ctx->cfg.frame.rebate_width, box);
    ctx->setting_spins.frame_sill_rebate  = add_setting_row("Sill Rebate:", 0, 50, 1, ctx->cfg.frame.sill_rebate, box);

    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(box), sep1);

    /* ---  SASH SECTION --- */
    GtkWidget *lbl_sash = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_sash), "<b>Sash Specifications (mm)</b>");
    gtk_widget_set_halign(lbl_sash, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), lbl_sash);

    ctx->setting_spins.sash_width = add_setting_row("Sash Face Width:", 40, 150, 1, ctx->cfg.sash.width, box);
    ctx->setting_spins.sash_depth = add_setting_row("Sash Depth:", 40, 150, 1, ctx->cfg.sash.depth, box);
    ctx->setting_spins.sash_overlap = add_setting_row("Sash Overlap:", 0, 30, 1, ctx->cfg.sash.overlap, box);
    ctx->setting_spins.sash_glass_rebate_w = add_setting_row("Glass Rebate Width:", 5, 40, 1, ctx->cfg.sash.glass_rebate_w, box);
    ctx->setting_spins.sash_bead_width = add_setting_row("Glazing Bead Width:", 5, 30, 1, ctx->cfg.sash.bead_width, box);

    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(box), sep2);

    /* ---  GAPS SECTION --- */
    GtkWidget *lbl_gaps = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_gaps), "<b>Gaps and Clearances (mm)</b>");
    gtk_widget_set_halign(lbl_gaps, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), lbl_gaps);

    ctx->setting_spins.gaps_air_gap = add_setting_row("Air Clearance Gap:", 1, 20, 1, ctx->cfg.gaps.air_gap, box);
    ctx->setting_spins.gaps_glass_clearance = add_setting_row("Glass Clearance Gap:", 1, 15, 1, ctx->cfg.gaps.glass_clearance, box);

    /* --- SAVE BUTTON --- */
    GtkWidget *btn_save = gtk_button_new_with_label("Save System Defaults");
    gtk_widget_set_margin_top(btn_save, 10);
    gtk_box_append(GTK_BOX(box), btn_save);

    g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_settings_clicked), ctx);

    gtk_window_set_child(GTK_WINDOW(dialog), scroll);
    gtk_window_present(GTK_WINDOW(dialog));
}