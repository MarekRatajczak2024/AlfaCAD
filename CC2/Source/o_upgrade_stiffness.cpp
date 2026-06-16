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
// Created by marek on 6/8/26.
//
#include <iostream>
#include <fstream>
#include <string>
//#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

#include "o_static.h"


// Structure to hold our dynamically updated element property states
struct ElementStiffness {
    double Ax, Asy, Asz, Jx, Iy, Iz;
};

#ifdef __cplusplus
extern "C" {
#endif

extern PROP_PRECISIONS *prop_precisions;
extern void set_decimal_format(char *text, double l, double precision);

void UpdateInputFileForPass2(const char *original_3dd, const char *output_3dd, const char *output_bak_3dd, int total_elements, const C_ElementStiffness *updated_elements_array)
{
    std::ifstream src(original_3dd);
    std::ofstream dst(output_3dd);
    std::string line;

    if (!src.is_open() || !dst.is_open()) return;

    bool inside_element_block = false;
    int expected_elements = 0;
    int elements_processed = 0;

    while (std::getline(src, line)) {
        // 1. Look for the frame element data header token
        if (line.find("# frame element data") != std::string::npos) {
            dst << line << "\n"; // Copy "# frame element data ..."

            // The very next line contains the number of elements (e.g., "4")
            if (std::getline(src, line)) {
                dst << line << "\n";
                std::stringstream num_ss(line);
                num_ss >> expected_elements; // Extract the integer "4"
            }

            // Copy the next 2 lines of text column units headers
            for (int h = 0; h < 2; h++) {
                if (std::getline(src, line)) {
                    dst << line << "\n";
                }
            }

            inside_element_block = true;
            elements_processed = 0;
            continue;
        }

        if (inside_element_block) {
            // Skip any remaining comment or empty lines before the numeric rows start
            if (line.empty() || line[0] == '#' || line[0] == '\r' || line[0] == '\n') {
                dst << line << "\n";
                continue;
            }

            std::stringstream ss(line);
            int elmt_id, node1, node2;

            if (ss >> elmt_id >> node1 >> node2) {
                int c_array_idx = elmt_id - 1;

                if (c_array_idx >= 0 && c_array_idx < total_elements) {
                    const C_ElementStiffness &stiff = updated_elements_array[c_array_idx];

                    // Discard the 6 old cross-sectional value columns
                    double dummy;
                    for (int k = 0; k < 6; k++) ss >> dummy;

                    // Capture everything remaining on this line safely
                    std::string trailing_parameters;
                    std::getline(ss, trailing_parameters);

                    /*  //VARIANT
                    // Helper lambda function to dynamically format cross-section properties
                    auto format_prop = [](double val) -> std::string {
                        std::stringstream ss;
                        // If the number is large and has no meaningful fractional part, format as an integer
                        if (val >= 100.0 && val == std::floor(val)) {
                            ss << std::fixed << std::setprecision(0) << val;
                        } else {
                            // Keeps up to 5 decimals for Imperial units (IMP) or fractional metric dimensions
                            ss << std::fixed << std::setprecision(5) << val;
                            // Optional: Remove trailing zeros to keep the printout tight and clean
                            std::string s = ss.str();
                            s.erase(s.find_last_not_of('0') + 1, std::string::npos);
                            if (s.back() == '.') s.pop_back();
                            return s;
                        }
                        return ss.str();
                    };

                    // Write out the modified stiffness properties safely without data loss
                    dst << " " << elmt_id
                        << " " << node1
                        << " " << node2
                        << " " << format_prop(stiff.Ax)
                        << " " << format_prop(stiff.Asy)
                        << " " << format_prop(stiff.Asz)
                        << " " << format_prop(stiff.Jx)
                        << " " << format_prop(stiff.Iz)
                        << " " << format_prop(stiff.Iy)
                        << trailing_parameters << "\n";
                     */

                     /*
                    // Write out the modified stiffness properties
                    dst << " " << elmt_id
                        << " " << node1
                        << " " << node2
                        << std::scientific << std::setprecision(5)
                        << " " << stiff.Ax
                        << " " << stiff.Asy
                        << " " << stiff.Asz
                        << " " << stiff.Jx
                        << " " << stiff.Iz
                        << " " << stiff.Iy
                        << trailing_parameters << "\n";
                        */

                    auto format_prop_A = [](double val) -> std::string {
                        char text[32];
                        set_decimal_format(text, val, prop_precisions->A_precision);
                        std::string str(text);
                        return str;
                    };

                    auto format_prop_I = [](double val) -> std::string {
                        char text[32];
                        set_decimal_format(text, val, prop_precisions->I_precision);
                        std::string str(text);
                        return str;
                    };

                    dst << " " << elmt_id
                        << " " << node1
                        << " " << node2
                        << " " << format_prop_A(stiff.Ax)
                        << " " << format_prop_A(stiff.Asy)
                        << " " << format_prop_A(stiff.Asz)
                        << " " << format_prop_I(stiff.Jx)
                        << " " << format_prop_I(stiff.Iz)
                        << " " << format_prop_I(stiff.Iy)
                        << trailing_parameters << "\n";

                } else {
                    dst << line << "\n";
                }

                elements_processed++;
                // Secure Exit Switch: Turn off parsing the exact millisecond all elements are updated
                if (elements_processed >= expected_elements) {
                    inside_element_block = false;
                }
            } else {
                dst << line << "\n";
            }
        } else {
            dst << line << "\n";
        }
    }

    src.close();
    dst.close();

    std::remove(output_bak_3dd);
    std::rename(original_3dd, output_bak_3dd);
    std::rename(output_3dd, original_3dd);
}



struct StructuralEnvelope {
    double max_Nx = 0.0;
    double max_My = 0.0;
    double max_Mz = 0.0;
};

// Compile this inside your .cpp module, exposed to C via a header
/*
void ExtractGlobalCrackingEnvelopes(char *folder, int start_file_idx, int end_file_idx, int total_elements, C_StructuralEnvelope *out_envelope_array)
    {
        std::string line;

        // Zero-out the C memory block before populating it
        for (int e = 0; e <= total_elements; ++e) {
            out_envelope_array[e].max_Nx = 0.0;
            out_envelope_array[e].max_My = 0.0;
            out_envelope_array[e].max_Mz = 0.0;
        }

        for (int file_idx = start_file_idx; file_idx <= end_file_idx; ++file_idx) {
            char filename[256];
            sprintf(filename, "%salfacad.if%02d", folder, file_idx);

            std::ifstream file(filename, std::ios::in | std::ios::binary);
            if (!file.is_open()) continue;

            int current_element_id = 0;

            while (std::getline(file, line)) {
                if (line.compare(0, 7, "#\tElmnt") == 0) {
                    std::getline(file, line);
                    std::stringstream ss(line);
                    std::string marker;
                    int n1, n2;
                    ss >> marker >> current_element_id >> n1 >> n2;
                } else if (line.compare(0, 9, "# MAXIMUM") == 0) {
                    std::stringstream ss(line);
                    std::string lbl1, lbl2;
                    double Nx, Vy, Vz, Tx, My, Mz;
                    ss >> lbl1 >> lbl2 >> Nx >> Vy >> Vz >> Tx >> My >> Mz;

                    int c_array_idx = current_element_id - 1;

                    if (c_array_idx < total_elements) {
                        // Directly modify the raw C memory array!
                        out_envelope_array[current_element_id].max_Nx = std::max(out_envelope_array[current_element_id].max_Nx, std::abs(Nx));
                        out_envelope_array[current_element_id].max_My = std::max(out_envelope_array[current_element_id].max_My, std::abs(My));
                        out_envelope_array[current_element_id].max_Mz = std::max(out_envelope_array[current_element_id].max_Mz, std::abs(Mz));
                    }
                }
            }
            file.close();
        }
    }
*/
#ifdef __cplusplus
}
#endif
