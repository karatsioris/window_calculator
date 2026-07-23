#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "types.h"

typedef struct s_spin_data
{
    float		*field;
    t_window	*win;
    t_result	*res;
    GtkWidget	*area;
} t_spin_data;


void activate(GtkApplication *app, gpointer data);

GtkWidget *build_sidebar(t_window *win, t_result *res, GtkWidget *area);

#endif