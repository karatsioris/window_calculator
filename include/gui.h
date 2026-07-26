#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "types.h"

typedef struct s_spin_data
{
    float				*field;
    t_window			*win;
	t_profile_config	*cfg;
    t_result			*res;
    GtkWidget			*area;
} t_spin_data;


void	activate(GtkApplication *app, gpointer data);

void	on_app_shutdown(GtkApplication *app, gpointer user_data);

// GtkWidget *build_sidebar(t_window *win, t_result *res,t_profile_config *cfg, GtkWidget *area);
GtkWidget *build_sidebar(t_app_context *ctx);


void	on_save_project_clicked(GtkButton *btn, gpointer user_data);
void	on_import_clicked(GtkButton *btn, gpointer user_data);
#endif