#include "renderer.h"

/**
 * Draws a horizontal dimension line
 * 
 * @param cr        Cairo context
 * @param x1        Start X coordinate (in pixels)
 * @param x2        End X coordinate (in pixels)
 * @param line_y    The Y level of the horizontal dimension line (in pixels)
 * @param value_mm  The dimension value in mm (to be displayed)
 * @param ext_start The Y level where the vertical extension lines start
 * @param ext_end   The Y level where the vertical extension lines end
 */

void draw_horizontal_dimension(cairo_t *cr, double x1, double x2, double line_y,
								float value_mm, double ext_start, double ext_end)
{
    char label[32];
    snprintf(label, sizeof(label), "%.0f mm", value_mm);

    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 0.3);

    // (Extension Lines)
    cairo_move_to(cr, x1, ext_start);
    cairo_line_to(cr, x1, ext_end);
    
    cairo_move_to(cr, x2, ext_start);
    cairo_line_to(cr, x2, ext_end);

    // Dimension line
    cairo_move_to(cr, x1, line_y);
    cairo_line_to(cr, x2, line_y);
    
    cairo_stroke(cr);

    draw_dimension_text(cr, x1 + ((x2 - x1) / 2.0), line_y - 12.0, label, 0.0);

    cairo_restore(cr);
}

void draw_vertical_dimension(cairo_t *cr, double y1, double y2, double line_x, 
                             float value_mm, double ext_start, double ext_end)
{
    char label[32];
    snprintf(label, sizeof(label), "%.0f mm", value_mm);

    cairo_save(cr);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_set_line_width(cr, 0.3);

    // (Extension Lines)
    cairo_move_to(cr, ext_start, y1);
    cairo_line_to(cr, ext_end, y1);
    
    cairo_move_to(cr, ext_start, y2);
    cairo_line_to(cr, ext_end, y2);

    // Dimention line
    cairo_move_to(cr, line_x, y1);
    cairo_line_to(cr, line_x, y2);
    
    cairo_stroke(cr);

    draw_dimension_text(cr, line_x + 12.0, y1 + ((y2 - y1) / 2.0), label, -G_PI / 2.0);

    cairo_restore(cr);
}

void draw_dimension_text(cairo_t *cr, double x, double y, const char *text, double angle_rad)
{
    cairo_text_extents_t extents;

    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12.0);
    cairo_text_extents(cr, text, &extents);

    cairo_save(cr);
    
    // Move origin to the target center point
    cairo_translate(cr, x, y);
    
    // Rotate text if needed (e.g., for vertical dimensions)
    if (angle_rad != 0.0)
	{
        cairo_rotate(cr, angle_rad);
    }

    // Center the text bounding box on (0, 0)
    double text_x = -(extents.width / 2.0 + extents.x_bearing);
    double text_y = -(extents.height / 2.0 + extents.y_bearing);

    cairo_move_to(cr, text_x, text_y);
    cairo_show_text(cr, text);

    cairo_restore(cr);
}

void draw_all_dimensions(cairo_t *cr, float frame_x, float frame_y, float sash_x, float sash_y, 
                                const t_scaled_vars *sv, const t_result *res)
{
    double frame_dim_y = frame_y - DIM_OFFSET_OUTER;
    double sash_dim_y  = frame_y - DIM_OFFSET_INNER;
    double frame_dim_x = frame_x + sv->frame_w + DIM_OFFSET_OUTER;
    double sash_dim_x  = frame_x + sv->frame_w + DIM_OFFSET_INNER;

    // Top Dimensions (Widths)
    draw_horizontal_dimension(cr, frame_x, frame_x + sv->frame_w,frame_dim_y,
								res->frame_width,
								frame_dim_y - DIM_EXT_OVERHANG,
								frame_y - DIM_EXT_GAP);

    draw_horizontal_dimension(cr, sash_x, sash_x + sv->sash_w, sash_dim_y,
								res->sash_width,
								sash_dim_y - DIM_EXT_OVERHANG,
								frame_y - DIM_EXT_GAP);

    // Right Dimensions (Heights)
    draw_vertical_dimension(cr, frame_y, frame_y + sv->frame_h, frame_dim_x,
							res->frame_height,
							frame_x + sv->frame_w + DIM_EXT_GAP,
							frame_dim_x + DIM_EXT_OVERHANG);

    draw_vertical_dimension(cr, sash_y, sash_y + sv->sash_h, sash_dim_x,
							res->sash_height,
							frame_x + sv->frame_w + DIM_EXT_GAP,
							sash_dim_x + DIM_EXT_OVERHANG);
}

void draw_fixed_dimensions(cairo_t *cr, float frame_x, float frame_y, 
                                const t_scaled_vars *sv, const t_result *res)
{
    double frame_dim_y = frame_y - DIM_OFFSET_INNER;
    double frame_dim_x = frame_x + sv->frame_w + DIM_OFFSET_INNER;

    draw_horizontal_dimension(cr, frame_x, frame_x + sv->frame_w,frame_dim_y,
								res->frame_width,
								frame_dim_y - DIM_EXT_OVERHANG,
								frame_y - DIM_EXT_GAP);

    draw_vertical_dimension(cr, frame_y, frame_y + sv->frame_h, frame_dim_x,
							res->frame_height,
							frame_x + sv->frame_w + DIM_EXT_GAP,
							frame_dim_x + DIM_EXT_OVERHANG);
}
