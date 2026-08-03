#include "types.h"

#define RAW_FRAME_WIDTH_MM  80.0f
#define RAW_SASH_WIDTH_MM   80.0f
#define LENGTH_ALLOWANCE_MM 50.0f

static const char *window_type_to_str(t_window_type t)
{
    switch (t)
    {
        case WINDOW_SINGLE_SASH: return "Single Sash";
        case WINDOW_DOUBLE_SASH: return "Double Sash";
        case WINDOW_FIXED:       return "Fixed";
        default:                 return "Unknown";
    }
}

static const char *opening_dir_to_str(t_opening_dir d)
{
    switch (d)
    {
        case OPEN_LEFT:  return "Left";
        case OPEN_RIGHT: return "Right";
        default:         return "N/A";
    }
}

static const char *mechanism_to_str(t_mechanism_type m)
{
    switch (m)
    {
        case MECH_TURN_ONLY:     return "Turn Only";
        case MECH_TILT_AND_TURN: return "Tilt and Turn";
        case MECH_FIXED:         return "Fixed";
        default:                 return "Unknown";
    }
}

void on_export_csv_clicked(GtkButton *btn, t_app_context *ctx)
{
    (void)btn;
    FILE *f = fopen("cutting_list.csv", "w");
    if (!f)
        return;

    // --- Project metadata header ---
    fprintf(f, "Window Calculator - Cutting List Export\n\n");
    fprintf(f, "Window Type,%s\n", window_type_to_str(ctx->win.type));
    fprintf(f, "Opening Direction,%s\n", opening_dir_to_str(ctx->win.opening_dir));
    fprintf(f, "Mechanism,%s\n", mechanism_to_str(ctx->win.mechanism));
    fprintf(f, "Requested Width (mm),%.0f\n", ctx->win.width);
    fprintf(f, "Requested Height (mm),%.0f\n", ctx->win.height);
    fprintf(f, "Frame Overall Width (mm),%.0f\n", ctx->res.frame_width);
    fprintf(f, "Frame Overall Height (mm),%.0f\n", ctx->res.frame_height);
    if (ctx->win.mechanism != MECH_FIXED)
    {
        fprintf(f, "Sash Overall Width (mm),%.0f\n", ctx->res.sash_width);
        fprintf(f, "Sash Overall Height (mm),%.0f\n", ctx->res.sash_height);
    }
    fprintf(f, "\n"); // blank row separates metadata from table

    // --- Cutting list table ---
    fprintf(f, "Item,Material,Qty,Over Thickness,Over Width,Over Length,"
               "Finish Thickness,Finish Width,Finish Length\n");

    fprintf(f, "Frame Head,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
        ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM,
        ctx->res.frame_horizontal_cut + LENGTH_ALLOWANCE_MM,
        ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_horizontal_cut);

    fprintf(f, "Frame Sill,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
        ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM,
        ctx->res.frame_horizontal_cut + LENGTH_ALLOWANCE_MM,
        ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_horizontal_cut);

    fprintf(f, "Frame Left Jamb,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
        ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM,
        ctx->res.frame_vertical_cut + LENGTH_ALLOWANCE_MM,
        ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_vertical_cut);

    fprintf(f, "Frame Right Jamb,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
        ctx->cfg.frame.depth, RAW_FRAME_WIDTH_MM,
        ctx->res.frame_vertical_cut + LENGTH_ALLOWANCE_MM,
        ctx->cfg.frame.depth, ctx->cfg.frame.width, ctx->res.frame_vertical_cut);

    if (ctx->win.mechanism != MECH_FIXED)
    {
        fprintf(f, "Sash Top Rail,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
            ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM,
            ctx->res.sash_horizontal_cut + LENGTH_ALLOWANCE_MM,
            ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_horizontal_cut);

        fprintf(f, "Sash Bottom Rail,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
            ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM,
            ctx->res.sash_horizontal_cut + LENGTH_ALLOWANCE_MM,
            ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_horizontal_cut);

        fprintf(f, "Sash Left Stile,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
            ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM,
            ctx->res.sash_height + LENGTH_ALLOWANCE_MM,
            ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_height);

        fprintf(f, "Sash Right Stile,Oak,1,%.0f,%.0f,%.0f,%.0f,%.0f,%.0f\n",
            ctx->cfg.sash.depth, RAW_SASH_WIDTH_MM,
            ctx->res.sash_height + LENGTH_ALLOWANCE_MM,
            ctx->cfg.sash.depth, ctx->cfg.sash.width, ctx->res.sash_height);
    }

    fprintf(f, "Glass Pane,Glass,1,-,-,-,-,%.0f,%.0f\n",
        ctx->res.glass_width, ctx->res.glass_height);

    fclose(f);
}