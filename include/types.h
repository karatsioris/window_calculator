#ifndef TYPES_H
#define TYPES_H


#include <gtk/gtk.h>	

typedef enum e_window_type
{
    WINDOW_SINGLE_SASH,
    WINDOW_DOUBLE_SASH,
    WINDOW_FIXED
} t_window_type;

typedef enum e_opening_dir
{
	OPEN_LEFT,
	OPEN_RIGHT
} t_opening_dir;

typedef	enum e_mechanism_type
{
	MECH_TURN_ONLY,
	MECH_TILT_AND_TURN,
	MECH_FIXED
} t_mechanism_type;

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

    float	glass_rebate_w; // 4mm
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
	t_opening_dir		opening_dir;
	t_mechanism_type	mechanism;
    float				width;
    float				height;
} t_window;

typedef struct s_profile_config
{
    t_frame_profile		frame;
    t_sash_profile 		sash;
    t_hardware_gaps		gaps;
} t_profile_config;

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

typedef struct s_cut_list_widgets
{
    GtkWidget *lbl_frame_v_raw;
    GtkWidget *lbl_frame_v_net;
    GtkWidget *lbl_frame_h_raw;
    GtkWidget *lbl_frame_h_net;
    GtkWidget *lbl_sash_v_raw;
    GtkWidget *lbl_sash_v_net;
    GtkWidget *lbl_sash_h_raw;
    GtkWidget *lbl_sash_h_net;
    GtkWidget *lbl_glass_net;
} t_cut_list_widgets;

typedef struct s_app_context
{
    t_window         win;
    t_profile_config cfg;
    t_result         res;
	t_cut_list_widgets cut_list_ui;

    GtkWidget       *area;
    GtkWidget       *main_window;
	GtkWidget       *box;

    GtkWidget       *right_panel;
    
    GtkSpinButton   *spin_width;
    GtkSpinButton   *spin_height;
    GtkDropDown     *drop_direction;
    GtkDropDown     *drop_mechanism;
} t_app_context;

#endif