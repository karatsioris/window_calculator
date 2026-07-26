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

	// cfg->opening_dir = OPEN_LEFT;
    // cfg->mechanism = MECH_TURN_ONLY;
}

void init_window(t_window *win,const t_profile_config *cfg)
{
	if (!win || !cfg)
        return;

	win->type = WINDOW_SINGLE_SASH;
    win->width = 500.0f;
    win->height = 500.0f;
    
	win->frame = cfg->frame;
	win->sash = cfg->sash;
	win->gaps = cfg->gaps;

	// win->opening_dir = cfg->opening_dir;
    // win->mechanism   = cfg->mechanism;
}

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

	// if (g_key_file_has_key(keyfile, "Hardware", "opening_dir", NULL))
    // 	cfg->opening_dir = (t_opening_dir)g_key_file_get_integer(keyfile, "Hardware", "opening_dir", NULL);
	// else
    // 	cfg->opening_dir = OPEN_LEFT; // Default

	// if (g_key_file_has_key(keyfile, "Hardware", "mechanism", NULL))
	// 	cfg->mechanism = (t_mechanism_type)g_key_file_get_integer(keyfile, "Hardware", "mechanism", NULL);
	// else
	// 	cfg->mechanism = MECH_TURN_ONLY; // Default

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

	// g_key_file_set_integer(keyfile, "Hardware", "opening_dir", (int)cfg->opening_dir);
	// g_key_file_set_integer(keyfile, "Hardware", "mechanism", (int)cfg->mechanism);

    g_key_file_save_to_file(keyfile, filename, NULL);
    g_key_file_free(keyfile);
}
