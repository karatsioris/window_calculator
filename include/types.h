#ifndef TYPES_H
#define TYPES_H

typedef enum e_window_type
{
    WINDOW_SINGLE_SASH,
    WINDOW_DOUBLE_SASH,
    WINDOW_FIXED
} t_window_type;

typedef struct s_frame_profile
{
	float	depth;			// 68mm, 78mm, 92mm
	float	width;			// 79mm
	float	rebate_width;	// 32mm
	float	rebate_depth;	// 12mm
	float	sill_rebate;	// 20mm
} t_frame_profile;

typedef struct s_sash_profile
{
    float	depth;			// 68mm
    float	width;			// 75mm
	float	rebate_width;	// 18mm
	float	rebate_depth;	// 12mm
    float	overlap;		// 6mm

    float	glass_rebate_w;
    float	bead_width;		// 15mm
} t_sash_profile;

typedef struct s_hardware_gaps
{
    float	air_gap;		// 4mm
    float	glass_clearance;
} t_hardware_gaps;

typedef struct s_window
{
    t_window_type		type;
    float				width;
    float				height;

    t_frame_profile		frame;
    t_sash_profile		sash;
    t_hardware_gaps		gaps;
} t_window;

typedef struct s_result
{
    float	frame_width;
    float	frame_height;

    float	frame_horizontal_cut;
    float	frame_vertical_cut;

    int		sash_count;
    float	sash_width;
    float	sash_height;
    float	sash_horizontal_cut;
    float	sash_vertical_cut;

    float	glass_width;
    float	glass_height;
} t_result;
#endif