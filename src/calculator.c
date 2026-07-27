#include "calculator.h"

void calculate_dimensions( t_window *win, t_result *res, const t_profile_config *cfg)
{
	if (!win || !res || !cfg)
        return;

	if(win->mechanism == MECH_FIXED)
	{
		res->sash_width = win->width;
		res->sash_height = win->height;
	}
	else
	{
		// Μονόφυλλο κούφωμα
		res->sash_width = win->width - 2 * cfg->frame.width  + (2 * cfg->frame.rebate_width) + (2 * cfg->sash.overlap);
		res->sash_height = win->height - 2 * cfg->frame.width  + (cfg->frame.rebate_width + cfg->frame.sill_rebate) + (2 * cfg->sash.overlap);
	
		res->frame_vertical_cut = win->height;
		res->frame_horizontal_cut = win->width - (2 * cfg->frame.width) + ( 2 * cfg->frame.rebate_width);
	
		res->frame_width = win->width;
		res->frame_height = win->height;
	
		res->glass_width = cfg->sash.width - 2 * (cfg->sash.width - cfg->sash.glass_rebate_w) + 2 * cfg->gaps.glass_clearance;
		res->glass_height = cfg->sash.width - 2 * (cfg->sash.width - cfg->sash.glass_rebate_w) + 2 * cfg->gaps.glass_clearance;
	}
}
