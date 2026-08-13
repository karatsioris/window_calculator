#include "stdio.h"
#include "stdlib.h"
#include "gtk/gtk.h"

#include "calculator.h"
#include "gui.h"
#include "config.h"


int main(int argc, char **argv)
{
    GtkApplication		*app;
	t_app_context		*ctx;
    int					status;
	const char			*filename = "config.ini";

    ctx = g_new0(t_app_context, 1);
  
	if (!ctx)
	{
        g_printerr("Error: Memory allocation failed!\n");
        return 1;
    }
 
	if (!load_config_from_ini(filename, &ctx->cfg))
    {
        load_default_config(&ctx->cfg);
        save_config_to_ini(filename, &ctx->cfg);
    }
  
	init_app_context(ctx);

    app = gtk_application_new("com.konstantinos.windowcalc", G_APPLICATION_DEFAULT_FLAGS);
	

    g_signal_connect(app, "activate", G_CALLBACK(activate), ctx);

	g_signal_connect(app, "shutdown", G_CALLBACK(shutdown), ctx);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    

    g_object_unref(app);
	g_free(ctx);
    return status;

}