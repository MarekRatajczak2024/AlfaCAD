/*   ______   ___       ___         ____     ______  ____
*   /\  _  \ /\_ \    /'___\       /\  _`\  /\  _  \/\  _`\
*   \ \ \L\ \\//\ \  /\ \__/   __  \ \ \/\_\\ \ \L\ \ \ \/\ \
*    \ \  __ \ \ \ \ \ \ ,__\/'__`\ \ \ \/_/_\ \  __ \ \ \ \ \
*     \ \ \/\ \ \_\ \_\ \ \_/\ \L\.\_\ \ \L\ \\ \ \/\ \ \ \_\ \
*      \ \_\ \_\/\____\\ \_\\ \__/.\_\\ \____/ \ \_\ \_\ \____/
*       \/_/\/_/\/____/ \/_/ \/__/\/_/ \/___/   \/_/\/_/\/___/
*
*   A CAD
*
*   By Marek Ratajczak 2024
*
*   See readme_alfacad.txt for copyright information.
*
*/
//
// Created by Marek on 19/10/2026.
//
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>

#include <forwin.h>
#include "bib_e.h"
#include "rysuj_e.h"
#include "bgiext.h"
#include "o_libfun.h"
#include "o_pdf_import.h"
#include "o_hatch.h"
#include "o_lttype.h"
#include "alffont.h"

#ifdef min
    #undef min
#endif
#ifdef max
    #undef max
#endif

#ifdef byte
    #define ALFACAD_RESTORE_BYTE byte
    #undef byte
#endif
#ifdef byte_
    #define ALFACAD_RESTORE_BYTE_ALT byte_
    #undef byte_
#endif

// 3. Include nlohmann JSON library here in a fully protected layout environment
#include "json.hpp"

// 4. Restore byte bindings for the rest of o_pdf_import.cpp if your local file functions need them
#ifdef ALFACAD_RESTORE_BYTE
    #define byte ALFACAD_RESTORE_BYTE
    #undef ALFACAD_RESTORE_BYTE
#endif
#ifdef ALFACAD_RESTORE_BYTE_ALT
    #define byte_ ALFACAD_RESTORE_BYTE_ALT
    #undef ALFACAD_RESTORE_BYTE_ALT
#endif

// 5. Replace min/max with standard inline templates for the remaining code in this file
template <typename T1, typename T2>
inline auto min(const T1& a, const T2& b) {
    return (a < b) ? a : b;
}

template <typename T1, typename T2>
inline auto max(const T1& a, const T2& b) {
    return (a > b) ? a : b;
}


extern int hatch_proc (long_long l_offb, long_long l_offe, int seeds_no,
                 double df_pointx[], double df_pointy[],
                 T_PTR_Hatch_Param ptrs_hatch_param, int comput_area,
                 double df_apx1, double df_apy1, double df_apx2, double df_apy2, void *adb, int to_block, int draw);  //to_block=0, int draw=TRUE

extern void set_solidhatchtranslucency(int poz0, int opcja);
extern "C" TTF_font_files ttf_font_files;
// Native AlfaCAD function linkage declared extern
extern "C" int Add_New_Font_TTF_OTF(char* name, int type, int cur);

#include "nlohmann/json.hpp" // Using the standard nlohmann/json library

using json = nlohmann::json;

// ============================================================================
// 1. AlfaCAD Memory Engine Structures & Globals
// ============================================================================
typedef struct { double x, y; } CAD_POINT;
typedef struct { int r, g, b; } CAD_COLOR;

// Global tracking map for the active file execution pass
std::map<double, int> g_width_map;
std::map<int, std::string> g_layer_registry; // Up-to-date document global layer dictionary

BOOL layer_set[MAX_NUMBER_OF_LAYERS]={0};

// ============================================================================
// Up-to-Date Layer Mapping Router (Fulfilling Handshake Rule #2)
// ============================================================================
int GetLayerMappingIndex_(const json& node) {
    // 1. Read integer code layer_id out of the unified geometry/text leaf node
    // If missing, default straight to baseline layer index 0
    int layer_id = node.value("layer_id", 0);

    // 2. Fallback constraint: If index doesn't exist in our global registry table map, use 0
    if (g_layer_registry.find(layer_id) == g_layer_registry.end()) {
        layer_id = 0;
    }

    // 3. Resolve the actual text name registered for this index
    std::string layer_name_str = g_layer_registry[layer_id];

    if (layer_id>0)
    {
        int a=0;
    }

    // Enforce Rule #2: Eliminate any legacy "LAYER_0" strings from getting to your CAD kernel
    if (layer_name_str == "LAYER_0" || layer_name_str.empty()) {
        layer_name_str = "0";
    }

    // 4. Pass down cleanly to your drawing environment layer factory
    int target_layer_idx = FindOrCreateAlfaCadLayer(layer_name_str.c_str());

    if (!layer_set[target_layer_idx]) {
        int is_visible = node.value("is_visible", 1);
        set_layer_visibility(target_layer_idx, is_visible);
        layer_set[target_layer_idx] = 1;
    }

    return target_layer_idx;
}

int GetLayerMappingIndex(const json& node) {
    // 1. Ingest raw value directly from the geometry/text leaf node
    int layer_id = node.value("layer_id", 0);

    // ========================================================================
    // PURE DIAGNOSTIC ENTRY POINT: Check what value came from the file
    // ========================================================================
    if (layer_id > 0) {
        int a = 0; // 🎯 BREAKPOINT HERE: See your real IDs (like 3, 26) as they parse!
    }

    // 2. Resolve the text name from your map tracker array registers
    std::string layer_name_str = "0";

    if (g_layer_registry.find(layer_id) != g_layer_registry.end()) {
        layer_name_str = g_layer_registry[layer_id];
    }

    if (layer_name_str.empty()) {
        layer_name_str = "0";
    }

    // 3. Hand over straight to your native CAD workspace layer allocator
    int target_layer_idx = FindOrCreateAlfaCadLayer(layer_name_str.c_str());

    if (target_layer_idx >= 0 && target_layer_idx < MAX_NUMBER_OF_LAYERS) {
        if (!layer_set[target_layer_idx]) {
            int is_visible = node.value("is_visible", 1);
            set_layer_visibility(target_layer_idx, is_visible);
            layer_set[target_layer_idx] = 1;
        }
    }

    return target_layer_idx;
}


void UpdateAlfaCadCanvasSize(double paper_w_mm, double paper_h_mm)
{
    FormatX = paper_w_mm;
    FormatY = paper_h_mm;
    X_max = jednostkiplt(FormatX);
    Y_max = jednostkiplt(FormatY);
    Xmin=0.;
    Ymin=0.;
    Xmax = X_max + Xmin;
    Ymax = Y_max + Ymin;
    if (X > Xmax) X = Xmax;
    if (Y > Ymax) Y = Ymax;
}

void UpdateAlfaCadScaleUnits(double scale, double units)
{
    double krok_s0;
    double krok_g0;

    krok_s0=milimetryob(krok_s);
    krok_g0=milimetryob(krok_g);

    SkalaF = scale;
    Jednostki = units;

    set_st_jedn();

    if (!snap_)krok_s = jednostkiY(1);
    else krok_s = jednostkiOb(krok_s0);
    krok_g=jednostkiOb (krok_g0) ;
}

void AnalyzeUniqueWidths(const json& pages_array) {
    std::set<double> raw_unique_widths;
    g_width_map.clear();

    // Scan unified payload array loop safely
    for (const auto& page : pages_array) {
        if (page.contains("vector_geometry")) {
            for (const auto& path : page["vector_geometry"]) {
                if (path.contains("line_width")) {
                    double w = path["line_width"].get<double>();
                    if (w > 0.0) raw_unique_widths.insert(w);
                }
            }
        }
    }

    if (raw_unique_widths.empty()) return;

    std::vector<double> raw_sorted(raw_unique_widths.begin(), raw_unique_widths.end());
    std::vector<double> clustered_tiers;
    clustered_tiers.push_back(raw_sorted[0]);

    for (size_t i = 1; i < raw_sorted.size(); ++i) {
        if (std::abs(raw_sorted[i] - clustered_tiers.back()) > 0.05) {
            clustered_tiers.push_back(raw_sorted[i]);
        }
    }

    size_t tier_count = clustered_tiers.size();
    std::map<double, int> tier_mapping;
    for (double raw_w : raw_sorted) {
        int matched_tier = 0;
        double min_diff = 1e9;

        for (int t_idx = 0; t_idx < static_cast<int>(tier_count); ++t_idx) {
            double diff = std::abs(raw_w - clustered_tiers[t_idx]);
            if (diff < min_diff) {
                min_diff = diff;
                matched_tier = t_idx;
            }
        }
        tier_mapping[raw_w] = matched_tier;
    }

    for (double raw_w : raw_sorted) {
        int tier = tier_mapping[raw_w];

        if (tier_count == 1) {
            g_width_map[raw_w] = 2;
        }
        else if (tier_count == 2) {
            g_width_map[raw_w] = (tier == 0) ? 1 : 2;
        }
        else if (tier_count == 3) {
            g_width_map[raw_w] = (tier == 0) ? 1 : (tier == 1 ? 2 : 3);
        }
        else if (tier_count == 4) {
            g_width_map[raw_w] = tier + 1;
        }
        else if (tier_count == 5) {
            g_width_map[raw_w] = tier;
        }
        else {
            double relative_position = (double)tier / (double)tier_count;
            if (relative_position < 0.20)      g_width_map[raw_w] = 0;
            else if (relative_position < 0.40) g_width_map[raw_w] = 1;
            else if (relative_position < 0.65) g_width_map[raw_w] = 2;
            else if (relative_position < 0.85) g_width_map[raw_w] = 3;
            else                               g_width_map[raw_w] = 4;
        }
    }
}

int MapThicknessToCadEnum(double extracted_width_dots, const AlfaCadSheetSettings* s) {
    if (g_width_map.find(extracted_width_dots) != g_width_map.end()) {
        return g_width_map[extracted_width_dots];
    }
    return 2; // Secure thick standard fallback
}

// ============================================================================
// Color Index Forensic Recovery Utility
// ============================================================================
unsigned char MapRgbToAlfaCadColorIndex(CAD_COLOR pdf_rgb) {
    // 1. Scale down from 8-bit to 6-bit space matching hardware DAC maps
    int r_6bit = pdf_rgb.r >> 2;
    int g_6bit = pdf_rgb.g >> 2;
    int b_6bit = pdf_rgb.b >> 2;

    int swizzled_index = 0;
    long min_distance_sq = 2000000000;

    // 2. Nearest-neighbor match inside your active engine table tracks
    for (int i = 0; i < 256; i++) {
        long dr = r_6bit - _dac_normal[i][0];
        long dg = g_6bit - _dac_normal[i][1];
        long db = b_6bit - _dac_normal[i][2];

        long distance_sq = (dr * dr) + (dg * dg) + (db * db);

        if (distance_sq < min_distance_sq) {
            min_distance_sq = distance_sq;
            swizzled_index = i;
        }
        if (distance_sq == 0) break;
    }

    // 3. Reverse map lookups for Windows native index mappings (0-16 structural pins)
    int transf[] = { 0, 12, 14, 10, 11, 9, 64, 16,  8,  4,  6,  2,  3,  1,  5,  7,  16 };

    for (int original_idx = 0; original_idx <= 16; original_idx++) {
        if (transf[original_idx] == swizzled_index) {
            return static_cast<unsigned char>(original_idx); // Exact engine color match recovered
        }
    }

    // Return the swizzled raw index if dealing with specialized custom tones
    return static_cast<unsigned char>(swizzled_index);
}




CAD_COLOR ExtractRgbColor(const json& rgb_array) {
    CAD_COLOR c = {0, 0, 0};
    if (rgb_array.is_array() && rgb_array.size() >= 3) {
        c.r = static_cast<int>(rgb_array[0].get<double>() * 255.0);
        c.g = static_cast<int>(rgb_array[1].get<double>() * 255.0);
        c.b = static_cast<int>(rgb_array[2].get<double>() * 255.0);
    }
    return c;
}

CAD_POINT ConvertPdfCoordsToCad(double pdf_x, double pdf_y, const AlfaCadSheetSettings* s) {
    CAD_POINT pt;
    double raw_mm_x = pdf_x * (25.4 / 72.0);
    double raw_mm_y = pdf_y * (25.4 / 72.0);

    pt.x = raw_mm_x + s->target_origin_x;
    pt.y = s->pdf_page_height - raw_mm_y + s->target_origin_y;
    return pt;
}

void ProcessLineSegment_(BLOK* block, const json& seg, CAD_COLOR color, double extracted_width_dots, const AlfaCadSheetSettings *s) {
    LINIA line = Ldef;

    CAD_POINT p1 = ConvertPdfCoordsToCad(seg["p1"][0].get<double>(), seg["p1"][1].get<double>(), s);
    CAD_POINT p2 = ConvertPdfCoordsToCad(seg["p2"][0].get<double>(), seg["p2"][1].get<double>(), s);

    line.x1 = static_cast<float>(p1.x);
    line.y1 = static_cast<float>(p1.y);
    line.x2 = static_cast<float>(p2.x);
    line.y2 = static_cast<float>(p2.y);

    line.blok       = (block != nullptr) ? 1 : 0;
    line.warstwa    = GetLayerMappingIndex(seg);
    line.kolor      = MapRgbToAlfaCadColorIndex(color);

    int line_width_idx = MapThicknessToCadEnum(extracted_width_dots, s);
    line.typ = (line_width_idx * 32) + 0;

    dodaj_obiekt(block, &line);
}

// Change the second parameter name to "line_coords", and add a third parameter "path_context"
void ProcessLineSegment(BLOK* block, const json& line_coords, const json& path_context, CAD_COLOR color, double extracted_width_dots, const AlfaCadSheetSettings *s) {
    LINIA line = Ldef;

    // Use "line_coords" to extract the geometric arrays
    CAD_POINT p1 = ConvertPdfCoordsToCad(line_coords["p1"][0].get<double>(), line_coords["p1"][1].get<double>(), s);
    CAD_POINT p2 = ConvertPdfCoordsToCad(line_coords["p2"][0].get<double>(), line_coords["p2"][1].get<double>(), s);

    line.x1 = static_cast<float>(p1.x);
    line.y1 = static_cast<float>(p1.y);
    line.x2 = static_cast<float>(p2.x);
    line.y2 = static_cast<float>(p2.y);

    line.blok       = (block != nullptr) ? 1 : 0;

    // 🎯 FIXED: Direct GetLayerMappingIndex to inspect the parent path context node!
    line.warstwa    = GetLayerMappingIndex(path_context);
    line.kolor      = MapRgbToAlfaCadColorIndex(color);

    int line_width_idx = MapThicknessToCadEnum(extracted_width_dots, s);
    line.typ = (line_width_idx * 32) + 0;

    dodaj_obiekt(block, &line);
}


void ProcessEllipseOrCircle(BLOK* block, const json& path_node, CAD_COLOR color, double extracted_width_dots, const AlfaCadSheetSettings *s)
{
    double x_min = 1e9, x_max = -1e9, y_min = 1e9, y_max = -1e9;
    bool found_points = false;

    if (!path_node.contains("segments") || !path_node["segments"].is_array()) return;
    const auto& items = path_node["segments"];

    for (const auto& item : items) {
        if (item.contains("op") && item["op"].get<std::string>() == "CURVE") {
            CAD_POINT p1 = ConvertPdfCoordsToCad(item["p1"][0].get<double>(), item["p1"][1].get<double>(), s);
            CAD_POINT p4 = ConvertPdfCoordsToCad(item["p4"][0].get<double>(), item["p4"][1].get<double>(), s);

            found_points = true;

            if (p1.x < x_min) x_min = p1.x;
            if (p4.x < x_min) x_min = p4.x;
            if (p1.x > x_max) x_max = p1.x;
            if (p4.x > x_max) x_max = p4.x;
            if (p1.y < y_min) y_min = p1.y;
            if (p4.y < y_min) y_min = p4.y;
            if (p1.y > y_max) y_max = p1.y;
            if (p4.y > y_max) y_max = p4.y;
        }
    }
    if (!found_points) return;
    double x  = (x_min + x_max) / 2.0;
    double y  = (y_min + y_max) / 2.0;
    double rx = (x_max - x_min) / 2.0;
    double ry = (y_max - y_min) / 2.0;
    int line_width_idx = MapThicknessToCadEnum(extracted_width_dots, s);
    if (std::abs(rx - ry) < 0.05) {
        OKRAG okrag = Odef;
        okrag.x = (float)(x);
        okrag.y = (float)(y);
        okrag.r = (float)rx;
        okrag.warstwa  = GetLayerMappingIndex(path_node);
        okrag.blok     = (block != nullptr) ? 1 : 0;
        okrag.kolor    = MapRgbToAlfaCadColorIndex(color);
        okrag.typ      = (line_width_idx * 32) + 0;
        dodaj_obiekt(block, &okrag);
    }
    else {
        ELLIPSE ellipse = Edef;
        ellipse.x      = (float)(x);
        ellipse.y      = (float)(y);
        ellipse.rx     = (float)rx;
        ellipse.ry     = (float)ry;
        ellipse.angle  = 0.f;
        ellipse.warstwa = GetLayerMappingIndex(path_node);
        ellipse.blok    = (block != nullptr) ? 1 : 0;
        ellipse.kolor   = MapRgbToAlfaCadColorIndex(color);
        ellipse.typ     = (line_width_idx * 32) + 0;
        dodaj_obiekt(block, &ellipse);
    }
}
void ProcessPolygonOrSolid(BLOK* block, const json::value_type& path_node, const json& items, CAD_COLOR fill_col, bool is_filled, double extracted_width_dots, const AlfaCadSheetSettings* s)
{
    std::vector<CAD_POINT> master_points;
    std::vector<CAD_POINT> unique_vertices;
    char* translucency_ptr;
    int opacity_val = path_node.value("opacity_255", 255);
    int total_line_ops = 0;
    for (const auto& item : items) {
        if (item.contains("op")) {
            std::string op = item["op"].get<std::string>();
            if (op == "LINE") {
                total_line_ops++;
                CAD_POINT p1 = ConvertPdfCoordsToCad(item["p1"][0].get<double>(), item["p1"][1].get<double>(), s);
                CAD_POINT p2 = ConvertPdfCoordsToCad(item["p2"][0].get<double>(), item["p2"][1].get<double>(), s);
                master_points.push_back(p1);
                master_points.push_back(p2);
            }
            else if (op == "RECT") {
                double x0 = item["x0"].get<double>();
                double y0 = item["y0"].get<double>();
                double w  = item["w"].get<double>();
                double h  = item["h"].get<double>();
                CAD_POINT top_left  = ConvertPdfCoordsToCad(x0, y0, s);
                CAD_POINT btm_right = ConvertPdfCoordsToCad(x0 + w, y0 + h, s);
                master_points.push_back({top_left.x, top_left.y});
                master_points.push_back({btm_right.x, top_left.y});
                master_points.push_back({btm_right.x, btm_right.y});
                master_points.push_back({top_left.x, btm_right.y});
            }
        }
    }
    if (master_points.empty()) return;
    bool dash_mesh_condensed = false;
    std::vector<CAD_POINT> final_solid_vertices;
    if (total_line_ops >= 4 && total_line_ops <= 12) {
        double cluster_tolerance = 0.15;
        for (const auto& candidate : master_points) {
            bool matched = false;
            for (const auto& existing : final_solid_vertices) {
                if (std::abs(candidate.x - existing.x) < cluster_tolerance &&
                std::abs(candidate.y - existing.y) < cluster_tolerance) {
                    matched = true;
                    break;
                }
            }
            if (!matched) {
                final_solid_vertices.push_back(candidate);
            }
        }
        if (final_solid_vertices.size() == 4) {
            dash_mesh_condensed = true;
        }
    }
    if (dash_mesh_condensed || (master_points.size() >= 3 && master_points.size() <= 4))
    {
        WIELOKAT solid = S4def;
        unique_vertices = dash_mesh_condensed ? final_solid_vertices : master_points;
        if (!dash_mesh_condensed && unique_vertices.size() > 2) {
            if (std::abs(unique_vertices.back().x - unique_vertices[0].x) < 0.02 &&
            std::abs(unique_vertices.back().y - unique_vertices[0].y) < 0.02) {
                unique_vertices.pop_back();
            }
        }
        solid.lp = (unique_vertices.size() == 3) ? 6 : 8;
        if (unique_vertices.size() == 4) {
            double cx = (unique_vertices[0].x + unique_vertices[1].x + unique_vertices[2].x + unique_vertices[3].x) / 4.0;
            double cy = (unique_vertices[0].y + unique_vertices[1].y + unique_vertices[2].y + unique_vertices[3].y) / 4.0;
            std::sort(unique_vertices.begin(), unique_vertices.end(), [cx, cy](const CAD_POINT& a, const CAD_POINT& b) {
            return std::atan2(a.y - cy, a.x - cx) < std::atan2(b.y - cy, b.x - cx);
            });
        }
        for (size_t i = 0; i < unique_vertices.size(); ++i) {
            solid.xy[i*2]     = (float)unique_vertices[i].x;
            solid.xy[i*2 + 1] = (float)unique_vertices[i].y;
        }
        if (unique_vertices.size() == 3) {
            solid.xy[6] = solid.xy[4];
            solid.xy[7] = solid.xy[5];
        }
        if (!is_filled) {
            solid.empty_typ = MapThicknessToCadEnum(extracted_width_dots, s) + 1;
            solid.translucent = 0;
            solid.pattern = 0;
            solid.n = 8 + solid.lp * sizeof(float);
        } else {
            solid.empty_typ = 0;
            if (opacity_val == 255) {
                solid.translucent = 0;
                solid.n = 8 + solid.lp * sizeof(float);
            } else {
                solid.translucent = 1;
                translucency_ptr = (char*)solid.xy;
                translucency_ptr += (solid.lp * sizeof(float));
                unsigned char opacity_byte = static_cast<unsigned char>(opacity_val);
                memmove(translucency_ptr, &opacity_byte, sizeof(unsigned char));
                solid.n = 8 + solid.lp * sizeof(float) + sizeof(unsigned char);
            }
        }
        solid.warstwa = GetLayerMappingIndex(path_node);
        solid.kolor = MapRgbToAlfaCadColorIndex(fill_col);
        dodaj_obiekt(block, &solid);
    }
    else if (master_points.size() > 4)
    {
        BLOK blockd = Bdef;
        char c_pltype = is_filled ? PL_HATCH : PL_PLINE;
        blockd.n = B3 + sizeof(c_pltype);
        blockd.kod_obiektu = is_filled ? B_HATCH : B_PLINE;
        blockd.atrybut = Anormalny;
        blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
        BLOK* boundary_container = (BLOK*)dodaj_obiekt(block, &blockd);
        if (!boundary_container) return;
        memmove(&(boundary_container->opis_obiektu[0]), &c_pltype, sizeof(c_pltype));
        if (is_filled) {
            std::vector<CAD_POINT> cell_vertices;
            double epsilon = 0.01;
            struct COMMITTED_TRIANGLE { POINTD pts[3]; };
            std::vector<COMMITTED_TRIANGLE> drawn_triangles;
            for (const auto& item : items) {
                if (item.contains("op") && item["op"].get<std::string>() == "LINE") {
                    CAD_POINT p1 = ConvertPdfCoordsToCad(item["p1"][0].get<double>(), item["p1"][1].get<double>(), s);
                    CAD_POINT p2 = ConvertPdfCoordsToCad(item["p2"][0].get<double>(), item["p2"][1].get<double>(), s);
                    bool p1_exists = false;
                    for (const auto& v : cell_vertices) {
                        if (std::abs(v.x - p1.x) < epsilon && std::abs(v.y - p1.y) < epsilon) { p1_exists = true; break; }
                    }
                    if (!p1_exists) cell_vertices.push_back(p1);
                    bool p2_exists = false;
                    for (const auto& v : cell_vertices) {
                        if (std::abs(v.x - p2.x) < epsilon && std::abs(v.y - p2.y) < epsilon) { p2_exists = true; break; }
                    }
                    if (!p2_exists) cell_vertices.push_back(p2);
                    if (cell_vertices.size() >= 3) {
                        POINTD candidate_pts[3] = {
                            {cell_vertices[0].x, cell_vertices[0].y},
                            {cell_vertices[1].x, cell_vertices[1].y},
                            {cell_vertices[2].x, cell_vertices[2].y}
                        };
                        bool is_duplicate = false;
                        for (const auto& drawn : drawn_triangles) {
                            int matching_vertices = 0;
                            for (int c_idx = 0; c_idx < 3; ++c_idx) {
                                size_t d_idx; // Declared outside due to C compliance preferences
                                for (d_idx = 0; d_idx < 3; ++d_idx) {
                                    if (std::abs(candidate_pts[c_idx].x - drawn.pts[d_idx].x) < epsilon &&
                                    std::abs(candidate_pts[c_idx].y - drawn.pts[d_idx].y) < epsilon) {
                                        matching_vertices++; break;
                                    }
                                }
                            }
                            if (matching_vertices == 3) { is_duplicate = true; break; }
                        }
                        if (!is_duplicate) {
                            COMMITTED_TRIANGLE new_tri;
                            new_tri.pts[0] = candidate_pts[0];
                            new_tri.pts[1] = candidate_pts[1];
                            new_tri.pts[2] = candidate_pts[2];
                            drawn_triangles.push_back(new_tri);
                            WIELOKAT triangular_solid = Stdef;
                            triangular_solid.xy[0] = (float)candidate_pts[0].x;
                            triangular_solid.xy[1] = (float)candidate_pts[0].y;
                            triangular_solid.xy[2] = (float)candidate_pts[1].x;
                            triangular_solid.xy[3] = (float)candidate_pts[1].y;
                            triangular_solid.xy[4] = (float)candidate_pts[2].x;
                            triangular_solid.xy[5] = (float)candidate_pts[2].y;
                            triangular_solid.lp = 6;
                            triangular_solid.empty_typ = 0;
                            triangular_solid.atrybut = Anormalny;
                            triangular_solid.blok = 1;
                            if (opacity_val == 255) {
                                triangular_solid.translucent = 0;
                                triangular_solid.n = 8 + triangular_solid.lp * sizeof(float);
                            } else {
                                triangular_solid.translucent = 1;
                                translucency_ptr = (char*)triangular_solid.xy;
                                translucency_ptr += (triangular_solid.lp * sizeof(float));
                                unsigned char opacity_byte = static_cast<unsigned char>(opacity_val);
                                memmove(translucency_ptr, &opacity_byte, sizeof(unsigned char));
                                triangular_solid.n = 8 + triangular_solid.lp * sizeof(float) + sizeof(unsigned char);
                            }
                            triangular_solid.warstwa = GetLayerMappingIndex(path_node);
                            triangular_solid.kolor = MapRgbToAlfaCadColorIndex(fill_col);
                            PTR__GTMPBLOCK = (char*)boundary_container;
                            dodaj_obiekt(boundary_container, &triangular_solid);
                            boundary_container = (BLOK*)PTR__GTMPBLOCK;
                        }
                        cell_vertices.clear();
                    }
                }
            }
        }
        else {
            LINIA L = Ldef;
            L.blok    = 1;
            L.warstwa = GetLayerMappingIndex(path_node);
            L.kolor   = MapRgbToAlfaCadColorIndex(fill_col);
            L.atrybut = Anormalny;
            int line_width_idx = MapThicknessToCadEnum(extracted_width_dots, s);
            L.typ = (line_width_idx * 32) + 0;
            for (const auto& item : items) {
                if (item.contains("op") && item["op"].get<std::string>() == "LINE")  {
                    CAD_POINT p1 = ConvertPdfCoordsToCad(item["p1"][0].get<double>(), item["p1"][1].get<double>(), s);
                    CAD_POINT p2 = ConvertPdfCoordsToCad(item["p2"][0].get<double>(), item["p2"][1].get<double>(), s);
                    L.x1 = (float)p1.x;
                    L.y1 = (float)p1.y;
                    L.x2 = (float)p2.x;
                    L.y2 = (float)p2.y;
                    PTR__GTMPBLOCK = (char*)boundary_container;
                    dodaj_obiekt(boundary_container, &L);
                    boundary_container = (BLOK*)PTR__GTMPBLOCK;
                }
            }
        }
    }
}
unsigned char MapPdfFontToAlfaCadIndex(const std::string& pdf_font_name)
{
    std::string lower_pdf_name = pdf_font_name;
    std::transform(lower_pdf_name.begin(), lower_pdf_name.end(), lower_pdf_name.begin(), ::tolower);
    size_t hyphen_pos = lower_pdf_name.find_first_of("-,");
    std::string base_family_name = (hyphen_pos == std::string::npos) ? lower_pdf_name : lower_pdf_name.substr(0, hyphen_pos);
    for (int i = 0; i < MaxNumberTextStyle; i++) {
        if (ttf_font_files.file[i] != nullptr && std::strlen(ttf_font_files.file[i]) > 0) {
            std::string local_font_file = ttf_font_files.file[i];
            std::transform(local_font_file.begin(), local_font_file.end(), local_font_file.begin(), ::tolower);
            size_t last_slash = local_font_file.find_last_of("\\/");
            std::string raw_family = (last_slash == std::string::npos) ? local_font_file : local_font_file.substr(last_slash + 1);
            size_t dot_pos = raw_family.find_last_of('.');
            if (dot_pos != std::string::npos) {
                raw_family = raw_family.substr(0, dot_pos);
            }
            if (base_family_name.find(raw_family) != std::string::npos || raw_family.find(base_family_name) != std::string::npos) {
                return static_cast<unsigned char>(i);
            }
        }
    }
    if (WhNumberTextStyle < MaxNumberTextStyle) {
        char font_name_buffer[128];
        std::strncpy(font_name_buffer, base_family_name.c_str(), sizeof(font_name_buffer) - 1);
        font_name_buffer[sizeof(font_name_buffer) - 1] = '\0';
        int newly_loaded_font_idx = Add_New_Font_TTF_OTF(font_name_buffer, 0, 1);
        if (newly_loaded_font_idx >= 0 && newly_loaded_font_idx < MaxNumberTextStyle) {
            return static_cast<unsigned char>(newly_loaded_font_idx);
        }
    }
    return 0;
}
void ProcessTextLayer(BLOK* block, const json& txt_obj, const AlfaCadSheetSettings* s)
{
    TEXT text_node = Tdef;
    double m_a = 1.0, m_b = 0.0, m_c = 0.0, m_d = 1.0;
    if (txt_obj.contains("matrix") && txt_obj["matrix"].is_array() && txt_obj["matrix"].size() >= 4) {
        m_a = txt_obj["matrix"][0].get<double>();
        m_b = txt_obj["matrix"][1].get<double>();
        m_c = txt_obj["matrix"][2].get<double>();
        m_d = txt_obj["matrix"][3].get<double>();
    }
    double calculated_angle_rad = std::atan2(m_b, m_a);
    text_node.kat = static_cast<float>(calculated_angle_rad);
    double combined_scale_x = std::sqrt((m_a * m_a) + (m_b * m_b));
    text_node.width_factor = static_cast<float>(combined_scale_x / 1.0);
    double skew_angle_rad = std::atan2(-m_c, m_d);
    text_node.italics = (std::abs(skew_angle_rad - calculated_angle_rad) > 0.05) ? 1 : 0;
    CAD_POINT origin = ConvertPdfCoordsToCad(txt_obj["origin"][0].get<double>(), txt_obj["origin"][1].get<double>(), s);
    text_node.x = static_cast<float>(origin.x);
    text_node.y = static_cast<float>(origin.y);
    double extracted_font_points = txt_obj["font_size"].get<double>();
    text_node.kat = static_cast<float>(txt_obj["text_angle_rad"].get<double>());
    text_node.italics = static_cast<unsigned>(txt_obj["italics"].get<bool>());
    double font_paper_mm = extracted_font_points * (25.4 / 72.0);
    font_paper_mm = font_paper_mm / 1.3119;
    if (font_paper_mm > 50.0) {
        text_node.wysokosc = 3.5f;
    } else if (font_paper_mm < 1.0) {
        text_node.wysokosc = 1.8f;
    } else {
        text_node.wysokosc = static_cast<float>(font_paper_mm);
    }
    std::string raw_font_name = txt_obj.value("font_name", "");
    text_node.czcionka = MapPdfFontToAlfaCadIndex(raw_font_name);
    std::string font_name_str = raw_font_name;
    std::transform(font_name_str.begin(), font_name_str.end(), font_name_str.begin(), ::tolower);
    text_node.bold = (font_name_str.find("bold") != std::string::npos) ? 1 : 0;
    text_node.warstwa  = GetLayerMappingIndex(txt_obj);
    text_node.encoding = 1;
    text_node.blok     = (block != nullptr) ? 1 : 0;
    if (txt_obj.contains("color") && txt_obj["color"].is_array()) {
        CAD_COLOR txt_rgb;
        txt_rgb.r = txt_obj["color"][0].get<int>();
        txt_rgb.g = txt_obj["color"][1].get<int>();
        txt_rgb.b = txt_obj["color"][2].get<int>();
        text_node.kolor = MapRgbToAlfaCadColorIndex(txt_rgb);
    }
    std::string content_str = txt_obj["text"].get<std::string>();
    std::strncpy(text_node.text, content_str.c_str(), MaxMultitextLen);
    text_node.text[MaxMultitextLen] = '\0';
    text_node.dl = static_cast<unsigned int>(std::strlen(text_node.text));
    text_node.n = T18 + text_node.dl;
    dodaj_obiekt(block, (void*)&text_node);
}
//extern int Load_PNG_JPG_real(B_PCX *b_pcx, char *nazwa_pcx, size_t width, size_t height, int horz_res, int vert_res, unsigned char bits_per_pixel, int type);
void ProcessImageLayer(BLOK* target_block, const json& img_obj, const AlfaCadSheetSettings* s)
{
    B_PCX b_pcx = PCXdef;
    unsigned int width, height, horz_res, vert_res;
    int info_ret;
    int type;
    unsigned char bits_per_pixel;
    PCXheader head;
    std::string path_str = img_obj["file_path"].get<std::string>();
    b_pcx.i_x = img_obj["pixel_width"].get<int>();
    b_pcx.i_y = img_obj["pixel_height"].get<int>();
    double pdf_w = img_obj["pdf_width_pt"].get<double>();
    double pdf_h = img_obj["pdf_height_pt"].get<double>();
    double total_mm_w = pdf_w * (25.4 / 72.0);
    double total_mm_h = pdf_h * (25.4 / 72.0);
    b_pcx.dx = static_cast<float>(total_mm_w / (double)b_pcx.i_x);
    b_pcx.dy = static_cast<float>(total_mm_h / (double)b_pcx.i_y);
    double pdf_x = img_obj["pdf_x_insert"].get<double>();
    double pdf_y = img_obj["pdf_y_insert"].get<double>();
    double raw_mm_x = pdf_x * (25.4 / 72.0);
    double raw_mm_y = pdf_y * (25.4 / 72.0);
    b_pcx.x = static_cast<float>(raw_mm_x + s->target_origin_x);
    b_pcx.y = static_cast<float>((s->pdf_page_height - raw_mm_y) + s->target_origin_y);
    b_pcx.kat        = static_cast<float>(img_obj["angle_deg"].get<double>());
    b_pcx.widoczny   = 1;
    b_pcx.kod_obiektu = 2;
    b_pcx.warstwa    = GetLayerMappingIndex(img_obj);
    b_pcx.blok       = (target_block != nullptr) ? 1 : 0;

    int is_on_top = img_obj.value("is_on_top", 0);
    int is_background = img_obj.value("is_background", false);

    if (is_on_top == 1) {
        b_pcx.on_front = 1;                  // Forces foreground skin elements to render over the backdrop
        b_pcx.ignore_background = 0;         // Prevent background masks from choking out details
        b_pcx.background = 255;              // Make the layer texture context fully transparent/clear
    } else {
        b_pcx.on_front = 0;
        b_pcx.ignore_background = 1;
        b_pcx.background = 15;
    }

    b_pcx.v_flip                  = 0;
    b_pcx.h_flip                  = 0;
    b_pcx.inwersja                = 0;
    //b_pcx.ignore_background       = 1;
    b_pcx.set_foreground          = 0;
    b_pcx.ignore_print_background = 1;
    b_pcx.markers = 0;
    b_pcx.stripped = 0;

    b_pcx.buffered = 0;
    //b_pcx.background              = 15;
    b_pcx.foreground              = 255;
    std::strncpy(b_pcx.pcx, path_str.c_str(), sizeof(b_pcx.pcx) - 1);
    b_pcx.pcx[sizeof(b_pcx.pcx) - 1] = '\0';
    if (strstr(b_pcx.pcx, ".jpeg") != nullptr) type=1;
    else  type=0;
    if (type==0) {
        info_ret = LoadPNGinfo(b_pcx.pcx, &width, &height, &horz_res, &vert_res, &bits_per_pixel);
        b_pcx.kod_obiektu=2;
    }
    else {
        info_ret = LoadJPGinfo(b_pcx.pcx, &width, &height, &horz_res, &vert_res, &bits_per_pixel);
        b_pcx.kod_obiektu=3;
    }
    b_pcx.i_x = static_cast<int>(width);
    b_pcx.i_y = static_cast<int>(height);
    if (horz_res == 0) horz_res = 150;
    if (vert_res == 0) vert_res = 150;
    head.xmin = 0;
    head.xmax = static_cast<short>(width);
    head.ymin = 0;
    head.ymax = static_cast<short>(height);
    head.bits_per_pixel = bits_per_pixel;
    head.horz_res = static_cast<short>(horz_res);
    head.vert_res = static_cast<short>(vert_res);
    if (!Load_PNG_JPG_real(&b_pcx, b_pcx.pcx, width, height, horz_res, vert_res, bits_per_pixel, type))
        return;
    bitmap_exist = TRUE;
    bitmap_png_exist = TRUE;
    bitmap_view = TRUE;
}

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void restoreAlfaCADParameters(const nlohmann::json& root, AlfaCadSheetSettings *settings)
{
    settings->SkalaF=1.0;
    settings->Jednostki=1.0;
    settings->Exaggeration=1.0;

    // 1. Read the raw metadata string from nlohmann::json object
    std::string smuggledStr = root.value("smuggled_metadata", "");
    if (smuggledStr.empty()) return;

    // Default target parameter buffers
    double scaleFactor = 1.0;
    std::string units = "mm";
    double exaggeration = 1.0;

    // 2. Parse out individual elements via RegEx
    // Matches patterns like: Scale 1:10, [mm], and (Exagg. 50x)
    std::smatch match;

    // Extract scale ratio (e.g., "1:10" -> captures 10)
    if (std::regex_search(smuggledStr, match, std::regex(R"(Scale\s+\d+:([0-9.]+))"))) {
        scaleFactor = std::stod(match[1].str());
    }

    // Extract units inside brackets (e.g., "[mm]")
    if (std::regex_search(smuggledStr, match, std::regex(R"(\[(\w+)\])"))) {
        units = match[1].str();
    }

    // Extract exaggeration number (e.g., "(Exagg. 50x)" -> captures 50)
    if (std::regex_search(smuggledStr, match, std::regex(R"(\(Exagg\.\s+([0-9.]+)x\))"))) {
        exaggeration = std::stod(match[1].str());
    }

    // 3. Apply variables to AlfaCAD environment
    //std::cout << "Restored Parameters:\n";
    //std::cout << "Scale Factor: " << scaleFactor << "\n";
    //std::cout << "Units:        " << units << "\n";
    //std::cout << "Exaggeration: " << exaggeration << "x\n";

    settings->SkalaF=scaleFactor;
    settings->Jednostki=get_st_jedn((char*)units.c_str());
    settings->Exaggeration=exaggeration;
}

// ============================================================================
// 4. Clean Master Importer Entry Point (Integrating Unified Root Layout)
// ============================================================================
extern "C" int ImportPdfDumpToAlfaCad(char *alfa_file_name, const char *json_list_path, BLOK* target_block, AlfaCadSheetSettings *settings)
{
    LAYER *Master_Layers=nullptr;
    int Master_No_Layers = 0;
    if (!json_list_path) return 0;
    std::ifstream file(json_list_path);
    if (!file.is_open()) return 0;
    json root;
    try {
        file >> root;
    } catch (...) {
        return 0;
    }
    if (root.empty()) return 0;

    Master_Layers=(LAYER*)malloc(MAX_NUMBER_OF_LAYERS*sizeof(LAYER));

    restoreAlfaCADParameters(root, settings);

    // --- PASS 1: FIND LOGICAL BOUNDS & DYNAMICALLY ALLOCATE CHANNELS ---
    int max_file_layer_idx = 0;
    nlohmann::json layer_table = root["layer_registry_table"];
    if (layer_table.contains("layer_registry_table")) {
        layer_table = layer_table["layer_registry_table"];
    }

    if (layer_table.is_object()) {
        for (auto& [key, value] : layer_table.items()) {
            if (key.find_first_not_of("0123456789") == std::string::npos) {
                max_file_layer_idx = max(max_file_layer_idx, std::stoi(key));
            }
        }
    }

    // Set your tracking boundary dynamically based on the file content bounds
    Master_No_Layers = min(max_file_layer_idx + 1, MAX_NUMBER_OF_LAYERS);

    // Initialize ONLY the required range with default parameters (e.g. 0 to 87 instead of 65535)
    for (int i = 0; i < Master_No_Layers; i++) {
        strncpy(Master_Layers[i].name, "", maxlen_w);
        Master_Layers[i].on = 1;
        Master_Layers[i].edit = 1;
        Master_Layers[i].point = 1;
        Master_Layers[i].color = 7;
        Master_Layers[i].line_type = 64;
        Master_Layers[i].grey = 0;
        Master_Layers[i].bw = 0;
        Master_Layers[i].len_name = 1;
        Master_Layers[i].reserve = 0;
    }

    // 1. POPULATE THE UNIFIED GLOBAL LAYER REGISTRY MAP
    g_layer_registry.clear();

    if (layer_table.is_object()) {
        for (auto& [key, value] : layer_table.items()) {
            if (key.find_first_not_of("0123456789") != std::string::npos) continue;

            int layer_idx = std::stoi(key);
            if (layer_idx >= MAX_NUMBER_OF_LAYERS) continue;

            std::string layer_name = value.get<std::string>();
            if (layer_name == "LAYER_0" || layer_name == "None" || layer_name.empty()) {
                layer_name = "0";
            }

            g_layer_registry[layer_idx] = layer_name;

            // This direct write is now completely safe, regardless of JSON sorting!
            strncpy(Master_Layers[layer_idx].name, layer_name.c_str(), maxlen_w);
        }
    }
    // Ensure safe default fallback definition exists in memory
    if (g_layer_registry.find(0) == g_layer_registry.end()) {
        g_layer_registry[0] = "0";
    }
    // Check for the "pages" array inside the root structural dictionary
    if (!root.contains("pages") || !root["pages"].is_array()) {
        return 0;
    }
    const auto& pages_array = root["pages"];
    std::memset(layer_set, 0, sizeof(layer_set));
    // Run thickness evaluation pass on the pages array payload directly
    AnalyzeUniqueWidths(pages_array);
    int pages_processed = 0;
    char sk[MAXPATH] = "";
    for (const auto& page : pages_array) {
        if (pages_processed == 0) {
            Change=FALSE;
            Nowy_rysunek();
            sprintf(sk,"%s#%d.alf", alfa_file_name, pages_processed);
            strcpy(zbior_danych, sk);
            Skala_format();

            No_Layers = Master_No_Layers;
            for (int i = 0; i < No_Layers; i++) {
                memmove(&Layers[i],&Master_Layers[i], sizeof(LAYER));
                //Layers[i] = Master_Layers[i];
            }
        }
        else {
            if (GetActiveMdiSlotCount() == -1) {
                break;
            }
            New_window_empty();
            sprintf(sk,"%s#%d.alf", alfa_file_name, pages_processed);
            strcpy(zbior_danych, sk);
            Skala_format();
            No_Layers = Master_No_Layers;
            for (int i = 0; i < No_Layers; i++) {
                memmove(Layers,Master_Layers, sizeof(LAYER));
                //Layers[i] = Master_Layers[i];
            }
        }
        double file_page_width  = page["width"].get<double>();
        double file_page_height = page["height"].get<double>();
        double paper_w_mm = file_page_width  * (25.4 / 72.0);
        double paper_h_mm = file_page_height * (25.4 / 72.0);
        settings->pdf_page_height = paper_h_mm;
        UpdateAlfaCadCanvasSize(paper_w_mm, paper_h_mm);
        UpdateAlfaCadScaleUnits(settings->SkalaF, settings->Jednostki);
        // Step 1: Compile Vector Art layers
        if (page.contains("vector_geometry")) {
            for (const auto& path : page["vector_geometry"]) {
                double raw_width = path.value("line_width", 1.0);
                std::string type = path["type"].get<std::string>();
                CAD_COLOR stroke_color = ExtractRgbColor(path["stroke_color"]);
                if (!path.contains("segments") || path["segments"].empty()) continue;
                const auto& segments = path["segments"];
                bool only_curves = true;
                bool only_lines = true;
                for (const auto& seg : segments) {
                    if (seg.contains("op")) {
                        std::string op = seg["op"].get<std::string>();
                        if (op != "CURVE") only_curves = false;
                        if (op != "LINE" && op != "RECT") only_lines = false;
                    }
                }
                if (segments.size() == 4 && only_curves && (type == "f" || type == "fs")) {
                    ProcessEllipseOrCircle(target_block, path, stroke_color, raw_width, settings);
                }
                else if (type == "f" || type == "fs") {
                    CAD_COLOR fill_color;
                    if (path.contains("fill_color") && !path["fill_color"].is_null()) {
                        fill_color = ExtractRgbColor(path["fill_color"]);
                    } else {
                        fill_color = stroke_color;
                    }
                    ProcessPolygonOrSolid(target_block, path, segments, fill_color, true, raw_width, settings);
                }
                else {
                    for (const auto& seg : path["segments"]) {
                        if (!seg.contains("op")) continue;
                        std::string op = seg["op"].get<std::string>();
                        if (op == "LINE") {
                            ProcessLineSegment(target_block, seg, path, stroke_color, raw_width, settings);
                        }
                        else if (op == "CURVE") {
                            double x1 = seg["p1"][0].get<double>(); double y1 = seg["p1"][1].get<double>();
                            double x2 = seg["p2"][0].get<double>(); double y2 = seg["p2"][1].get<double>();
                            double x3 = seg["p3"][0].get<double>(); double y3 = seg["p3"][1].get<double>();
                            double x4 = seg["p4"][0].get<double>(); double y4 = seg["p4"][1].get<double>();
                            BLOK blockd = Bdef;
                            char c_pltype = PL_PLINE;
                            blockd.n = B3 + sizeof(c_pltype);
                            blockd.kod_obiektu = B_PLINE;
                            blockd.atrybut = Anormalny;
                            blockd.dlugosc_opisu_obiektu = sizeof(c_pltype);
                            blockd.warstwa_blk = 0;
                            BLOK* boundary_container = (BLOK*)dodaj_obiekt(target_block, &blockd);
                            if (!boundary_container) continue;
                            memmove(&(boundary_container->opis_obiektu), &c_pltype, sizeof(c_pltype));
                            const int steps = 16;
                            double last_cx = x1;
                            double last_cy = y1;
                            for (int step = 1; step <= steps; ++step) {
                                double t = (double)step / (double)steps;
                                double mt = 1.0 - t;
                                double cx = (mt*mt*mt * x1) + (3.0 * mt*mt * t * x2) + (3.0 * mt * t*t * x3) + (t*t*t * x4);
                                double cy = (mt*mt*mt * y1) + (3.0 * mt*mt * t * y2) + (3.0 * mt * t*t * y3) + (t*t*t * y4);
                                json temp_seg;
                                temp_seg["op"] = "LINE";
                                temp_seg["p1"] = { last_cx, last_cy };
                                temp_seg["p2"] = { cx, cy };
                                temp_seg["layer_id"] = path.value("layer_id", 0); // Inherit using new up-to-date index layout keys

                                PTR__GTMPBLOCK = (char*)boundary_container;
                                ProcessLineSegment(boundary_container, temp_seg, path, stroke_color, raw_width, settings);
                                boundary_container = (BLOK*)PTR__GTMPBLOCK;
                                last_cx = cx;
                                last_cy = cy;
                            }
                        }
                    }
                }
            }
        }
        // Step 2: Compile Text String entities
        if (page.contains("text_objects")) {
            for (const auto& txt_obj : page["text_objects"]) {
                ProcessTextLayer(target_block, txt_obj, settings);
            }
        }
        // Step 3: Compile Raster Tiling
        if (page.contains("image_objects")) {
            for (const auto& img_obj : page["image_objects"]) {
                ProcessImageLayer(target_block, img_obj, settings);
            }
        }
        pages_processed++;
        Skala_format();
    }
    SolidHatch = 0;
    if (Master_Layers!=nullptr)
        free(Master_Layers);
    return pages_processed;
}