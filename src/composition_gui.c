#include "gui.h"

static GtkWidget	*create_and_pack_header_button(char	*label, char *btn_icon, GtkWidget *header_bar)
{
	
	GtkWidget *btn = gtk_button_new();
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_box_append(GTK_BOX(box), gtk_image_new_from_file(btn_icon));
	gtk_box_append(GTK_BOX(box), gtk_label_new(label));
    gtk_button_set_child(GTK_BUTTON(btn), box);
    gtk_widget_set_margin_start(btn, 20);
	gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), btn);
   
	return(btn);
}

void	activate(GtkApplication *app, gpointer user_data)
{
	
    t_app_context	*ctx = (t_app_context *)user_data;
	GtkWidget		*import_btn;
	GtkWidget		*save_btn;
	GtkWidget		*settings_btn;
	GtkWidget		*header_bar;
	GtkWidget		*left_panel;
    GtkWidget		*right_panel;
    
	ctx->box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    ctx->main_window = gtk_application_window_new(app);
    ctx->area = gtk_drawing_area_new();
    
    calculate_dimensions(&ctx->win, &ctx->res, &ctx->cfg, &ctx->offsets);

    header_bar = gtk_header_bar_new();
    left_panel = build_sidebar(ctx);
    right_panel = build_cut_list_panel(ctx);
	import_btn = create_and_pack_header_button("Import", "icons/import.svg", header_bar);
	settings_btn = create_and_pack_header_button("Settings", "icons/settings.svg", header_bar);
    save_btn = create_and_pack_header_button("Save", "icons/save.svg", header_bar);
    
	gtk_window_set_titlebar(GTK_WINDOW(ctx->main_window), header_bar);
	
	g_signal_connect(import_btn, "clicked", G_CALLBACK(on_import_clicked), ctx);
	g_signal_connect(settings_btn, "clicked", G_CALLBACK(on_settings_clicked), ctx);
	g_signal_connect(save_btn, "clicked", G_CALLBACK(on_save_project_clicked), ctx);

    gtk_widget_set_hexpand(ctx->area, TRUE);
    gtk_widget_set_vexpand(ctx->area, TRUE);
    
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(ctx->area), draw_function, ctx, NULL);
    
    gtk_box_append(GTK_BOX(ctx->box), left_panel);
    gtk_box_append(GTK_BOX(ctx->box), ctx->area);
    gtk_box_append(GTK_BOX(ctx->box), right_panel);
    
    gtk_window_set_title(GTK_WINDOW(ctx->main_window), "Window calculator");
    gtk_window_set_child(GTK_WINDOW(ctx->main_window), ctx->box);

    gtk_window_maximize(GTK_WINDOW(ctx->main_window));
    gtk_window_present(GTK_WINDOW(ctx->main_window));
}