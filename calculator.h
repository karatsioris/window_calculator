#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "gtk/gtk.h"

// 1. Προφίλ Φύλλου (Sash Profile)
typedef struct s_sash
{
    float   width;          // Face width (π.χ. 75mm)
    float   depth;          // Profile depth
    // float   visible_width;  // Το ορατό μέρος του προφίλ αν χρειάζεται 60mm
    float   rebate;    // Πλάτος πατούρας φύλλου 30mm
} t_sash;

// 2. Προφίλ Κάσας (Frame Profile)
typedef struct s_frame
{
    float   width;          // Face width (π.χ. 79mm)
    float   depth;          // Profile depth (π.χ. 68mm)
    // float   visible_width;  // 47mm Το ορατό κομμάτι της κάσας
    float   rebate;   // Πλάτος πατούρας κάσας (π.χ. 32mm)
    float   sill_rebate;    // 20mm
} t_frame;

// 3. Σημείο Σύνδεσης / Πατούρες / Διάκενα (Join / Rebate Geometry)
typedef struct s_join
{
    float   overlap;        // Επικάλυψη (π.χ. 6mm)
    // float   face_overlap;   // Συνολική επικάλυψη (frame_rebate + overlap = 38mm) μαλλον πρεπει να φυγει
    float   clearance;      // Διάκενο / Αέρας (π.χ. 4mm)
    // float   meeting_stile;  // Μπινί
} t_join;

// 4. Συνολικό Κούφωμα (Main Window Input)
typedef struct s_window
{
    t_frame frame;
    t_sash  sash;
    t_join  join;
    float   width;          // Συνολικό πλάτος ανοίγματος (Window Width)
    float   height;         // Συνολικό ύψος ανοίγματος (Window Height)
} t_window;

// 5. Αποτελέσματα Κοπής / Κατασκευής (Output Results)
typedef struct s_result
{
    float  frame_w;
    float   frame_h;

    float   frame_horizontal_cut;   // Πάνω & Κάτω κάσα (Head / Sill) -> 2 κομμάτια
    float   frame_vertical_cut;     // Αριστερό & Δεξί μπόγι (Jambs) -> 2 κομμάτια

    float   sash_horizontal_cut;    // Πάνω & Κάτω τραβέρσα (Rails) -> 2 κομμάτια
    float   sash_vertical_cut;      // Μπόγια φύλλου (Stiles)       -> 2 κομμάτια

    float   sash_width;     // Τελικό πλάτος φύλλου (για κοπή)
    float   sash_height;    // Τελικό ύψος φύλλου (για κοπή)
    
    float   glass_width;    // Διαστάσεις τζαμιού πλάτος
    float   glass_height;   // Διαστάσεις τζαμιού ύψος
} t_result;


// Συναρτήσεις υπολογισμού
void calculate_dimensions(t_window *win, t_result *res);



void activate(GtkApplication *app, gpointer data);
void draw_function (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data);

#endif