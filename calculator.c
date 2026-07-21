#include "calculator.h"

void calculate_dimensions( t_window *win, t_result *res)
{
    // Μονόφυλλο κούφωμα
    res->sash_width = win->width - 2 * win->frame.width  + (2 * win->frame.rebate) + (2 * win->join.overlap);
    res->sash_height = win->width - 2 * win->frame.width  + (win->frame.rebate + win->frame.sill_rebate) + (2 * win->join.overlap);

    res->frame_vertical_cut = win->height;
    res->frame_horizontal_cut = win->width - (2 * win->frame.width) + ( 2 * win->frame.rebate);
}
