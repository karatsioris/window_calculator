#include "stdio.h"
#include "stdlib.h"
#include "gtk/gtk.h"

#include "calculator.h"
#include "gui.h"


void    initialization(t_window *win)
{
    win->width = 500;
    win->height = 500;
    win->sash.width = 75;
    win->frame.width = 79;
    win->frame.rebate_width = 32;
    win->frame.sill_rebate = 20;
    win->sash.overlap = 6;
    win->sash.bead_width = 15;
}


int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;
    t_window    *win = NULL;

    win = malloc(sizeof (t_window));
  
    initialization(win);

    app = gtk_application_new("com.konstantinos.windowcalc", G_APPLICATION_DEFAULT_FLAGS);
    // app = gtk_application_new("com.konstantinos.windowcalc", G_APPLICATION_NON_UNIQUE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), win);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);

	free(win);
    return status;

}