#ifndef CONFIG_H
# define CONFIG_H

# include <gtk/gtk.h>
# include "types.h"


void        load_default_config(t_profile_config *cfg);
void        init_window(t_window *win, const t_profile_config *cfg);

gboolean    load_config_from_ini(const char *filename, t_profile_config *cfg);
void        save_config_to_ini(const char *filename, const t_profile_config *cfg);


gboolean 	save_window_to_win(const char *filename, const t_app_context *ctx);
int			load_project_from_win(const char *filepath, t_app_context *ctx);

void 		init_app_context(t_app_context *ctx);

void		shutdown(GtkApplication *app, gpointer user_data);

#endif