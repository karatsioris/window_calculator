#include "calculator.h"

void calculate_layout_offsets(const t_profile_config *cfg, t_layout_offsets *out)
{
    out->offset_top          = cfg->frame.width - cfg->frame.rebate_width - cfg->sash.overlap; // 41mm
    out->inner_frame_offset  = out->offset_top + cfg->sash.overlap; // 47mm
    out->inner_sill_offset   = cfg->frame.width - cfg->frame.sill_rebate - cfg->sash.overlap;
}

void calculate_dimensions(const t_window *win, t_result *res, const t_profile_config *cfg, t_layout_offsets *offsets)
{
	if (!win || !res || !cfg)
        return;

	res->sash_horizontal_cut = (res->sash_width - ((2 * cfg->sash.width) - (2 * cfg->sash.bead_width))) + 8;
    res->sash_vertical_cut = res->sash_height;

	res->sash_width = win->width - 2 * cfg->frame.width  + (2 * cfg->frame.rebate_width) + (2 * cfg->sash.overlap);
	res->sash_height = win->height - 2 * cfg->frame.width  + (cfg->frame.rebate_width + cfg->frame.sill_rebate) + (2 * cfg->sash.overlap);

	res->frame_vertical_cut = win->height;
	res->frame_horizontal_cut = win->width - (2 * cfg->frame.width) + ( 2 * cfg->frame.rebate_width);

	res->frame_width = win->width;
	res->frame_height = win->height;

	res->glass_width = res->sash_width - 2 * (cfg->sash.width - cfg->sash.glass_rebate_w) + 2 * cfg->gaps.glass_clearance;
	res->glass_height = res->sash_height - 2 * (cfg->sash.width - cfg->sash.glass_rebate_w) + 2 * cfg->gaps.glass_clearance;
	
	calculate_layout_offsets(cfg, offsets);
}