//
// Created by Marek on 19/10/2026.
//
#ifndef O_PDF_IMPORT_H
#define O_PDF_IMPORT_H

// Your structural global environment parameters passed from the main drawing context
typedef struct {
    double SkalaF;                // Fallback / default to 1.0
    double Jednostki;             // Fallback / default to 1.0 (mm)
    double Exaggeration;
    double target_origin_x;       // Where to center it on your canvas (default 0.0)
    double target_origin_y;       // Where to center it on your canvas (default 0.0)
    double pdf_page_height;       // NEW: Page height in points read directly from JSON
    int current_active_layer_idx; // Fixed cleanly to Layer 0
} AlfaCadSheetSettings;

/**
 * Parses the generated JSON list stream and compiles native AlfaCAD entities
 * directly into the provided memory block allocation chain.
 *
 * @param json_list_path The absolute disk file path to the list.txt document.
 * @param target_block   Pointer to the active BLOK structure context (or NULL).
 * @param settings       The structural blueprint environment settings tracking state.
 * @return               True if the document layers compiled successfully without faults.
 */

#ifdef __cplusplus
extern "C" {
#endif
int ImportPdfDumpToAlfaCad(char *alfa_file_name, const char* json_list_path, BLOK* target_block, AlfaCadSheetSettings *settings);
extern int Load_PNG_JPG_real(B_PCX *b_pcx, char *nazwa_pcx, size_t width, size_t height, int horz_res, int vert_res, unsigned char bits_per_pixel, int type);
extern int LoadPNGinfo(char *filename, unsigned int *width, unsigned int *height, unsigned int *dpiX, unsigned int *dpiY, unsigned char* bits_per_pixel);
extern int LoadJPGinfo(char* filename, unsigned int* width, unsigned int* height, unsigned int* dpiX, unsigned int* dpiY, unsigned char* bits_per_pixel);
extern void Nowy_rysunek(void);
extern void New_window_empty(void);
extern int GetActiveMdiSlotCount(void);
extern void set_st_jedn(void);
extern void Skala_format (void);
extern void set_layer_visibility(int layer_no, int is_visible);
extern double get_st_jedn(char *st_jedn);
#ifndef LINUX
extern BOOL glb_silent;
#endif
int FindOrCreateAlfaCadLayer(const char* desired_layer);

#ifdef __cplusplus
}
#endif
#ifdef LINUX
extern BOOL glb_silent;
#endif
#endif // O_PDF_IMPORT_H

