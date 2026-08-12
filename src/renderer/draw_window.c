#include "renderer.h"

static float compute_fit_scale(int canvas_width, int canvas_height, float object_w_mm, float object_h_mm, float padding_px)
{
    float avail_w = (float)canvas_width  - (2.0f * padding_px);
    float avail_h = (float)canvas_height - (2.0f * padding_px);

    if (object_w_mm <= 0.0f || object_h_mm <= 0.0f || avail_w <= 0.0f || avail_h <= 0.0f)
        return 0.1f;

    float scale_x = avail_w / object_w_mm;
    float scale_y = avail_h / object_h_mm;
    
	float fit_scale = (scale_x < scale_y) ? scale_x : scale_y;

    return (fit_scale < MAX_SCALE_PX_PER_MM) ? fit_scale : MAX_SCALE_PX_PER_MM;
}


static void		calculated_scaled_vars(int width,int height, t_app_context *ctx, t_scaled_vars *scaled_var, t_layout_offsets *out)
{
    if(!ctx || !scaled_var)
    {
        return;
    }

	float scale = compute_fit_scale(width, height, ctx->res.frame_width, ctx->res.frame_height, 200.0f);
	
	scaled_var->frame_w				= ctx->res.frame_width * scale;
    scaled_var->frame_h				= ctx->res.frame_height * scale;
    scaled_var->sash_w				= ctx->res.sash_width * scale;
    scaled_var->sash_h				= ctx->res.sash_height * scale;
	scaled_var->offset_top			= out->offset_top * scale;
	scaled_var->inner_frame_offset	= out->inner_frame_offset * scale; // 47mm
	scaled_var->inner_sill_offset	= out->inner_sill_offset * scale;
	scaled_var->sash_visible_w		= (ctx->cfg.sash.width - ctx->cfg.sash.bead_width) * scale;
	scaled_var->sash_w_profile		= ctx->cfg.sash.width * scale;
	
}


static void draw_frame_lines(cairo_t *cr, float frame_x, float frame_y, const t_scaled_vars *sv)
{
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_rectangle(cr, frame_x, frame_y, sv->frame_w, sv->frame_h);
    cairo_stroke(cr);

    cairo_set_line_width(cr, 0.5);
    // Πάνω Γωνίες
    cairo_move_to(cr, frame_x + sv->inner_frame_offset, frame_y + sv->offset_top);
    cairo_line_to(cr, frame_x + sv->inner_frame_offset, frame_y);

    cairo_move_to(cr, frame_x + sv->frame_w - sv->inner_frame_offset, frame_y + sv->offset_top);
    cairo_line_to(cr, frame_x + sv->frame_w - sv->inner_frame_offset, frame_y);
    
    // Κάτω Γωνίες (Sill)
    cairo_move_to(cr, frame_x + sv->inner_frame_offset, frame_y + sv->frame_h - sv->inner_sill_offset);
    cairo_line_to(cr, frame_x + sv->inner_frame_offset, frame_y + sv->frame_h);

    cairo_move_to(cr, frame_x + sv->frame_w - sv->inner_frame_offset, frame_y + sv->frame_h - sv->inner_sill_offset);
    cairo_line_to(cr, frame_x + sv->frame_w - sv->inner_frame_offset, frame_y + sv->frame_h);
    cairo_stroke(cr);
}

static void draw_fix_lines(cairo_t *cr, float frame_x, float frame_y, const t_scaled_vars *sv)
{
    cairo_set_line_width(cr, 1.0);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_rectangle(cr, frame_x, frame_y, sv->frame_w, sv->frame_h);
    cairo_stroke(cr);

	cairo_set_line_width(cr, 0.5);
	 // Inner Sash / Glass Opening
    cairo_rectangle(cr, frame_x + sv->sash_visible_w,
                        frame_y + sv->sash_visible_w,
                        sv->frame_w - (2.0f * sv->sash_visible_w),
                        sv->frame_h - (2.0f * sv->sash_visible_w));

	cairo_rectangle(cr, frame_x + sv->sash_w_profile,
                        frame_y + sv->sash_w_profile,
                        sv->frame_w - (2.0f * sv->sash_w_profile),
                        sv->frame_h - (2.0f * sv->sash_w_profile));
    cairo_stroke(cr);

	 // Φάλτσα Πηχακίων (4 Corner Lines)
    cairo_move_to(cr, frame_x + sv->sash_visible_w, frame_y + sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->sash_w_profile, frame_y + sv->sash_w_profile);

    cairo_move_to(cr, frame_x + sv->frame_w - sv->sash_visible_w, frame_y + sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->frame_w - sv->sash_w_profile, frame_y + sv->sash_w_profile);

    cairo_move_to(cr, frame_x + sv->sash_visible_w, frame_y + sv->frame_h - sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->sash_w_profile, frame_y + sv->frame_h - sv->sash_w_profile);

    cairo_move_to(cr, frame_x + sv->frame_w - sv->sash_visible_w, frame_y + sv->frame_h - sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->frame_w - sv->sash_w_profile, frame_y + sv->frame_h - sv->sash_w_profile);

    // Ένωση Τεμαχίων Φύλλου (Stiles/Rails)
    cairo_move_to(cr, frame_x + sv->sash_visible_w, frame_y + sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->sash_visible_w, frame_y);

    cairo_move_to(cr, frame_x + sv->sash_visible_w, frame_y + sv->frame_h - sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->sash_visible_w, frame_y + sv->frame_h);

    cairo_move_to(cr, frame_x + sv->frame_w - sv->sash_visible_w, frame_y + sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->frame_w - sv->sash_visible_w, frame_y);

    cairo_move_to(cr, frame_x + sv->frame_w - sv->sash_visible_w, frame_y + sv->frame_h - sv->sash_visible_w);
    cairo_line_to(cr, frame_x + sv->frame_w - sv->sash_visible_w, frame_y + sv->frame_h);

    cairo_stroke(cr);


}

static void draw_sash_lines(cairo_t *cr, float sash_x, float sash_y, const t_scaled_vars *sv)
{
    cairo_set_line_width(cr, 0.5);
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);

    // Outer Sash Box
    cairo_rectangle(cr, sash_x, sash_y, sv->sash_w, sv->sash_h);

    // Inner Sash / Glass Opening
    cairo_rectangle(cr, sash_x + sv->sash_visible_w,
                        sash_y + sv->sash_visible_w,
                        sv->sash_w - (2.0f * sv->sash_visible_w),
                        sv->sash_h - (2.0f * sv->sash_visible_w));

    // Glazing Bead Inner Box
    cairo_rectangle(cr, sash_x + sv->sash_w_profile,
                        sash_y + sv->sash_w_profile,
                        sv->sash_w - (2.0f * sv->sash_w_profile),
                        sv->sash_h - (2.0f * sv->sash_w_profile));
    cairo_stroke(cr);

    // Φάλτσα Πηχακίων (4 Corner Lines)
    cairo_move_to(cr, sash_x + sv->sash_visible_w, sash_y + sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_w_profile, sash_y + sv->sash_w_profile);

    cairo_move_to(cr, sash_x + sv->sash_w - sv->sash_visible_w, sash_y + sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_w - sv->sash_w_profile, sash_y + sv->sash_w_profile);

    cairo_move_to(cr, sash_x + sv->sash_visible_w, sash_y + sv->sash_h - sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_w_profile, sash_y + sv->sash_h - sv->sash_w_profile);

    cairo_move_to(cr, sash_x + sv->sash_w - sv->sash_visible_w, sash_y + sv->sash_h - sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_w - sv->sash_w_profile, sash_y + sv->sash_h - sv->sash_w_profile);

    // Ένωση Τεμαχίων Φύλλου (Stiles/Rails)
    cairo_move_to(cr, sash_x + sv->sash_visible_w, sash_y + sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_visible_w, sash_y);

    cairo_move_to(cr, sash_x + sv->sash_visible_w, sash_y + sv->sash_h - sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_visible_w, sash_y + sv->sash_h);

    cairo_move_to(cr, sash_x + sv->sash_w - sv->sash_visible_w, sash_y + sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_w - sv->sash_visible_w, sash_y);

    cairo_move_to(cr, sash_x + sv->sash_w - sv->sash_visible_w, sash_y + sv->sash_h - sv->sash_visible_w);
    cairo_line_to(cr, sash_x + sv->sash_w - sv->sash_visible_w, sash_y + sv->sash_h);

    cairo_stroke(cr);
}


void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data)
{
    (void)area;
    t_app_context *ctx = (t_app_context *) data;
    if (!ctx)
        return;
    t_scaled_vars  sv;

    calculated_scaled_vars(width, height, ctx, &sv, &ctx->offsets);

    float frame_x = (width  / 2.0f) - (sv.frame_w / 2.0f);
    float frame_y = (height / 2.0f) - (sv.frame_h / 2.0f);
    float sash_x = frame_x + (sv.frame_w - sv.sash_w) / 2.0f;;
    float sash_y = frame_y + sv.offset_top;;
	
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);

	if(ctx->win.mechanism == MECH_FIXED)
	{
		draw_fix_lines(cr, frame_x, frame_y, &sv);
		draw_mechanism(cr, frame_x, frame_y, sv.frame_w, sv.frame_h, ctx->win.opening_dir, ctx->win.mechanism);
		draw_fixed_dimensions(cr, frame_x, frame_y, &sv, &ctx->res);
	}
	else
	{
		draw_frame_lines(cr, frame_x, frame_y, &sv);
		draw_sash_lines(cr, sash_x, sash_y, &sv);
		draw_mechanism(cr, sash_x, sash_y, sv.sash_w, sv.sash_h, ctx->win.opening_dir, ctx->win.mechanism);
		draw_all_dimensions(cr, frame_x, frame_y, sash_x, sash_y, &sv, &ctx->res);
	}
}