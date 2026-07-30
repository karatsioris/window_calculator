#include "gui.h"

void activate(GtkApplication *app, gpointer user_data)
{
    t_app_context *ctx = (t_app_context *)user_data;
    
    ctx->main_window = gtk_application_window_new(app);
    ctx->area = gtk_drawing_area_new();
    
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

    // --- Create Header Bar ---
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_window_set_titlebar(GTK_WINDOW(ctx->main_window), header_bar);
    
    
    // Create a horizontal box container with 10px spacing between children
    GtkWidget *btn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 30);
	gtk_widget_set_margin_start(btn_box, 40);
	gtk_widget_set_margin_top(btn_box, 10);
	gtk_widget_set_margin_bottom(btn_box, 10);


    // Insert Button
    GtkWidget *insert_btn = gtk_button_new_with_label("Insert");
    g_signal_connect(insert_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);

    // Save Button
    GtkWidget *save_btn = gtk_button_new_with_label("Save");
    g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);

    // Append buttons to the box container
    gtk_box_append(GTK_BOX(btn_box), insert_btn);
    gtk_box_append(GTK_BOX(btn_box), save_btn);

    // Pack the entire button box into the HeaderBar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn_box);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *sidebar = build_sidebar(ctx);
    GtkWidget *right_panel = build_cut_list_panel(ctx);

    gtk_widget_set_size_request(sidebar, 250, -1);
    set_margin(sidebar, 40);
    
    gtk_widget_set_hexpand(ctx->area, TRUE);
    gtk_widget_set_vexpand(ctx->area, TRUE);
    
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ctx->area), draw_function, ctx, NULL);
    
    gtk_box_append(GTK_BOX(box), sidebar);
    gtk_box_append(GTK_BOX(box), ctx->area);
    gtk_box_append(GTK_BOX(box), right_panel);
    
    gtk_window_set_title(GTK_WINDOW(ctx->main_window), "Window calculator");
    gtk_window_set_child(GTK_WINDOW(ctx->main_window), box);

    gtk_window_maximize(GTK_WINDOW(ctx->main_window));
    gtk_window_present(GTK_WINDOW(ctx->main_window));

}