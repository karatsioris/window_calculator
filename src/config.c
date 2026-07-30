#include "config.h"

void load_default_config(t_profile_config *cfg)
{
    if (!cfg) return;

    cfg->frame.depth = 68.0f;
    cfg->frame.width = 79.0f;
    cfg->frame.rebate_width = 32.0f;
    cfg->frame.rebate_depth = 12.0f;
    cfg->frame.sill_rebate = 20.0f;

    cfg->sash.depth = 68.0f;
    cfg->sash.width = 75.0f;
    cfg->sash.rebate_width = 18.0f;
    cfg->sash.rebate_depth = 12.0f;
    cfg->sash.overlap = 6.0f;
    cfg->sash.bead_width = 15.0f;
    cfg->sash.glass_rebate_w = 4.0f;

    cfg->gaps.air_gap = 4.0f;
    cfg->gaps.glass_clearance = 2.0f;
}

// because i use the t_app_context i can use this function for init
void init_app_context(t_app_context *ctx)
{
    if (!ctx)
        return;

    /* Initialize Window Instance */
    ctx->win.type = WINDOW_SINGLE_SASH;
    ctx->win.width = 500.0f;
    ctx->win.height = 500.0f;
    ctx->win.opening_dir = OPEN_LEFT;
    ctx->win.mechanism = MECH_TURN_ONLY;
}




// void init_window(t_window *win,const t_profile_config *cfg)
// {
// 	if (!win || !cfg)
//         return;

// 	win->type = WINDOW_SINGLE_SASH;
//     win->width = 500.0f;
//     win->height = 500.0f;

// 	win->opening_dir = OPEN_LEFT;
//     win->mechanism   = MECH_TURN_ONLY;
// }


gboolean load_config_from_ini(const char *filename, t_profile_config *cfg)
{
	GKeyFile	*keyfile = g_key_file_new();
	GError		*error = NULL;

	if (!cfg || !filename)
		return FALSE;
	if(!g_key_file_load_from_file(keyfile, filename, G_KEY_FILE_NONE, &error))
	{
		g_printerr("Config info: %s. Using default profile.\n", error->message);
        g_error_free(error);
        g_key_file_free(keyfile);
        return FALSE;
	}

    cfg->frame.depth        = g_key_file_get_double(keyfile, "Frame", "depth", NULL);
    cfg->frame.width        = g_key_file_get_double(keyfile, "Frame", "width", NULL);
    cfg->frame.rebate_width = g_key_file_get_double(keyfile, "Frame", "rebate_width", NULL);
    cfg->frame.rebate_depth = g_key_file_get_double(keyfile, "Frame", "rebate_depth", NULL);
    cfg->frame.sill_rebate  = g_key_file_get_double(keyfile, "Frame", "sill_rebate", NULL);

    cfg->sash.depth          = g_key_file_get_double(keyfile, "Sash", "depth", NULL);
    cfg->sash.width          = g_key_file_get_double(keyfile, "Sash", "width", NULL);
    cfg->sash.rebate_width   = g_key_file_get_double(keyfile, "Sash", "rebate_width", NULL);
    cfg->sash.rebate_depth   = g_key_file_get_double(keyfile, "Sash", "rebate_depth", NULL);
    cfg->sash.overlap        = g_key_file_get_double(keyfile, "Sash", "overlap", NULL);
    cfg->sash.bead_width     = g_key_file_get_double(keyfile, "Sash", "bead_width", NULL);
    cfg->sash.glass_rebate_w = g_key_file_get_double(keyfile, "Sash", "glass_rebate_w", NULL);

    cfg->gaps.air_gap         = g_key_file_get_double(keyfile, "Gaps", "air_gap", NULL);
    cfg->gaps.glass_clearance = g_key_file_get_double(keyfile, "Gaps", "glass_clearance", NULL);

    g_key_file_free(keyfile);
    return TRUE;
}


void save_config_to_ini(const char *filename, const t_profile_config *cfg)
{
    GKeyFile *keyfile = g_key_file_new();

    g_key_file_set_double(keyfile, "Frame", "depth", cfg->frame.depth);
    g_key_file_set_double(keyfile, "Frame", "width", cfg->frame.width);
    g_key_file_set_double(keyfile, "Frame", "rebate_width", cfg->frame.rebate_width);
    g_key_file_set_double(keyfile, "Frame", "rebate_depth", cfg->frame.rebate_depth);
    g_key_file_set_double(keyfile, "Frame", "sill_rebate", cfg->frame.sill_rebate);

    g_key_file_set_double(keyfile, "Sash", "depth", cfg->sash.depth);
    g_key_file_set_double(keyfile, "Sash", "width", cfg->sash.width);
    g_key_file_set_double(keyfile, "Sash", "rebate_width", cfg->sash.rebate_width);
    g_key_file_set_double(keyfile, "Sash", "rebate_depth", cfg->sash.rebate_depth);
    g_key_file_set_double(keyfile, "Sash", "overlap", cfg->sash.overlap);
    g_key_file_set_double(keyfile, "Sash", "bead_width", cfg->sash.bead_width);
    g_key_file_set_double(keyfile, "Sash", "glass_rebate_w", cfg->sash.glass_rebate_w);

    g_key_file_set_double(keyfile, "Gaps", "air_gap", cfg->gaps.air_gap);
    g_key_file_set_double(keyfile, "Gaps", "glass_clearance", cfg->gaps.glass_clearance);

    g_key_file_save_to_file(keyfile, filename, NULL);
    g_key_file_free(keyfile);
}

int	save_project_to_win(const char *filepath, const t_app_context *ctx)
{
	FILE *f = fopen(filepath, "w");
	if(!f)
		return 0;

	fprintf(f, "[Window]\n");
	fprintf(f,"width=%.1fn\n", ctx->win.width);
	fprintf(f, "height=%.1f\n", ctx->win.height);
    fprintf(f, "mechanism=%d\n", ctx->win.mechanism);
    fprintf(f, "opening_dir=%d\n", ctx->win.opening_dir);
    fprintf(f, "type=%d\n\n", ctx->win.type);

	fprintf(f, "[Frame]\n");
    fprintf(f, "width=%.1f\n", ctx->cfg.frame.width);
    fprintf(f, "rebate_width=%.1f\n\n", ctx->cfg.frame.rebate_width);

	fprintf(f, "[Sash]\n");
    fprintf(f, "width=%.1f\n", ctx->cfg.sash.width);
    fprintf(f, "overlap=%.1f\n", ctx->cfg.sash.overlap);

	fclose(f);
	return 1;
}

int load_project_from_win(const char *filepath, t_app_context *ctx)
{
	FILE *f = fopen(filepath, "r");
	if(!f)
		return 0;

	char line[128];
    char section[32] = "";

    while (fgets(line, sizeof(line), f))
	{
        if (line[0] == '[' && strchr(line, ']'))
		{
            sscanf(line, "[%31[^]]]", section);
            continue;
        }

        float fval;
        int ival;

        if (strcmp(section, "Window") == 0)
		{
            if (sscanf(line, "width=%f", &fval) == 1) ctx->win.width = fval;
            else if (sscanf(line, "height=%f", &fval) == 1) ctx->win.height = fval;
            else if (sscanf(line, "mechanism=%d", &ival) == 1) ctx->win.mechanism = (t_mechanism_type)ival;
            else if (sscanf(line, "opening_dir=%d", &ival) == 1) ctx->win.opening_dir = (t_opening_dir)ival;
            else if (sscanf(line, "type=%d", &ival) == 1) ctx->win.type = (t_window_type)ival;
        }
        else if (strcmp(section, "Frame") == 0)
		{
            if (sscanf(line, "width=%f", &fval) == 1) ctx->cfg.frame.width = fval;
            else if (sscanf(line, "rebate_width=%f", &fval) == 1) ctx->cfg.frame.rebate_width = fval;
        }
        else if (strcmp(section, "Sash") == 0)
		{
            if (sscanf(line, "width=%f", &fval) == 1) ctx->cfg.sash.width = fval;
            else if (sscanf(line, "overlap=%f", &fval) == 1) ctx->cfg.sash.overlap = fval;
        }
    }

    fclose(f);
    return 1;
}

gboolean save_window_to_win(const char *filename, const t_app_context *ctx)
{
    if (!filename || !ctx)
        return FALSE;

    GKeyFile *keyfile = g_key_file_new();

    // 1. [Window] Section (Accessing win via ctx->win)
    g_key_file_set_double(keyfile, "Window", "width", ctx->win.width);
    g_key_file_set_double(keyfile, "Window", "height", ctx->win.height);
    g_key_file_set_integer(keyfile, "Window", "mechanism", (int)ctx->win.mechanism);
    g_key_file_set_integer(keyfile, "Window", "opening_dir", (int)ctx->win.opening_dir);

    // 2. [Frame] Section (Accessing profile config via ctx->cfg)
    g_key_file_set_double(keyfile, "Frame", "width", ctx->cfg.frame.width);
    g_key_file_set_double(keyfile, "Frame", "rebate_width", ctx->cfg.frame.rebate_width);

    // 3. [Sash] Section
    g_key_file_set_double(keyfile, "Sash", "width", ctx->cfg.sash.width);
    g_key_file_set_double(keyfile, "Sash", "overlap", ctx->cfg.sash.overlap);

    GError *error = NULL;
    gboolean success = g_key_file_save_to_file(keyfile, filename, &error);

    if (!success)
    {
        g_printerr("Error saving WIN file: %s\n", error ? error->message : "Unknown");
        if (error)
            g_error_free(error);
    }

    g_key_file_free(keyfile);
    return success;
}

void	on_app_shutdown(GtkApplication *app, gpointer user_data)
{
	(void)app;
	t_app_context *ctx = (t_app_context *)user_data;

	if(ctx)
	{
		save_config_to_ini("config.ini", &ctx->cfg);
        g_print("Config saved successfully on exit.\n");
	}
}