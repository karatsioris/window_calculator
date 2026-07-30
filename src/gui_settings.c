#include "gui.h"

void open_profile_settings_dialog(GtkButton *btn, gpointer user_data)
{
    (void)btn;
    t_app_context *ctx = (t_app_context *)user_data;

    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Profile & Hardware Setup (config.ini)");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(ctx->main_window));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 500);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);
    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);

    /* Frame Section */
    GtkWidget *lbl_frame = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(lbl_frame), "<b>Frame Specifications (mm)</b>");
    gtk_widget_set_halign(lbl_frame, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), lbl_frame);

    GtkWidget *sb_f_width = gtk_spin_button_new_with_range(40, 150, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_f_width), ctx->cfg.frame.width);
    gtk_box_append(GTK_BOX(box), sb_f_width);

    GtkWidget *sb_f_rebate = gtk_spin_button_new_with_range(10, 60, 1);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(sb_f_rebate), ctx->cfg.frame.rebate_width);
    gtk_box_append(GTK_BOX(box), sb_f_rebate);

    /* Save & Close Button */
    GtkWidget *btn_save = gtk_button_new_with_label("Save System Defaults");
    gtk_box_append(GTK_BOX(box), btn_save);

    gtk_window_set_child(GTK_WINDOW(dialog), box);
    gtk_window_present(GTK_WINDOW(dialog));
}