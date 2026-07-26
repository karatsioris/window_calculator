#include "stdio.h"
#include "stdlib.h"
#include "gtk/gtk.h"

#include "calculator.h"
#include "gui.h"
#include"config.h"


int main(int argc, char **argv)
{
    GtkApplication		*app;
    t_window			*win = NULL;
	t_profile_config	*cfg = NULL;
    int					status;
	const char			*filename = "config.ini";

    win = malloc(sizeof (t_window));
	cfg = malloc(sizeof (t_profile_config));
  
	if (!win || !cfg)
	{
        g_printerr("Error: Memory allocation failed!\n");
        return 1;
    }
 
	if (!load_config_from_ini(filename, cfg))
    {
        load_default_config(cfg);
        save_config_to_ini(filename, cfg);
    }
  
	init_window(win,cfg);

    app = gtk_application_new("com.konstantinos.windowcalc", G_APPLICATION_DEFAULT_FLAGS);
	
	t_spin_data *app_data = g_new0(t_spin_data, 1);
    app_data->win = win;
    app_data->cfg = cfg;

    g_signal_connect(app, "activate", G_CALLBACK(activate), app_data);

	g_signal_connect(app, "shutdown", G_CALLBACK(on_app_shutdown), app_data);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    
    g_object_unref(app);

	free(win);
	free(cfg);
    return status;

}