#include "calculator.h"

void calculate_dimensions( t_window *win, t_result *res)
{
    // Μονόφυλλο κούφωμα
    res->sash_width = win->width - 2 * win->frame.width  + (2 * win->frame.rebate_width) + (2 * win->sash.overlap);
    res->sash_height = win->height - 2 * win->frame.width  + (win->frame.rebate_width + win->frame.sill_rebate) + (2 * win->sash.overlap);

    res->frame_vertical_cut = win->height;
    res->frame_horizontal_cut = win->width - (2 * win->frame.width) + ( 2 * win->frame.rebate_width);

    res->frame_width = win->width;
    res->frame_height = win->height;

    res->glass_width = win->sash.width - 2 * (win->sash.width - win->sash.glass_rebate_w) + 2 * win->gaps.glass_clearance;
    res->glass_height = win->sash.width - 2 * (win->sash.width - win->sash.glass_rebate_w) + 2 * win->gaps.glass_clearance;

    

}
