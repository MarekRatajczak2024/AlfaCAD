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

#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_MODULE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H

#include <glyph.h>
#include <glyph_structs.h>

#include "leak_detector_c.h"

#define MAX_CHARCODE 1920
extern int xMin;

extern bool firstpass;
extern bool startcontour;

int moveTo(FT_Vector* to, void* fp){return 0;}
int lineTo(FT_Vector* to, void* fp) {return 0;}
int conicTo(FT_Vector* control, FT_Vector* to, void* fp) {return 0;};
int cubicTo(FT_Vector* control1, FT_Vector* control2, FT_Vector* to, void* fp) {return 0;};

static const FT_Outline_Funcs funcs
        = {
                (FT_Outline_MoveTo_Func) moveTo,
                (FT_Outline_LineTo_Func) lineTo,
                (FT_Outline_ConicTo_Func)conicTo,
                (FT_Outline_CubicTo_Func)cubicTo,
                0, 0
        };

static int gk_face_character_outline(GLYPH_FACE* const face, const unsigned charcode, char *alf)
{
	FT_Error error;
	FT_Glyph glyph;
	int index;
	long roz_in;
	char *buffer;

	roz_in= 32000L;
	buffer = (char*)malloc(roz_in);

	if (!face || !face->face) return 1;
	if (charcode > GK_MAX_UNICODE) return 1;
	if (charcode >= 0xD800 && charcode <= 0xDFFF) return 1;

   FT_UInt glyph_index;
   glyph_index = FT_Get_Char_Index( face->face, (FT_ULong) charcode );  //ALREADY CHANGED  face  .... face->face  ////MODIFIED!!! FT_ULong*
	// load glyph
	error = FT_Load_Glyph(face->face,
		FT_Get_Char_Index(face->face, charcode),  //ALREADY CHANGED  face  .... face->face
		FT_LOAD_NO_BITMAP | FT_LOAD_NO_SCALE);
	if (error) {
		free(buffer);
		return error;
	}

	FT_Get_Glyph(face->face->glyph, &glyph);
	FT_OutlineGlyph og = (FT_OutlineGlyph)glyph;
	if (face->face->glyph->format != ft_glyph_format_outline) {
		;
	}


	// trace outline of the glyph
	xMin = 1000.0;
	firstpass = 1;
	error = FT_Outline_Decompose(&(og->outline), &funcs, buffer);  //CHANGED &funcs to funcs
	if (error)
		////std::cerr << "FT_Outline_Decompose: first pass: " << FT_StrError(error) << std::endl;
		;

	firstpass = 0;
	startcontour = 1;
	error = FT_Outline_Decompose(&(og->outline), &funcs, buffer); //CHANGED &funcs to funcs

	if (error)
		////std::cerr << "FT_Outline_Decompose: second pass: " << FT_StrError(error) << std::endl;
		;
    
	free(buffer);
	return error;

	
}
