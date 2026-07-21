#include <stdio.h>

void generate_correct_elevation(double total_w, double total_h) {
    FILE *f = fopen("elevation_fixed.cmd", "w");
    if (!f) return;

    // --- ΔΙΑΣΤΑΣΕΙΣ ΚΑΣΑΣ ---
    double frame_side = 68.0;   // Πλάτος προφίλ κάσας αριστερά/δεξιά
    double frame_bottom = 80.0; // Κατωκάσι (κάτω)
    double frame_top = 68.0;    // Πάνω κάσα

    // --- ΥΠΟΛΟΓΙΣΜΟΣ ΦΩΤΟΣ ΚΑΣΑΣ (Clear Opening) ---
    double light_left = frame_side;
    double light_right = total_w - frame_side;
    double light_bottom = frame_bottom;
    double light_top = total_h - frame_top;

    // --- ΚΑΝΟΝΑΣ ΦΥΛΛΟΥ (Από το σχέδιό σου) ---
    double sash_total_width = 67.0; // Η διάσταση 67 στο πάνω μέρος
    double sash_step = 38.0;        // Η διάσταση 38 στο κάτω μέρος

    // ==========================================
    // ΣΧΕΔΙΑΣΗ ΣΤΟ LIBRECAD
    // ==========================================

    // 1. Εξωτερική Κάσα
    fprintf(f, "rect\n0,0\n%f,%f\n\n", total_w, total_h);

    // 2. Εσωτερική Κάσα (Φως Κάσας)
    fprintf(f, "rect\n%f,%f\n%f,%f\n\n", light_left, light_bottom, light_right, light_top);

    // 3. Εξωτερικό Περίγραμμα Φύλλου (Η μεγάλη πλευρά, διάσταση 67)
    // Το φύλλο εκτείνεται κατά 67mm προς το εσωτερικό του κουφώματος από το φως της κάσας
    double sash_ext_left = light_left + sash_total_width;
    double sash_ext_right = light_right - sash_total_width;
    double sash_ext_bottom = light_bottom + sash_total_width;
    double sash_ext_top = light_top - sash_total_width;
    
    fprintf(f, "rect\n%f,%f\n%f,%f\n\n", sash_ext_left, sash_ext_bottom, sash_ext_right, sash_ext_top);

    // 4. Το Πατούρισμα / Σκαλοπάτι του Φύλλου (Διάσταση 38)
    // Η γραμμή που πατάει πάνω στην κάσα
    double sash_step_left = light_left + sash_step;
    double sash_step_right = light_right - sash_step;
    double sash_step_bottom = light_bottom + sash_step;
    double sash_step_top = light_top - sash_step;

    fprintf(f, "rect\n%f,%f\n%f,%f\n\n", sash_step_left, sash_step_bottom, sash_step_right, sash_step_top);

    // --- ΑΥΤΟΜΑΤΕΣ ΔΙΑΣΤΑΣΕΙΣ ΓΙΑ ΕΠΑΛΗΘΕΥΣΗ ---
    // Διάσταση 38 (Από το φως της κάσας έως το σκαλοπάτι του φύλλου)
    fprintf(f, "dimhorizontal\n%f,%f\n%f,%f\n%f,%f\n\n", 
            light_left, light_bottom - 10.0, 
            sash_step_left, light_bottom - 10.0, 
            light_left + (sash_step / 2.0), light_bottom - 25.0);

    // Διάσταση 67 (Από το φως της κάσας έως την εσωτερική άκρη του φύλλου)
    fprintf(f, "dimhorizontal\n%f,%f\n%f,%f\n%f,%f\n\n", 
            light_left, light_bottom - 40.0, 
            sash_ext_left, light_bottom - 40.0, 
            light_left + (sash_total_width / 2.0), light_bottom - 55.0);

    fclose(f);
    printf("Το διορθωμένο script 'elevation_fixed.cmd' δημιουργήθηκε!\n");
}

int main() {
    // Δοκιμή με συνολικές εξωτερικές διαστάσεις κάσας 600x600
    generate_correct_elevation(600.0, 600.0);
    return 0;
}