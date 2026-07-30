#ifndef RENDERER_H
#define RENDERER_H

#include "types.h"
#include "gui.h"

#define REFERENCE_MAX_MM 5000.0

// Layout & Spacing Constants for Dimensions (in pixels)
#define DIM_OFFSET_OUTER    55.0f  // Distance of the outer dimension line (Frame) from the window
#define DIM_OFFSET_INNER    25.0f  // Distance of the inner dimension line (Sash) from the window
#define DIM_EXT_OVERHANG    10.0f  // Extension line overhang past the dimension line
#define DIM_EXT_GAP          5.0f  // Gap between the window edge and the start of the extension line

typedef struct s_scaled_vars
{
    float	frame_w;
    float	frame_h;
    float	sash_w;
    float	sash_h;
    float	offset_top;
    float	inner_frame_offset;
    float	inner_sill_offset;
    float	sash_visible_w;
    float	sash_w_profile;
} t_scaled_vars;

// --- render_dimension.c ---
void draw_horizontal_dimension(cairo_t *cr, double x1, double x2, double line_y,float value_mm, double ext_start, double ext_end);
void draw_vertical_dimension(cairo_t *cr, double y1, double y2, double line_x, float value_mm, double ext_start, double ext_end);
void draw_dimension_text(cairo_t *cr, double x, double y, const char *text, double angle_rad);
void draw_all_dimensions(cairo_t *cr, float frame_x, float frame_y, float sash_x, float sash_y, const t_scaled_vars *sv, const t_result *res);
void draw_fixed_dimensions(cairo_t *cr, float frame_x, float frame_y, const t_scaled_vars *sv, const t_result *res);

// --- render_mechanism.c ---
void draw_mechanism(cairo_t *cr, double x, double y, double w, double h, t_opening_dir dir, t_mechanism_type mech);

// --- draw_window.c ---
void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data);
void draw_dimension_text(cairo_t *cr, double x, double y, const char *text, double angle_rad);
void draw_horizontal_dimension(cairo_t *cr, double x1, double x2, double line_y, float value_mm, double ext_start, double ext_end);
void draw_dimension_text(cairo_t *cr, double x, double y, const char *text, double angle_rad);

#endif