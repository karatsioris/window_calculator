#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "types.h"
#include "renderer.h"
#include "calculator.h"
#include "config.h"
#include "gui_cut_list.h"

typedef struct s_spin_data
{
    float				*field;
    t_window			*win;
	t_profile_config	*cfg;
    t_result			*res;
    GtkWidget			*area;
} t_spin_data;



GtkWidget *create_main_layout(t_app_context *ctx);

// composition_gui.c
void	activate(GtkApplication *app, gpointer data);

// gui_sidebar.c
GtkWidget	*build_sidebar(t_app_context *ctx);
void 		set_margin(GtkWidget *place, int num);
void		on_mechanism_changed(GtkDropDown *dropdown, GParamSpec *pspec, gpointer user_data);


// save_import.c
void	on_save_project_clicked(GtkButton *btn, gpointer user_data);
void	on_import_clicked(GtkButton *btn, gpointer user_data);

// gui_sidebar.c
void	set_margin(GtkWidget *place, int num);
void	update_cut_list_ui(t_app_context *ctx);

// export.c
void on_export_csv_clicked(GtkButton *btn, t_app_context *ctx);

// gui_settings.c
void on_settings_clicked(GtkButton *btn, gpointer user_data);

#endif