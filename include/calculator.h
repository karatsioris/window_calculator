#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "types.h"


void    window_init_defaults(t_window *win);
void    calculate_dimensions(const t_window *win, t_result *res, const t_profile_config *cfg, t_layout_offsets *offsets);
void    calculate_layout_offsets(const t_profile_config *cfg, t_layout_offsets *out);

#endif