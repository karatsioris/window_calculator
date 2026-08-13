#include "gui.h"

void activate(GtkApplication *app, gpointer user_data)
{
    t_app_context *ctx = (t_app_context *)user_data;
    
    ctx->main_window = gtk_application_window_new(app);
    ctx->area = gtk_drawing_area_new();
    
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(ctx->main_window), header_bar);

    // Import Button
    GtkWidget *import_btn = gtk_button_new();
    GtkWidget *import_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(import_box), gtk_image_new_from_icon_name("document-open-symbolic"));
	gtk_box_append(GTK_BOX(import_box), gtk_label_new("Import"));
    gtk_button_set_child(GTK_BUTTON(import_btn), import_box);
    gtk_widget_set_margin_start(import_btn, 40);
    g_signal_connect(import_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), import_btn);

    // Save Button
    GtkWidget *save_btn = gtk_button_new();
    GtkWidget *save_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(save_box), gtk_image_new_from_icon_name("document-save-symbolic"));
	gtk_box_append(GTK_BOX(save_box), gtk_label_new("Save"));
    gtk_button_set_child(GTK_BUTTON(save_btn), save_box);
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);
    gtk_widget_add_css_class(save_btn, "suggested-action");
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), save_btn);

	// Setting Button
	GtkWidget *settings_btn = gtk_button_new();
	GtkWidget *settings_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
	gtk_box_append(GTK_BOX(settings_box), gtk_image_new_from_file("settings.svg"));
	gtk_box_append(GTK_BOX(settings_box), gtk_label_new("Settings"));
	gtk_button_set_child(GTK_BUTTON(settings_btn), settings_box);
	g_signal_connect(settings_btn, "clicked", G_CALLBACK(open_profile_settings_dialog), ctx);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), settings_btn);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *left_panel = build_sidebar(ctx);
    GtkWidget *right_panel = build_cut_list_panel(ctx);

    gtk_widget_set_size_request(left_panel, 250, -1);
    set_margin(left_panel, 40);
    
    gtk_widget_set_hexpand(ctx->area, TRUE);
    gtk_widget_set_vexpand(ctx->area, TRUE);
    
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ctx->area), draw_function, ctx, NULL);
    
    gtk_box_append(GTK_BOX(box), left_panel);
    gtk_box_append(GTK_BOX(box), ctx->area);
    gtk_box_append(GTK_BOX(box), right_panel);
    
    gtk_window_set_title(GTK_WINDOW(ctx->main_window), "Window calculator");
    gtk_window_set_child(GTK_WINDOW(ctx->main_window), box);

    gtk_window_maximize(GTK_WINDOW(ctx->main_window));
    gtk_window_present(GTK_WINDOW(ctx->main_window));
}