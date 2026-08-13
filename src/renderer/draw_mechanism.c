#include "renderer.h"

void draw_mechanism(cairo_t *cr, double x, double y, double w, double h,
								t_opening_dir dir, t_mechanism_type mech)
{
	
	cairo_save(cr);
	double	dashes[] = {6.0, 4,0};
	cairo_set_dash(cr, dashes, 2, 0);
	cairo_set_line_width(cr, 0.5);
	cairo_set_source_rgb(cr, 0.4, 0.4, 0.4);
	
	if(mech == MECH_FIXED)
	{
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x + w, y + h);
		cairo_move_to(cr, x, y + h);
    	cairo_line_to(cr, x + w, y);
	}
	else if(dir == OPEN_LEFT)
	{
		cairo_move_to(cr, x, y);
		cairo_line_to(cr, x + w, y + h / 2.0);
		cairo_line_to(cr, x, y + h);
	}
	else
	{
		cairo_move_to(cr, x + w, y);
		cairo_line_to(cr, x , y + h / 2.0);
		cairo_line_to(cr, x + w, y + h);
	}
	if (mech == MECH_TILT_AND_TURN)
	{
        cairo_move_to(cr, x, y + h);
        cairo_line_to(cr, x + w / 2.0, y);
        cairo_line_to(cr, x + w, y + h);
    }


    cairo_stroke(cr);
    cairo_restore(cr);
}