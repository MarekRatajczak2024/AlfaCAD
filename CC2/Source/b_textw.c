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

#include<forwin.h>
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<string.h>
#include "bib_e.h"
#include "alffont.h"
#include "rysuj_e.h"
#include "o_libfun.h"
#include "bib_arc.h"
#include "b_textw.h"
#include "o_lttype.h"

#include <glyph.h>

#define MIN_TTF_VIEW 5 


double INDEX_FACTOR = 0.6;
BOOL TTF_printing = FALSE;
double INDEX_SHIFT=0.15;

int known_letters[]={8308, 8364, 8730, 8731};
int known_letters_no=sizeof(known_letters)/sizeof(known_letters[0]);

int known_bytes[][3]={{226, 129, 180},
                     {226, 130, 172},
                     {226, 136, 154},
                     {226, 136, 155},
                     };
int known_bytes_no=sizeof(known_bytes)/sizeof(known_bytes[0]);

/*-----------------------------------------------------------------------*/

extern  BITMAP *screenplay;
extern void setwritemode( int mode );
extern BOOL Get_Point_View (void);
extern void setlinestyle1(int line_style, unsigned short u_pattern, int thickness);
extern void DrawArc(long Xr, long Yr, double sta, double ea, long R, int mode);
extern void DrawArcNocut(long Xr, long Yr, double sta, double ea, long R);
extern long pikseleDX(double jednostki);
extern int TTF_bold_factor;
extern double TTF_width_factor;
extern double TTF_height_factor;

extern GrContext *second_screen;
extern GLYPH_FACE *face_ttf[128];
extern GLYPH_FACE *face_UI;

extern BOOL enforce_redraw;

extern void Draw_TTF_text(TEXT *t, char *t_text, BITMAP *bmp, int x0, int y0, double kat, float wysokosc_p, COLOR_ kolor, int mode, int t_width, int t_height);
extern void Amend_Draw_TTF_text(TEXT *t, float wysokosc_p);
extern void Init_Draw_TTF_text(TEXT *t, double kat, float wysokosc_p, COLOR_ kolor);
extern void Draw_TTF_char(TEXT *t, unsigned int unicode, BITMAP *bmp, int x0, int y0);
extern void gk_char_left_top(GLYPH_REND* const rend, const unsigned unicode, int *const left, int *const top);
extern int32_t utf8_to_ucs2(const uint8_t * input, const uint8_t ** end_ptr);
extern void Get_Face_Character_yMax(TEXT *t, int *yMax);
extern void getcolor_RGB_char(unsigned char *red, unsigned char *green, unsigned char *blue, int color);
extern int Get_TTF_Char_Outline(TEXT *t, unsigned int unicode, char *alf, long *lw, int yMax);
extern char  readmouse(void);

extern void gk_text_advance_subpixel_utf8_matrix(GLYPH_REND* const rend,const char* const text,int* const adv_x,int* const adv_y, int *matrix, int *i_matrix, unsigned *l_code);

static unsigned w_char_no=0;
static long w_lw_no=0 ;
static int w_text_no=0;

extern char *alft;

extern void setcolor(int kolor);

BOOL known3b(int letter)
{
    for (int i=0; i<known_letters_no; i++)
    {
        if (letter==known_letters[i]) return TRUE;
    }
    return FALSE;
}

BOOL known3bytes(unsigned char c1, unsigned char c2, unsigned char c3)
{
    for (int i=0; i<known_bytes_no; i++)
    {
        if ((c1==known_bytes[i][0]) &&
            (c2==known_bytes[i][1]) &&
            (c3==known_bytes[i][2])) return TRUE;
    }
    return FALSE;
}

void zero_lw_counter(void)
{
	w_char_no=0;
	w_lw_no=0;
}

unsigned int u8toMazovia(unsigned int int_code0)
{
	unsigned int int_code;
	
	switch (int_code0)
	{
	case 261: int_code = 134;  //a
		break;
	case 263: int_code = 141;  //c
		break;
	case 260: int_code = 143;  //A
		break;
	case 280: int_code = 144;  //E
		break;
	case 281: int_code = 145;  //e
		break;
	case 322: int_code = 146;  //l
		break;
	case 262: int_code = 149;  //C
		break;
	case 346: int_code = 152;  //S
		break;
	case 321: int_code = 156;  //L
		break;
	case 347: int_code = 158;  //s
		break;
	case 377: int_code = 160;  //Z'
		break;
	case 379: int_code = 161;  //Z.
		break;
	case 243: int_code = 162;  //o
		break;
	case 211: int_code = 163;  //O
		break;
	case 324: int_code = 164;  //n
		break;
	case 323: int_code = 165;  //N
		break;
	case 378: int_code = 166;  //z.
		break;
	case 380: int_code = 167;  //z'
		break;
	case 176: int_code = 248;  //deg
		break;
	case 183: int_code = 249;  //middle dot
		break;
	case 178: int_code = 253;  //square
		break;
	case 189: int_code = 171;  //1/2
		break;
	case 188: int_code = 172;  //1/4
		break;
	case 0x3B1: int_code = 0xE0; // alpha
		break;
	case 0x3B2: int_code = 0xE1; // beta
		break;
	case 0x3B3: int_code = 0xE2; // gamma
		break;
	case 0x3A0: int_code = 0xE3; // Pi
		break;
	case 0x3A3: int_code = 0xE4; // Sigma
		break;
	case 0x3C3: int_code = 0xE5; // sigma
		break;
	case 0x3BC: int_code = 0xE6; // mi
		break;
	case 0x3C4: int_code = 0xE7; // tal
		break;
	case 0xD8: int_code = 0xE8; // fi  or F8 small
		break;
	case 0x3A9: int_code = 0xEA; // Omega
		break;
	case 0x3B4: int_code = 0xEB; // delta
		break;
	case 0x3B5: int_code = 0xEE; // sigma
		break;
	case 0xB1: int_code = 0xF1; // plus minus
		break;
	case 0xB3: int_code = 0xFC; // cubic
		break;
	default:
		if (int_code0>=127) int_code=32; //returns space for unknown / not supported letters
		else int_code = int_code0;
		break;
	}

		return int_code;
}

int get_WhNumberTextStyle(void)
{
	return WhNumberTextStyle;
}

void mvcurbpT (unsigned i)      /* ini */
/*-----------------------*/
{
  int w_char_no = i ;
  int w_lw_no = i ;
}

int Get_PTRS__Text_Style_type(int czcionka)
{
	return (PTRS__Text_Style[czcionka]->type);
}

void Get_Text_Scale (TEXT *ptrs_text, double *ptrdf_sx, double *ptrdf_sy)
/*----------------------------------------------------------------------*/
{
  if (NULL == PTRS__Text_Style [ptrs_text->czcionka])
  {
    *ptrdf_sx = 1 ;
    *ptrdf_sy = 1 ;
    return ;
  }
  if (PTRS__Text_Style[ptrs_text->czcionka]->type == 2)
  {
	  *ptrdf_sy = 1.0; // ptrs_text->wysokosc;
	  *ptrdf_sx = 1.0; // *ptrdf_sy * ptrs_text->width_factor;
  }
  else
  {
	  *ptrdf_sy = ptrs_text->wysokosc *
		  PTRS__Text_Style[ptrs_text->czcionka]->font_aspect;
	  *ptrdf_sx = *ptrdf_sy * ptrs_text->width_factor;

  }
}


int Get_Char_Matix_Len_TTF(unsigned int chr, int i_font, float height)
/*------------------------------------------------------------------*/
{

	long l_len;
	GLYPH_FACE *face;
	GLYPH_REND *rend;
	float wysokosc_p;
	char ptrsz_tmp[2];
	int text_h, text_v;
	
	l_len = 0;

	if (PTRS__Text_Style[i_font]->type == 2) //TTF
	{
		face = face_ttf[i_font];
		if (!face) return 0;
		rend = gk_create_renderer(face, 0);
		if (!rend) return 0;

		wysokosc_p = pikseleDY(height);  //lets calculate for 10mm height

		gk_rend_set_size_pixels(rend, (int)((wysokosc_p*TTF_width_factor)+0.5), (int)((wysokosc_p*TTF_height_factor)+0.5));
		gk_rend_set_bold_strength(rend, 50);
		gk_rend_set_angle_in_radians(rend, 0.0); // ptrs_text->kat);

		ptrsz_tmp[0] = chr;
		ptrsz_tmp[1] = '\0';
		gk_text_size_utf8(rend, ptrsz_tmp, &text_h, &text_v);
		l_len = jednostkiX(text_h);
		gk_done_renderer(rend);
	}

	return l_len;
}

int Get_Char_Matix_Len(unsigned int chr, int i_font)
/*------------------------------------------------*/
{
  unsigned char *ptrsz_widths ;
  unsigned int i_first;
  int i_ret ;
  


  if (NULL == PTRS__Text_Style[i_font])
  {
	  return 0;
  }

  if (chr < 32)
  {
	  return 0;
  }
  
  i_first = PTRS__Text_Style [i_font]->first_char ;
  ptrsz_widths = PTRS__Text_Style [i_font]->ptrsz_widths ;
  if (((unsigned int)chr - i_first) > PTRS__Text_Style [i_font]->num_char) i_ret=0;
  else i_ret = ptrsz_widths [(unsigned int)chr - i_first] ;
  if (i_ret == 0)
  {
    i_ret = ptrsz_widths [(unsigned int )'a' - i_first] ;
  }
  return i_ret ;
}

int Get_W_Matix_Len(int i_font)
{
	return Get_Char_Matix_Len((unsigned int)'W', i_font);
}

int Get_W_Matix_Len_TTF(int i_font, int height)
{
	return Get_Char_Matix_Len_TTF((unsigned int)'W', i_font, height);
}


double Get_TTF_Char_Left_Top(TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index0, int *TTF_left, int *TTF_top)
{
	GLYPH_FACE *face;
	GLYPH_REND *rend;
	double wysokosc_p;
	double margin_char_scale0 = 1.0, margin_char_scale = 1.0;
	int font_index = 0;
	int f_type;
	int ileft, itop;
	double left;
	double df_sx, df_sy;
	
	if (TTF_left) *TTF_left = 0;
	if (TTF_top) *TTF_top = 0;

	if (NULL == PTRS__Text_Style[ptrs_text->czcionka+1])
	{
		return 0;
	}

	f_type = PTRS__Text_Style[ptrs_text->czcionka]->type;
	if (f_type != 2) return 0;

	font_index = font_index0;
	margin_char_scale = margin_char_scale0 * pow(INDEX_FACTOR, abs(font_index));

	face = face_ttf[ptrs_text->czcionka];
	if (!face) return 0.0;
	rend = gk_create_renderer(face, 0);
	if (!rend) return 0.0;

	wysokosc_p = pikseleFDY(ptrs_text->wysokosc);

	gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*ptrs_text->width_factor*TTF_width_factor*64.0) + 0.5), (int)((wysokosc_p*TTF_height_factor * 64.0) + 0.5));
	if (ptrs_text->italics) gk_rend_set_italic_angle_in_degrees(rend, 10.5);
	if (ptrs_text->bold) gk_rend_set_bold_strength(rend, TTF_bold_factor);
	gk_rend_set_angle_in_radians(rend, 0.0); // ptrs_text->kat);

    gk_char_left_top(rend, ptrsz_t[0], &ileft, &itop);

	if (TTF_left) *TTF_left = ileft;
	if (TTF_top) *TTF_top = itop;

	gk_done_renderer(rend);

	Get_Text_Scale(ptrs_text, &df_sx, &df_sy);

	left = (double)ileft*1.0; // 0.85;

	left = jednostkiX(left);
	
	return left;
}


double Get_Text_Matix_Len (TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index0, int *TTF_width, int *TTF_height, double *matrix_d, int *i_matrix_d)
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
  unsigned char *ptrsz_tmp ;
  double l_len, l_len_bar, l_len_index;
  double margin=0.0;
  double margin_char_scale0 = 1.0, margin_char_scale =1.0, margin_char_scale_index = 1.0;
  BOOL deduct = FALSE, deducted=FALSE;
  int font_index=0;
  int text_v, text_h, text_v_;
  int text_h_sum, text_h_sum_bar, text_h_sum_index;
  GLYPH_FACE *face;
  GLYPH_REND *rend;
  double wysokosc_p;
  unsigned int u8ptrsz_tmp;
  uint8_t *end_ptr;
  int f_type;
  char *ptr_n, *ptr_r;
  double factor;
  char *ptr_ind, *ptr_ind1, *ptr_ind2, *ptr_ind3;
  char char_ind='\0';

  int matrix[2048];
  int i_matrix, i_matrix_0;

  int index_pos=-1;
  double matrix_index;
  double margin_d;
  double margin_factor;
  float letter_spacing;
  int char_h, char_v, char_left, char_top, char_right;
  unsigned last_code=0;

  i_matrix=0;
  i_matrix_0=0;

  font_index = font_index0;
  margin_char_scale = margin_char_scale0 * pow(INDEX_FACTOR, abs(font_index));


  if (NULL == PTRS__Text_Style [ptrs_text->czcionka])
  {
    return 0 ;
  }

  l_len = 0 ;
  if (ptrsz_t == NULL)
  {
    ptrsz_t = ptrs_text->text ;
  }
  ptrsz_tmp = ptrsz_t;

  if (strlen(ptrsz_tmp)==0)
    {
        if (TTF_width != NULL) *TTF_width=0;
        if (TTF_height != NULL) *TTF_height=0;
        return 0.0;
    }

  f_type = PTRS__Text_Style[ptrs_text->czcionka]->type;

  if (f_type == 2) //TTF
  {
	  
	  if ((ptrsz_t == ptrs_text->text) && (ptrs_text->width > 0) && ((ptrs_text->czcionka>0) || (enforce_redraw==FALSE)) && (matrix_d==NULL))
	  {
		  if (TTF_printing == TRUE) factor = ptrs_text->wysokosc / 100.0;
		  else factor = pikseleFDY(ptrs_text->wysokosc) / 100.0;
		  if (TTF_width != NULL) *TTF_width = (int)(((double)ptrs_text->width*factor)+0.5);
		  if (TTF_height != NULL) *TTF_height = (int)(((double)ptrs_text->height*factor)+0.5);
		  if (TTF_printing == TRUE) l_len = ((double)(ptrs_text->width)*factor / 64.0);
		  else l_len = jednostkiX(((double)(ptrs_text->width))*factor/64.0);
		  return l_len;
	  }
	  
	  face = face_ttf[ptrs_text->czcionka];
	  if (!face) return 0.0;
	  rend = gk_create_renderer(face, 0);
	  if (!rend) return 0.0;

	  //normalizing height to 32 pxl
	  wysokosc_p = 100.0;

	  gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*ptrs_text->width_factor*TTF_width_factor*64.0*margin_char_scale)+0.5), (int)((wysokosc_p*TTF_height_factor*64* margin_char_scale)+0.5));
	  if (ptrs_text->italics) gk_rend_set_italic_angle_in_degrees(rend, 10.5);
	  if (ptrs_text->bold) gk_rend_set_bold_strength(rend, TTF_bold_factor);
	  gk_rend_set_angle_in_radians(rend, 0.0); // ptrs_text->kat);

	  ptr_n = strchr(ptrsz_tmp, '\n');
	  if (ptr_n != NULL) *ptr_n = '\0';
	  ptr_r = strchr(ptrsz_tmp, '\r');
	  if (ptr_r != NULL) *ptr_r = '\0';
	  
	  text_v = gk_text_height_utf8(rend, ptrsz_tmp);

	  ptr_ind1 = strchr(ptrsz_tmp, '^');
	  ptr_ind2 = strchr(ptrsz_tmp, '~');
      ptr_ind3 = NULL;
	  if (!ptr_ind1 && !ptr_ind2) {
          gk_text_advance_subpixel_utf8_matrix(rend, (char *) ptrsz_tmp, &text_h, &text_v_, matrix, &i_matrix, &last_code);
          //deduction
		  if (last_code > 0)
		  {
			  gk_char_left_top(rend, last_code, &char_left, &char_top);
			  text_h -= (char_left * 32);  //it's assumed the glyph "right" is half of its "left"
		  }
      }
	  else
	  {
		  //we cut for substrings divided by '^' or '~'
		  text_h_sum=0;
          text_h_sum_bar=0;
		  while (1)
		  {
              ptr_ind=NULL;
              if ((ptr_ind2!=NULL) && (ptr_ind1!=NULL))
              {
                  if (ptr_ind2<ptr_ind1) ptr_ind=ptr_ind2;
                  else ptr_ind=ptr_ind1;
              }
              else if (ptr_ind2!=NULL) ptr_ind=ptr_ind2;
              else if (ptr_ind1!=NULL) ptr_ind=ptr_ind1;

              if ((ptr_ind!=NULL) && (ptr_ind3!=NULL))
              {
                  if (ptr_ind3<ptr_ind) ptr_ind=ptr_ind3;
              }
              else if (ptr_ind3!=NULL)
                  ptr_ind=ptr_ind3;


              if ((char_ind == '|') && (index_pos>=0))
              {
                  //if (index_pos>=0)
                  matrix[i_matrix-1]=matrix_index;
                  text_h_sum_bar=text_h_sum;
                  //deduction
				  if (last_code > 0)
				  {
					  gk_char_left_top(rend, last_code, &char_left, &char_top);
					  text_h_sum_bar -= (char_left * 32);  //it's assumed the glyph "right" is half of its "left"
				  }

                  text_h_sum=text_h_sum_index;
              }


			  char_ind = *ptr_ind;
              *ptr_ind = '\0'; //cut string

			  gk_text_advance_subpixel_utf8_matrix(rend, ptrsz_tmp, &text_h, &text_v_, matrix, &i_matrix, &last_code);


              if (char_ind != '|') {
                  if (i_matrix > 0) {
                      index_pos = i_matrix - 1;
                      matrix_index = matrix[index_pos];
                  } else {
                      index_pos = -1;
                      matrix_index = 0.0;
                  }
              }

			  *ptr_ind = char_ind;
			  text_h_sum += text_h;
			  ptrsz_tmp = ptr_ind + 1;

			  if (char_ind == '^') {font_index += 1; index_pos=i_matrix+1; text_h_sum_index=text_h_sum;}
			  else if (char_ind == '~') {font_index -= 1; index_pos=i_matrix+1;text_h_sum_index=text_h_sum;}
			  margin_char_scale = margin_char_scale0 * pow(INDEX_FACTOR, abs(font_index));
			  gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*ptrs_text->width_factor*TTF_width_factor*64.0*margin_char_scale) + 0.5), (int)((wysokosc_p*TTF_height_factor * 64 * margin_char_scale) + 0.5));

			  ptr_ind1 = strchr(ptrsz_tmp, '^');
			  ptr_ind2 = strchr(ptrsz_tmp, '~');
              ptr_ind3 = strchr(ptrsz_tmp, '|');
			  if (!ptr_ind1 && !ptr_ind2 && !ptr_ind3)
			  {
				  gk_text_advance_subpixel_utf8_matrix(rend, ptrsz_tmp, &text_h, &text_v_, matrix, &i_matrix, &last_code);
				  text_h_sum += text_h;
				  break;
			  }
		  }

          //deduction
		  if (last_code > 0)
		  {
			  gk_char_left_top(rend, last_code, &char_left, &char_top);
			  text_h_sum -= (char_left * 32);  //it's assumed the glyph "right" is half of its "left"
		  }

          if (text_h_sum>text_h_sum_bar) text_h = text_h_sum;
          else text_h = text_h_sum_bar;
	  }

	  if (TTF_printing == TRUE)
	  {
		  factor = ptrs_text->wysokosc / 100.0;
		  l_len = (((double)(text_h))*factor) / 64.0;

          if (matrix_d!=NULL) {
              for (int i = 0; i < i_matrix; i++) {
                  matrix_d[i] = ((double) (matrix[i])) * factor / 64.0;
              }
              *i_matrix_d=i_matrix;
          }

	  }
	  else
	  {
          //recalculating matrix vextor
		  factor = pikseleFDY(ptrs_text->wysokosc) / 100.0;
		  l_len = jednostkiX(((double)(text_h))*factor / 64.0);

          if (matrix_d!=NULL) {
              for (int i = 0; i < i_matrix; i++) {
                  matrix_d[i] = jednostkiX(((double) (matrix[i])) * factor / 64.0);
              }
              *i_matrix_d=i_matrix;
          }

	  }

	  if (ptr_n != NULL) *ptr_n = '\n';
	  if (ptr_r != NULL) *ptr_r = '\r';

	  gk_done_renderer(rend);
	  
	  if (TTF_width != NULL) *TTF_width = (int)((((double)text_h)*factor)+0.5);
	  if (TTF_height != NULL) *TTF_height = (int)((((double)text_v)*factor)+0.5);

	  if (ptrsz_t == ptrs_text->text)
	  {
		  ptrs_text->width = text_h;
		  ptrs_text ->height = text_v;
	  }
	 
  }
  else if (f_type == 3) //AFT
  {

	  if ((ptrsz_t == ptrs_text->text) && (ptrs_text->width > 0) && ((ptrs_text->czcionka > 0) || (enforce_redraw == FALSE)) && (matrix_d==NULL))
	  {
		  factor = pikseleFDY(ptrs_text->wysokosc) / 100.0;
		  if (TTF_width != NULL) *TTF_width = (int)(((double)ptrs_text->width*factor) + 0.5);
		  if (TTF_height != NULL) *TTF_height = (int)(((double)ptrs_text->height*factor) + 0.5);
		  l_len = jednostkiX(((double)(ptrs_text->width))*factor / 64.0);
		  return l_len;
	  }

	  face = face_ttf[ptrs_text->czcionka + 1];
	  
	  if (!face) return 0.0;
	  rend = gk_create_renderer(face, 0);
	  if (!rend) return 0.0;

	  //normalizing height to 32 pxl
	  wysokosc_p = 100.0;

	  gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*ptrs_text->width_factor*TTF_width_factor*64.0) + 0.5), (int)((wysokosc_p*TTF_height_factor * 64) + 0.5));
	  //getcolor_RGB_char(kolor.red, kolor.gre, kolor.blu, GetColorAC(t->kolor));
	  if (ptrs_text->italics) gk_rend_set_italic_angle_in_degrees(rend, 10.5);
	  if (ptrs_text->bold) gk_rend_set_bold_strength(rend, TTF_bold_factor);
	  gk_rend_set_angle_in_radians(rend, 0.0); // ptrs_text->kat);

	  ptr_n = strchr(ptrsz_tmp, '\n');
	  if (ptr_n != NULL) *ptr_n = '\0';
	  ptr_r = strchr(ptrsz_tmp, '\r');
	  if (ptr_r != NULL) *ptr_r = '\0';

	  text_v = gk_text_height_utf8(rend, ptrsz_tmp);

      ptr_ind1 = strchr(ptrsz_tmp, '^');
      ptr_ind2 = strchr(ptrsz_tmp, '~');
      ptr_ind3 = NULL;

      if (!ptr_ind1 && !ptr_ind2)
      {
          gk_text_advance_subpixel_utf8_matrix(rend, (char*)ptrsz_tmp, &text_h, &text_v_, matrix, &i_matrix, &last_code);
          //deduction
		  if (last_code > 0)
		  {
			  gk_char_left_top(rend, last_code, &char_left, &char_top);
			  text_h -= (char_left * 32);  //it's assumed the glyph "right" is half of its "left"
		  }
      }
      else
      {
          //we cut for substrings divided by '^' or '~'
          text_h_sum=0;
          text_h_sum_bar=0;
          while (1)
          {
              ptr_ind=NULL;
              if ((ptr_ind2!=NULL) && (ptr_ind1!=NULL))
              {
                  if (ptr_ind2<ptr_ind1) ptr_ind=ptr_ind2;
                  else ptr_ind=ptr_ind1;
              }
              else if (ptr_ind2!=NULL) ptr_ind=ptr_ind2;
              else if (ptr_ind1!=NULL) ptr_ind=ptr_ind1;

              if ((ptr_ind!=NULL) && (ptr_ind3!=NULL))
              {
                  if (ptr_ind3<ptr_ind) ptr_ind=ptr_ind3;
              }
              else if (ptr_ind3!=NULL)
                  ptr_ind=ptr_ind3;

              if ((char_ind == '|') && (index_pos>=0))
              {
                  //if (index_pos>=0)
                  matrix[i_matrix-1]=matrix_index;
                  text_h_sum_bar=text_h_sum;
                  //deduction
				  if (last_code > 0)
				  {
					  gk_char_left_top(rend, last_code, &char_left, &char_top);
					  text_h_sum_bar -= (char_left * 32);  //it's assumed the glyph "right" is half of its "left"
				  }

                  text_h_sum=text_h_sum_index;
              }

              char_ind = *ptr_ind;
              *ptr_ind = '\0'; //cut string

              gk_text_advance_subpixel_utf8_matrix(rend, ptrsz_tmp, &text_h, &text_v_, matrix, &i_matrix, &last_code);


              if (char_ind != '|') {
                  if (i_matrix > 0) {
                      index_pos = i_matrix - 1;
                      matrix_index = matrix[index_pos];
                  } else {
                      index_pos = -1;
                      matrix_index = 0.0;
                  }
              }

              *ptr_ind = char_ind;
              text_h_sum += text_h;
              ptrsz_tmp = ptr_ind + 1;

              if (char_ind == '^') {font_index += 1; index_pos=i_matrix+1;text_h_sum_index=text_h_sum;}
              else if (char_ind == '~') {font_index -= 1; index_pos=i_matrix+1;text_h_sum_index=text_h_sum;}
              margin_char_scale = margin_char_scale0 * pow(INDEX_FACTOR, abs(font_index));
              gk_rend_set_size_subpixel(rend, (int)((wysokosc_p*ptrs_text->width_factor*TTF_width_factor*64.0*margin_char_scale) + 0.5), (int)((wysokosc_p*TTF_height_factor * 64 * margin_char_scale) + 0.5));

              ptr_ind1 = strchr(ptrsz_tmp, '^');
              ptr_ind2 = strchr(ptrsz_tmp, '~');
              ptr_ind3 = strchr(ptrsz_tmp, '|');
              if (!ptr_ind1 && !ptr_ind2 && !ptr_ind3)
              {
                  gk_text_advance_subpixel_utf8_matrix(rend, ptrsz_tmp, &text_h, &text_v_, matrix, &i_matrix, &last_code);
                  text_h_sum += text_h;
                  break;
              }
          }

          //deduction
		  if (last_code > 0)
		  {
			  gk_char_left_top(rend, last_code, &char_left, &char_top);
			  text_h_sum -= (char_left * 32);  //it's assumed the glyph "right" is half of its "left"
		  }

          if (text_h_sum>text_h_sum_bar) text_h = text_h_sum;
          else text_h = text_h_sum_bar;
      }

      if (TTF_printing == TRUE)
      {
          factor = ptrs_text->wysokosc / 100.0;
          l_len = ((double)(text_h))*factor / 64.0;

          if (matrix_d!=NULL) {
              for (int i = 0; i < i_matrix; i++) {
                  matrix_d[i] = ((double) (matrix[i])) * factor / 64.0;
              }
              *i_matrix_d=i_matrix;
          }

      }
      else
      {
          //recalculating matrix vextor
          factor = pikseleFDY(ptrs_text->wysokosc) / 100.0;
          l_len = jednostkiX(((double)(text_h))*factor / 64.0);

          if (matrix_d!=NULL) {
              for (int i = 0; i < i_matrix; i++) {
                  matrix_d[i] = jednostkiX(((double) (matrix[i])) * factor / 64.0);
              }
              *i_matrix_d=i_matrix;
          }

      }

	  if (ptr_n != NULL) *ptr_n = '\n';
	  if (ptr_r != NULL) *ptr_r = '\r';

	  gk_done_renderer(rend);

	  if (TTF_width != NULL) *TTF_width = (int)((((double)text_h)*factor) + 0.5);
	  if (TTF_height != NULL) *TTF_height = (int)((((double)text_v)*factor) + 0.5);

	  if (ptrsz_t == ptrs_text->text)
	  {
		  ptrs_text->width = text_h;
		  ptrs_text->height = text_v;
	  }
  }
  else
  {
      if (PTRS__Text_Style[ptrs_text->czcionka]->type==1)
          letter_spacing = PTRS__Text_Style[ptrs_text->czcionka]->letter_spacing;
      else letter_spacing=0.0;

      if (PTRS__Text_Style[ptrs_text->czcionka]->type==0) margin_factor=0.5;
      else margin_factor=1.0;

	  if ((ptrsz_t == ptrs_text->text) && (ptrs_text->width > 0) && (enforce_redraw==FALSE) && (matrix_d==NULL))
	  {
		  l_len = ((double)(ptrs_text->width)) / 100.0;
	  }
	  else
	  {
          l_len_bar=0;
		  while ((*ptrsz_tmp != '\0') && (*ptrsz_tmp != '\n'))
		  {
			  if (*ptrsz_tmp == '\r')
			  {
				  ptrsz_tmp += 1;
				  continue;
			  }

			  if ((*ptrsz_tmp == '\45') &&
				  (*(ptrsz_tmp + 1) == '\45') &&
				  ((*(ptrsz_tmp + 2) == '\125') || (*(ptrsz_tmp + 2) == '\165')))
			  {
				  ptrsz_tmp += 3;
			  }

              if (*ptrsz_tmp == '|')
              {
                  if (index_pos>=0) {
                      if ((matrix_d!=NULL) && (i_matrix>0)) matrix_d[i_matrix-1]=matrix_index;
                      ptrsz_tmp++;
                      l_len_bar=l_len;
                      l_len_bar-=(PTRS__Text_Style[ptrs_text->czcionka]->letter_spacing*margin_char_scale_index* ptrs_text->width_factor);
                      l_len=l_len_index;
                      continue;
                  }
              }
			  if (*ptrsz_tmp == '\176') //~
			  {
				  ptrsz_tmp++;
				  if (*ptrsz_tmp == '\40') ptrsz_tmp++;
				  margin = (PTRS__Text_Style[ptrs_text->czcionka]->ptrsz_widths['|'] + PTRS__Text_Style[ptrs_text->czcionka]->margin_char) * margin_factor *margin_char_scale;  //0.5 or 1.0
                  font_index -= 1;
                  margin_char_scale = margin_char_scale0 * pow(INDEX_FACTOR, abs(font_index));
				  deduct = TRUE;
                  index_pos = i_matrix - 1;
                  if ( matrix_d!=NULL) {
                      matrix_index = matrix_d[index_pos];
                  }
                  l_len_index=l_len;
                  margin_char_scale_index=margin_char_scale;
			  }
			  else if (*ptrsz_tmp == '\136')  //^
			  {
				  ptrsz_tmp++;
				  if (*ptrsz_tmp == '\40') ptrsz_tmp++;
				  margin = (PTRS__Text_Style[ptrs_text->czcionka]->ptrsz_widths['|'] + PTRS__Text_Style[ptrs_text->czcionka]->margin_char) *margin_factor *margin_char_scale;  //0.5 or 1.0
				  font_index += 1;
				  margin_char_scale = margin_char_scale0 * pow(INDEX_FACTOR, abs(font_index));
                  deduct = TRUE;
                  index_pos = i_matrix - 1;
                  if ( matrix_d!=NULL) {
                      matrix_index = matrix_d[index_pos];
                  }
                  l_len_index=l_len;
                  margin_char_scale_index=margin_char_scale;
			  }
			  else
			  {
				  if (*ptrsz_tmp >= 127)
				  {
					  u8ptrsz_tmp = utf8_to_ucs2(ptrsz_tmp, (const uint8_t **) &end_ptr);

                      //if (u8ptrsz_tmp == 8308) //⁴ e.g. m⁴
                      if (u8ptrsz_tmp > 1920)
                      {
                          if (known3b(u8ptrsz_tmp)) //⁴ e.g. m⁴
                              ptrsz_tmp+=2;
                          else
                          {
                              u8ptrsz_tmp = 32;
                              ptrsz_tmp++;
                          }
                      }
                      else
                      {
                          if (f_type == 0) {
                              //conver to Mazovia
                              u8ptrsz_tmp = u8toMazovia(u8ptrsz_tmp);
                          }
                          ptrsz_tmp++;
                      }
				  }
				  else
					  u8ptrsz_tmp = *ptrsz_tmp;

				  if (deduct)
				  {
					  if ((margin_char_scale > 0.0) && (l_len > 0))
                      {
                          margin_d=(margin*(1.0 - margin_char_scale))* ptrs_text->width_factor;
                          l_len -= ((int)(margin_d));
                          if (matrix_d!=NULL) matrix_d[i_matrix-1] -=margin_d;
                      }
                      l_len_index=l_len;
					  deduct = FALSE;
					  margin = 0.0;
				  }

                  if (matrix_d!=NULL)
                  {
                     if (i_matrix>0) l_len = matrix_d[i_matrix-1];
                  }
				  l_len += (((Get_Char_Matix_Len(u8ptrsz_tmp, ptrs_text->czcionka) + PTRS__Text_Style[ptrs_text->czcionka]->margin_char)*margin_char_scale) * ptrs_text->width_factor);

                  if (matrix_d!=NULL)  matrix_d[i_matrix]=l_len;
                  i_matrix++;

				  ptrsz_tmp++;
			  }
		  }

          if (matrix_d!=NULL) *i_matrix_d = i_matrix;

		  l_len -= (letter_spacing * margin_char_scale * ptrs_text->width_factor);

		  if (ptrsz_t == ptrs_text->text)
		  {
              if (l_len_bar>l_len)
              {
                  ptrs_text->width = (int)(l_len_bar*100.0);
                  l_len=l_len_bar;
              }
              else ptrs_text->width = (int)(l_len*100.0);
			  ptrs_text->height = (int)(ptrs_text->wysokosc*100);
		  }
	  }
  }
  return l_len ;
}


double Get_Text_Len (TEXT *ptrs_text, char *ptrsz_t, double font_scale, int font_index, int *t_width, int *t_height, double *matrix_d, int *i_matrix_d)
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
{
  double df_len;
  double df_sx, df_sy ;

  df_len = Get_Text_Matix_Len (ptrs_text, ptrsz_t, font_scale, font_index, t_width, t_height, matrix_d, i_matrix_d) ;
  Get_Text_Scale (ptrs_text, &df_sx, &df_sy) ;
  df_len *= df_sy;

  if (matrix_d!=NULL) {
     for (int i=0; i<*i_matrix_d; i++) {
         matrix_d[i] *= df_sy;
     }
  }

  if (PTRS__Text_Style[ptrs_text->czcionka]->type == 3)
  {
	  df_len *= (40.6 / ptrs_text->wysokosc);

      for (int i=0; i<*i_matrix_d; i++) {
          matrix_d[i] *= (40.6 / ptrs_text->wysokosc);
      }
  }
  return df_len ;
}


void GetTextLen (TEXT *ptrs_text, double *tdl)
{ double tdl1;
 tdl1=Get_Text_Len (ptrs_text, ptrs_text->text, 1.0, 0, NULL, NULL, NULL, NULL); //TEMPORARY
 *tdl=tdl1;
}


void GetCharLeft(TEXT *ptrs_text, double *tdl)
{
	int f_type;
f_type = PTRS__Text_Style[ptrs_text->czcionka]->type;
if (f_type < 2) *tdl = 0.0;
else *tdl = Get_TTF_Char_Left_Top(ptrs_text, ptrs_text->text, 1.0, 0, NULL, NULL);
}
double Get_TextLen (TEXT *ptrs_text, char *ptrsz_t)
/*-----------------------------------------------*/
{
  double df_len ;
  double df_sx, df_sy ;
  double aspect;

  df_len=Get_Text_Matix_Len (ptrs_text, ptrsz_t, 1.0, 0, NULL, NULL, NULL, NULL) ;
  Get_Text_Scale (ptrs_text, &df_sx, &df_sy) ;
  df_len *= df_sy;

  if (PTRS__Text_Style[ptrs_text->czcionka]->type == 3)
  {
	  aspect = PTRS__Text_Style[ptrs_text->czcionka]->font_aspect;
	  if (aspect > 0) df_len /= (aspect*10.0);
  }

  return df_len ;
}

void Clear_Fonts_Flags (BOOL b_buf, BOOL b_buf_font)
/*------------------------------------------------*/
{
  int i ;

  for (i = 0 ; i < MaxNumberTextStyle ; i++)
  {
    if (NULL != PTRS__Text_Style [i])
    {
      if (b_buf == TRUE)
      {
	PTRS__Text_Style [i]->place_buf = 0 ;
      }
      if (b_buf_font == TRUE)
      {
	PTRS__Text_Style [i]->place_buf_font = 0 ;
      }
    }
  }
}

void arcSER(float pl_xs, float pl_ys, float pl_xe, float pl_ye, double r, float *l_x0, float *l_y0, double *l_kat1, double *l_kat2)
{
	double x0, y0, /*x, y,*/ xr, yr, xe, ye, xs, ys, dl;
	double a1, a2, si, co;
	float l_x, l_y;
	//double l_r;

	x0 = (pl_xe + pl_xs) / 2;
	y0 = (pl_ye + pl_ys) / 2;
	xs = pl_xs - x0;
	ys = pl_ys - y0;
	dl = sqrt(xs*xs + ys * ys);
	si = ys / dl;
	co = xs / dl;
	obru(si, co, xs, ys, &xs, &ys);
	xr = 0;
	yr = sqrt(fabs(r*r - xs * xs));  /* zawsze r >= xs, dla  r == xs moze byc float, dlatego fabs*/
	if (r > 0) yr = -yr;
	obrd(si, co, xr, yr, &xr, &yr);
	l_x = xr + x0;
	l_y = yr + y0;
	xs = pl_xs - l_x;  xe = pl_xe - l_x;
	ys = pl_ys - l_y;  ye = pl_ye - l_y;
	a1 = Atan2(ys, xs);
	a2 = Atan2(ye, xe);
	*l_x0 = l_x;
	*l_y0 = l_y;
	if (a1 < 0.0) a1 = 6.283185 + a1;
	if (a1 < 0.0) a2 = 6.283185 + a2;
	*l_kat1 = a1;
	*l_kat2 = a2;
}

void arcSEA(float pl_xs, float pl_ys, float pl_xe, float pl_ye, double angle, double *l_r, double *l_x0, double *l_y0, double *l_kat1, double *l_kat2)
{
	double x0, y0, xr, yr, xe, ye, xs, ys, dl, t;
	double a1, a2, si, co;
	int znak_kata = 0;
	double l_x, l_y, r, dkat;

	dkat = (double)angle;
	x0 = (pl_xe + pl_xs) / 2;
	y0 = (pl_ye + pl_ys) / 2;
	xs = pl_xs - x0; ys = pl_ys - y0;
	dl = sqrt(xs*xs + ys * ys);
	si = ys / dl;
	co = xs / dl;
	obru(si, co, xs, ys, &xs, &ys);
	xr = 0;
	t = tan(dkat / 2);
	yr = -xs / t;
	r = sqrt(xs*xs + yr * yr);
	obrd(si, co, xr, yr, &xr, &yr);
	l_x = xr + x0; 
	l_y = yr + y0;
	xs = pl_xs - l_x;  xe = pl_xe - l_x;
	ys = pl_ys - l_y;  ye = pl_ye - l_y;
	a1 = Atan2(ys, xs);
	a2 = Atan2(ye, xe);
	
	if (angle > 0)
	{
		*l_kat1 = a1;
		*l_kat2 = a2;
	}
	else
	{
		*l_kat1 = a2;
		*l_kat2 = a1;
	}
	

	*l_x0 = l_x;
	*l_y0 = l_y;

	*l_r = r;
}

void outtextxy_w_menu(TEXT *t)
{
	unsigned char *zn;
	long lw = 0;
	int f_type, i_num;
	long ad;
	long li;
	T_Font_Off * ptrs_off;
	unsigned char i_first;
	char *ptr;
	char *alf;
	int font_index = 0;
	double xind = 0, yind = 0;
	double df_scale_x0, df_scale_y0, WysokoscT;
	double font_scale = 1.0;
	int  k = 0;
	BOOL arc;
	float x1_f, y1_f, x2_f, y2_f;
	float angle_f;
	double x1, y1, x2, y2;
	double x01, y01, x001, y001; // x01_, y01_, x01__, y01__, x01___, y01___;
	double df_scale_x = 1.0;
	double df_scale_y = 1.0;
	double r, angle;
	double tsin, tcos;
	double l_x0, l_y0;
	double l_kat1, l_kat2;
	long xr, yr, lr;
	double df_tl, df_tl0; // df_tl1, df_tl2;
	unsigned int u8zn, Mzn;
	uint8_t *end_ptr;

	zn = (unsigned char *)t->text;

	tsin = sin(t->kat);
	tcos = cos(t->kat);

	df_tl0 = Get_Text_Len(t, (char*)zn, font_scale, font_index, NULL, NULL, NULL, NULL);
	df_tl = 0;

	Get_Text_Scale(t, &df_scale_x0, &df_scale_y0);
	df_scale_x = df_scale_x0;
	df_scale_y = df_scale_y0;
	WysokoscT = t->wysokosc;

	setwritemode(COPY_PUT);

	x001 = t->x;
	y001 = t->y;
	x01 = t->x;
	y01 = t->y;


	ptrs_off = PTRS__Text_Style[t->czcionka]->ptrs_off;

	alf = PTRS__Text_Style[t->czcionka]->ptrsz_font;

	i_first = PTRS__Text_Style[t->czcionka]->first_char;
	i_num = PTRS__Text_Style[t->czcionka]->num_char;
	f_type = PTRS__Text_Style[t->czcionka]->type;
	

	while (*zn != '\0')
	{
		lw = 0;
		if (*zn >= 127)
		{
			//convert to UNICODE and shift index
			u8zn = utf8_to_ucs2(zn, (const uint8_t **) &end_ptr);

            //if (u8zn == 8308) //⁴ e.g. m⁴
            if (u8zn > 1920)
            {
                if (known3b(u8zn)) zn+=2;
                else
                {
                    u8zn = 32;
                    zn++;
                }
            }
            else zn++;

		}
		else u8zn = (unsigned int)zn[0];

			if (f_type == 0)
			{
				//conver to Mazovia
				Mzn = u8toMazovia(u8zn);
				ad = ptrs_off[Mzn - i_first].i_offset;
			}
			else
			{
				//ad = ptrs_off[*zn].i_offset;
				ad = ptrs_off[u8zn].i_offset;
				ptr = alf + ad;
			}

			if ((ad != OFFEMPTY) && (u8zn <= (unsigned int)(i_first + i_num)))
			{
				if (f_type == 0)
				{
					//conver to Mazovia
					Mzn = u8toMazovia(u8zn);
					lw = ptrs_off[Mzn - i_first].vectorsno;
				}
				else
					lw = ptrs_off[u8zn].vectorsno;

				if (font_index > 0) yind = WysokoscT * (1.0 - font_scale);
				else yind = 0.0;

				for (li = 0; li < lw; li++)
				{
					if (mvcurb.akton && mvcurb.mvc)
					{
						w_char_no = k;
						w_lw_no = li;
						return;
					}
					if (mvcurb.aktoff && mvcurb.T == 1 &&
						w_char_no == k && w_lw_no == li)
					{
						return;
					}
					arc = FALSE;
					if ((f_type == 1) || (f_type == 3))
					{
						memmove(&x1_f, ptr, sizeof(float));
						ptr += sizeof(float);
						if (x1_f > 998)  //it's ARC
						{
							memmove(&angle_f, ptr, sizeof(float));
							ptr += sizeof(float);
							memmove(&x1_f, ptr, sizeof(float));
							ptr += sizeof(float);
							arc = TRUE;
						}
						memmove(&y1_f, ptr, sizeof(float));
						ptr += sizeof(float);
						memmove(&x2_f, ptr, sizeof(float));
						ptr += sizeof(float);
						memmove(&y2_f, ptr, sizeof(float));
						ptr += sizeof(float);
						x1 = x1_f * df_scale_x *4.5;
						y1 = y1_f * df_scale_y *4.5;
						x2 = x2_f * df_scale_x *4.5;
						y2 = y2_f * df_scale_y *4.5;
					}
					else
					{
						x1 = alf[ad++] * df_scale_x;
						y1 = alf[ad++] * df_scale_y;
						x2 = alf[ad++] * df_scale_x;
						y2 = alf[ad++] * df_scale_y;
					}

					y1 += yind;
					y2 += yind;

					if (arc == FALSE)
					{
						lineC(x01 + x1, y01 - y1, x01 + x2, y01 - y2);
					}
					else
					{
						angle = angle_f * 3.141592;
						arcSEA(x1, y1, x2, y2, angle, &r, &l_x0, &l_y0, &l_kat1, &l_kat2);

						xr = x01 + l_x0;
						yr = y01 + l_y0;
						lr = r;

						DrawArcNocut(xr, yr, l_kat1, l_kat2, lr);
					}

				}
			}

			zn++;

			df_tl = df_tl0 - Get_Text_Len(t, (char*)zn, font_scale, font_index, NULL, NULL, NULL, NULL);

			
			x01 = x001 + df_tl * tcos;
			y01 = y001 - Ky * df_tl * tsin;
				
	
			k++;
		
	}
}

void outlinetext(TEXT *t, QUAD *outline, double margin)
{
    unsigned char *zn;
    char *ttf_zn;
    int f_type;
    double df_width_marg, WysokoscT, tdl, tsin, tcos, df_scale_x, df_scale_y, df_scale_x0, df_scale_y0;
    float wysokosc_p;
    double x01, y01, ttf_x01, ttf_y01, x001, y001, x01__, y01__, x01___, y01___;
    double font_scale = 1.0;
    int font_index = 0;
    int t_height, t_width;
    BOOL b_hor;
    double xt1, yt1;
    double thick_temp1, thick_temp2;

    double front_space=t->wysokosc*0.1;

    zn = (unsigned char *)t->text;
    ttf_zn = zn;

    f_type = PTRS__Text_Style[t->czcionka]->type;
    if (f_type == 2)
    {
        df_scale_y0 = t->wysokosc;
        df_scale_x0 = df_scale_y0 * t->width_factor;
    }
    else Get_Text_Scale(t, &df_scale_x0, &df_scale_y0);
    df_scale_x = df_scale_x0;
    df_scale_y = df_scale_y0;
    WysokoscT = t->wysokosc;
    wysokosc_p = pikseleFDY(WysokoscT);

    x001 = t->x;
    y001 = t->y;
    x01 = t->x;
    y01 = t->y;
    ttf_x01 = x01;
    ttf_y01 = y01;

    tdl = Get_Text_Len(t, t->text, font_scale, font_index, &t_width, &t_height, NULL, NULL);
    tsin = sin(t->kat);
    tcos = cos(t->kat);

    b_hor = FALSE;
    if (TRUE == Check_if_Equal(t->kat, 0))
    {
        b_hor = TRUE;
    }

    switch (t->justowanie)
    {
        case 1: xt1 = x01 - tdl * tcos;  //do prawej
            yt1 = y01 + Ky * tdl * tsin;
            x01 = xt1;
            y01 = yt1;
            break;
        case 2: xt1 = x01 - (tdl / 2) * tcos;  //srodkowo
            yt1 = y01 + (Ky * tdl) / 2 * tsin;
            x01 = xt1;
            y01 = yt1;
            break;
        case 3: xt1 = x01 - (tdl / 2) * tcos + (WysokoscT / 2) * tsin;  //centralnie
            yt1 = y01 + (Ky * tdl) / 2 * tsin - (WysokoscT / 2) * tcos;
            x01 = xt1;
            y01 = yt1;
            break;
    }

    if (b_hor == TRUE)
    {
        thick_temp2 = Ky * WysokoscT;

        outline->xy[0]=x01-front_space-margin;
        outline->xy[2]=x01+tdl+margin-front_space;
        outline->xy[4]=outline->xy[2];
        outline->xy[6]=outline->xy[0];

        outline->xy[1]=y01-margin;
        outline->xy[3]=outline->xy[1];
        outline->xy[5]=y01 - thick_temp2 + margin;
        outline->xy[7]=outline->xy[5];
    }
    else
    {
        thick_temp1 = (WysokoscT + 2*margin)* tsin;
        thick_temp2 = Ky * (WysokoscT +2*margin)* tcos;

        outline->xy[0]=x01 + ((-front_space - margin)) * tcos - (-margin * tsin);
        outline->xy[2]=x01 + ((tdl + margin - front_space) * tcos) - (-margin * tsin);
        outline->xy[4]=outline->xy[2] - thick_temp1;
        outline->xy[6]=outline->xy[0] - thick_temp1;

        outline->xy[1]=y01 + Ky * ((margin * tcos) + (front_space + margin)* tsin);
        outline->xy[3]=y01 - Ky * (-(margin * tcos) + (tdl + margin - front_space) * tsin);
        outline->xy[5]=outline->xy[3] - thick_temp2;
        outline->xy[7]=outline->xy[1] - thick_temp2;
    }
}

void outtextxy_w_(TEXT *t0, int mode)
/*----------------------------------*/
{
	TEXT t1;
	TEXT *t;
	float wysokosc_p;
	long li;
	int ad;
	double x1, y1, x2, y2;
	double x01, y01, ttf_x01, ttf_y01, x001, y001, /*x01_, y01_,*/ x01__, y01__, x01___, y01___;
	double xu01, yu01, xu01_, yu01_;
	double xt1, yt1;
	int  k = 0;
	unsigned int i_first, i_num;
	unsigned char *zn, *zn0;
	uint32_t u8zn;
	char *ptr_zn;
#ifndef LINUX
	long lw = 0;
#else
    int lw = 0;
#endif
	double df_width_marg, WysokoscT, tdl, tsin, tcos, df_scale_x, df_scale_y, df_scale_x0, df_scale_y0;
	double thick_temp1, thick_temp2;
	double tsin_it, df_tl, df_tl_, df_tl__, df_tl0, df_tl1, df_tl2;
	char *alf;
	BOOL b_hor;
	T_Font_Off * ptrs_off;
	BOOL underlining;
	int f_type;
	float x1_f, y1_f, x2_f, y2_f, angle_f;
	BOOL arc;
	char *ptr;
	BOOL out_break;
	double dy = 0.0;
	long xr, yr, lr;
	double r, angle;
	double l_x0, l_y0;
	double l_kat1, l_kat2;
	double ddf_tl;
	double font_scale = 1.0;
	double xind = 0, yind = 0;
	int font_index = 0;
	uint8_t *end_ptr;
	char *ttf_zn;
	int text_no = 0;
	int t_height, t_width;
    int t_height_next, t_width_next;
	unsigned char red, green, blue;
	COLOR_ kolor;
	int TTF_left, TTF_top;
	int yMax;
	char *ptr_ind, *ptr_ind1, *ptr_ind2;

	char keym;
    int t0_kolor;

    double matrix[2048];
    int i_matrix=0;
    int i_zn=0;
    BOOL index_pos=FALSE;

    int a;

    long x_char, y_char;

    if (mode==XOR_PUT) t0_kolor= kolory.blok;
	else if (mode==NOT_PUT) t0_kolor= kolory.paper;
    else t0_kolor=GetColorAC(t0->kolor);

    mode = COPY_PUT;  //enforsed on NON-XOR version

	if ((t0->ukryty == 1) && (type_view == FALSE))
		return;

	underlining = FALSE;

	ptr_ind1 = strchr(t0->text, '^');
	ptr_ind2 = strchr(t0->text, '~');

    ptr_ind=NULL;
    if ((ptr_ind2!=NULL) && (ptr_ind1!=NULL))
    {
        if (ptr_ind2<ptr_ind1) ptr_ind=ptr_ind2;
        else ptr_ind=ptr_ind1;
    }
    else if (ptr_ind2!=NULL) ptr_ind=ptr_ind2;
    else if (ptr_ind1!=NULL) ptr_ind=ptr_ind1;


	switch (t0->justowanie)
	{
	case 0:
		Underl_del = 0.25*(t0->wysokosc);
		break;
	case 1:
		Underl_del = 0.25*(t0->wysokosc);
		break;
	case 2:
		Underl_del = 0.25*(t0->wysokosc);
		break;
	case 3:
		Underl_del = 0.75*(t0->wysokosc);
		break;
	}


	if (t0->italics == 1)
	{
		tsin_it = sin(ITALICS_ANGLE * Pi / 180);
	}
	if (t0->czcionka >= (unsigned int)WhNumberTextStyle)
	{
		return;	/*brak czcionki*/
	}
	if (NULL == PTRS__Text_Style[t0->czcionka])
	{
		return;
	}

	f_type = PTRS__Text_Style[t0->czcionka]->type;
	if ((f_type == 2) && (mode == 1))
	{
		df_scale_y0 = t0->wysokosc; // *PTRS__Text_Style[t0->czcionka]->font_aspect;
		df_scale_x0 = df_scale_y0 * t0->width_factor;
	}
	else Get_Text_Scale(t0, &df_scale_x0, &df_scale_y0);
	df_scale_x = df_scale_x0;
	df_scale_y = df_scale_y0;
	WysokoscT = t0->wysokosc;
	wysokosc_p = pikseleFDY(WysokoscT);

	if (f_type < 2)
	{
		df_width_marg = Get_Char_Matix_Len((unsigned int)'W', t0->czcionka) * df_scale_x;
	}
	else
	{
		df_width_marg = Get_Char_Matix_Len_TTF((unsigned int)'W', t0->czcionka, t0->wysokosc); // *df_scale_x;
	}

	setwritemode(mode);

	x001 = t0->x;
	y001 = t0->y;
	x01 = t0->x;
	y01 = t0->y;
	ttf_x01 = x01;
	ttf_y01 = y01;

	if (TRUE == Get_Point_View())
	{

		if ((strlen(t0->text) == 0) && (t0->typ > 2))
		{
			memmove(&t1, t0, sizeof(TEXT));  //TEXT_NAG));
			t1.text[0] = 255;
			t1.text[1] = '\0';

			t = &t1;
		}
		else t = t0;
	}
	else t = t0;

	//  setwritemode (mode) ;    
	tdl = Get_Text_Len(t, t->text, font_scale, font_index, &t_width, &t_height, matrix, &i_matrix);
	tsin = sin(t->kat);
	tcos = cos(t->kat);

	if (t->justowanie > 0)
	{
		if (FALSE == Check_if_Equal(tdl, 0))
		{

		}
	}

	b_hor = FALSE;
	if (TRUE == Check_if_Equal(t->kat, 0))
	{
		b_hor = TRUE;
	}

	if (wysokosc_p <= 3.0)
	{

		switch (t->justowanie)
		{
		case 1: xt1 = x01 - tdl * tcos;  //do prawej
			yt1 = y01 + Ky * tdl * tsin;
			x01 = xt1;
			y01 = yt1;

			break;
		case 2: xt1 = x01 - (tdl / 2) * tcos;  //srodkowo
			yt1 = y01 + (Ky * tdl) / 2 * tsin;
			x01 = xt1;
			y01 = yt1;

			break;
		case 3: xt1 = x01 - (tdl / 2) * tcos + (WysokoscT / 2) * tsin;  //centralnie
			yt1 = y01 + (Ky * tdl) / 2 * tsin - (WysokoscT / 2) * tcos;
			x01 = xt1;
			y01 = yt1;

			break;
		}

		zn = (unsigned char *)t->text;
		out_break = FALSE;
		while ((zn != '\0') && (!out_break))
		{
			if (wysokosc_p <= 1.0)
			{
				if ((t->ukryty == 0) && (t->typ != n_typ_atrybut))
				{
					if (t->bold == 0) setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
					else setlinestyle1(SOLID_LINE, 0, 3/*NORM_WIDTH*/);
				}
				else
				{
					//setlinestyle (SOLID_LINE, 0, 1) ;
					setlinestyle1(USERBIT_LINE, 1, 1);
				}
				if (b_hor == TRUE)
				{
					lineC(pikseleX0(x01), pikseleY0(y01), pikseleX0(x01 + tdl),
						pikseleY0(y01));
				}
				else
				{
					lineC(pikseleX0(x01), pikseleY0(y01), pikseleX0(x01 + tdl * tcos),
						pikseleY0(y01 + tdl * tsin));
				}
			}
			else
			{
				if (b_hor == TRUE)
				{
					thick_temp2 = Ky * 0.5 * WysokoscT;
					if ((t->ukryty == 0) && (t->typ != n_typ_atrybut)) setlinestyle1(SOLID_LINE, 0, THICK_WIDTH);
					else
					{
						//setlinestyle (SOLID_LINE, 0, 1) ;
						setlinestyle1(USERBIT_LINE, 1, 3);
					}

					lineC(pikseleX0(x01),
						pikseleY0(y01 - thick_temp2),
						pikseleX0(x01 + tdl),
						pikseleY0(y01 - thick_temp2));
				}
				else
				{
					thick_temp1 = 0.5 * WysokoscT * tsin;
					thick_temp2 = Ky * 0.5 * WysokoscT * tcos;
					if ((t->ukryty == 0) && (t->typ != n_typ_atrybut)) setlinestyle1(SOLID_LINE, 0, THICK_WIDTH);
					else
					{
						//setlinestyle (SOLID_LINE, 0, 1) ;
						setlinestyle1(USERBIT_LINE, 1, 3);
					}
					lineC(pikseleX0(x01 - thick_temp1),
						pikseleY0(y01 - thick_temp2),
						pikseleX0(x01 + tdl * tcos - thick_temp1),
						pikseleY0(y01 - Ky * tdl * tsin - thick_temp2));
				}
			}
			//next line if exists
			ptr_zn = strchr(zn, '\n');
			if (ptr_zn != NULL)
			{
				zn = ptr_zn + 1;

                i_matrix=0;
                i_zn=0;
				tdl = Get_Text_Len(t, (char*)zn, font_scale, font_index, &t_width, &t_height, matrix, &i_matrix);

				x001 += WysokoscT * ((float)(t0->spacing) / 10.0 + 1.0) *tsin; //SPACING
				y001 -= WysokoscT * ((float)(t0->spacing) / 10.0 + 1.0) *tcos; //SPACING

				x01 = x001;
				y01 = y001;

				//////////////////
				switch (t->justowanie)
				{
				case 1: xt1 = x01 - tdl * tcos;  /*to the right*/
					yt1 = y01 + Ky * tdl * tsin;
					x01 = xt1;
					y01 = yt1;

					break;
				case 2: xt1 = x01 - (tdl / 2) * tcos;  /*middle*/
					yt1 = y01 + (Ky * tdl) / 2 * tsin;
					x01 = xt1;
					y01 = yt1;

					break;
				case 3: xt1 = x01 - (tdl / 2) * tcos + (WysokoscT / 2) * tsin;  /*central*/
					yt1 = y01 + (Ky * tdl) / 2 * tsin - (WysokoscT / 2) * tcos;
					x01 = xt1;
					y01 = yt1;

					break;
				}
				///////////////////
			}
			else out_break = TRUE;
		}
		return;
	}
	////  > 3 mm

	zn = (unsigned char *)t->text;
	ttf_zn = zn;

	out_break = FALSE;

	df_tl0 = tdl;

	if (f_type == 2) df_tl_ = -Get_TTF_Char_Left_Top(t, (char*)zn, font_scale, font_index, &TTF_left, &TTF_top);
	else  df_tl_ = 0.0;

	if (df_tl_ > 0.0) df_tl_ = 0.0;

	df_tl = df_tl_;
	df_tl__ = df_tl_;

	switch (t->justowanie)
	{
	case 0: //to the left
		x01 = x001 + df_tl * tcos;
		y01 = y001 - Ky * df_tl * tsin;
		break;

	case 1: //to the right
		x01 = x001 + (-df_tl0 + df_tl) * tcos;
		y01 = y001 - Ky * (-df_tl0 + df_tl) * tsin;
		break;
	case 2: //middle
		x01 = x001 + ((-df_tl0 + df_tl )/ 2 ) * tcos;
		y01 = y001 - Ky * ((-df_tl0 + df_tl) / 2 ) * tsin;
		break;
	case 3: //central
		x01 = x001 + ((-df_tl0 + df_tl) / 2 ) * tcos + (WysokoscT / 2) * tsin;
		y01 = y001 - Ky * ((-df_tl0 + df_tl) / 2 ) * tsin - (WysokoscT / 2) * tcos;
		break;
	}
	
	ttf_x01 = x01;
	ttf_y01 = y01;


	ptrs_off = PTRS__Text_Style[t->czcionka]->ptrs_off;

	alf = PTRS__Text_Style[t->czcionka]->ptrsz_font;

	i_first = PTRS__Text_Style[t->czcionka]->first_char;
	i_num = PTRS__Text_Style[t->czcionka]->num_char;
	

		zn0 = zn;
		ttf_zn = zn;
			
		k = 0;

		if ((t->ukryty == 0) && (t->typ != n_typ_atrybut))
		{
			if (t->bold == 0) setlinestyle1(SOLID_LINE, 0, NORM_WIDTH);
			else setlinestyle1(SOLID_LINE, 0, 3/*NORM_WIDTH*/);
		}
		else
		{
			setlinestyle1(USERBIT_LINE, 1, 1);
		}


		if (f_type == 2)
		{
			Get_Face_Character_yMax(t, &yMax);
			if (mode == COPY_PUT)
			{
				//text_no++;
				if ((t0->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
				{
					getcolor_RGB_char(&red, &green, &blue, t0_kolor);
				}
				else getcolor_RGB_char(&red, &green, &blue, GetColorAC(8));

				kolor.red = red;
				kolor.gre = green;
				kolor.blu = blue;

				Init_Draw_TTF_text(t0, t0->kat, wysokosc_p, kolor);

			}

		}

		while (*zn != '\0')
		{
			lw = 0;

			if (*zn == '\r')
			{
				zn += 1;
				continue;
			}

			if (*zn == '\n')
			{

				if (t->underline == TRUE)
				{
					df_tl1 = df_tl0 - Get_Text_Len(t, (char*)zn0, 1.0 /*font_scale*/, 0, NULL, NULL, NULL, NULL); //_
					df_tl1 += df_tl__;

					df_tl2 = df_tl0 - Get_Text_Len(t, (char*)zn, font_scale, font_index, NULL, NULL, NULL, NULL);  //_
					df_tl2 += df_tl__;

					if (df_tl1 != df_tl2)
					{

						switch (t->justowanie)
						{
						case 0:
							break;
						case 1:
							ddf_tl = (df_tl2 - df_tl1);
							df_tl1 -= ddf_tl;
							df_tl2 -= ddf_tl;
							break;
						case 2:
							ddf_tl = (df_tl2 - df_tl1) / 2.0;
							df_tl1 -= ddf_tl;
							df_tl2 -= ddf_tl;
							break;
						case 3:
							ddf_tl = (df_tl2 - df_tl1) / 2.0;
							df_tl1 -= ddf_tl;
							df_tl2 -= ddf_tl;
							break;
						}

						df_tl1 = df_tl1 - df_width_marg * 0.1;
						df_tl2 = df_tl2 + df_width_marg * 0.1;

						x01__ = x001 + df_tl1 * tcos;
						y01__ = y001 - Ky * df_tl1 * tsin;

						x01___ = x001 + df_tl2 * tcos;
						y01___ = y001 - Ky * df_tl2 * tsin;

						xu01_ = x01__ + Underl_del * tsin;
						yu01_ = y01__ - Underl_del * tcos;
						xu01 = x01___ + Underl_del * tsin;
						yu01 = y01___ - Underl_del * tcos;

						linen(pikseleX0(xu01_), pikseleY0(yu01_),
							pikseleX0(xu01), pikseleY0(yu01), mode);
					}

				}

				
				if ((f_type == 2) && (mode==COPY_PUT) && (wysokosc_p<MIN_TTF_VIEW) && (!ptr_ind))
				{
					if ((t0->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
					{
						getcolor_RGB_char(&red, &green, &blue, t0_kolor);
					}
					else getcolor_RGB_char(&red, &green, &blue, GetColorAC(8));
					

					kolor.red = red;
					kolor.gre = green;
					kolor.blu = blue;

					Draw_TTF_text(t0, ttf_zn, screenplay, pikseleX0(ttf_x01), pikseleY0(ttf_y01), t0->kat, wysokosc_p, kolor, mode, t_width, t_height);
					
				}
					
				zn += 1;

				zn0 = zn;
				ttf_zn = zn;

                i_matrix=0;
                i_zn=0;
				df_tl0 = Get_Text_Len(t, (char*)zn, font_scale, font_index, &t_width, &t_height, matrix, &i_matrix);

				if (f_type == 2) df_tl_ = - Get_TTF_Char_Left_Top(t, (char*)zn, font_scale, font_index, &TTF_left, &TTF_top);
				
				else df_tl_ = 0.0;
				if (df_tl_ > 0.0) df_tl_ = 0.0;
				df_tl = df_tl_;
				df_tl__ = df_tl_;

				x001 += WysokoscT * ((float)(t0->spacing) / 10.0 + 1.0) *tsin; //SPACING
				y001 -= WysokoscT * ((float)(t0->spacing) / 10.0 + 1.0) *tcos; //SPACING

				switch (t->justowanie)
				{
				case 0: //to the left
					x01 = x001 + df_tl * tcos;
					y01 = y001 - Ky * df_tl * tsin;
					break;
				case 1: //to the right
					x01 = x001 + (-df_tl0 + df_tl) * tcos;
					y01 = y001 - Ky * (-df_tl0 + df_tl) * tsin;
					break;
				case 2: //middle
					x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos;
					y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin;
					break;
				case 3: //central
					x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos + (WysokoscT / 2) * tsin;  /*centralnie*/
					y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin - (WysokoscT / 2) * tcos;
					break;
				}

				ttf_x01 = x01;
				ttf_y01 = y01;

				if (*zn == '\r')
				{
					zn += 1;
					continue;
				}

				if (*zn == '\0')
				{
					break;
				}
			}


			if ((*zn == '\45') &&
				(*(zn + 1) == '\45') &&
				((*(zn + 2) == '\125') || (*(zn + 2) == '\165')))
			{
				if (underlining == FALSE) underlining = TRUE;
				else underlining = FALSE;
				zn += 3;
				if (*zn == '\0')
					break;
			}

            if (*zn == '|')
            {
                if (index_pos) {
                    zn++;
                    continue;
                }
            }
			if (*zn == '\176') //~
			{
                index_pos=TRUE;

				font_index -= 1;

				font_scale = pow(INDEX_FACTOR, abs(font_index));
				df_scale_x = df_scale_x0 * font_scale;
				df_scale_y = df_scale_y0 * font_scale;

				if ((f_type == 2) && (mode == COPY_PUT))
					Amend_Draw_TTF_text(t0, wysokosc_p*font_scale);

				zn++;
				if (*zn == '\40') zn++;
				continue;
			}
			else if (*zn == '\136') //^
			{
                index_pos=TRUE;

				font_index += 1;

				font_scale = pow(INDEX_FACTOR, abs(font_index));
				df_scale_x = df_scale_x0 * font_scale;
				df_scale_y = df_scale_y0 * font_scale;

				if ((f_type == 2) && (mode == COPY_PUT))
					Amend_Draw_TTF_text(t0, wysokosc_p*font_scale);

				zn++;
				if (*zn == '\40') zn++;
				continue;
			}
			else
			{
				if ((f_type < 2) || (f_type == 3) || ((f_type==2) && ((wysokosc_p>= MIN_TTF_VIEW) || (mode==XOR_PUT) || (ptr_ind))))
				{
					if (*zn >= 127)
					{
						//convert to UNICODE and shift index
						u8zn = utf8_to_ucs2(zn, (const uint8_t **) &end_ptr);

                        if (u8zn > 1920)
                        {
                            if (known3b(u8zn)) zn++;  //⁴ e.g. m⁴
                            else
                            {
                                u8zn = 32;
                            }
                        }
                        else {
                            if (f_type == 0)
                            {
                                //conver to Mazovia
                                u8zn = u8toMazovia(u8zn);
                            }
                        }
                        zn++;
					}
					else u8zn = (unsigned int)zn[0];

					if (u8zn < 32)
					{
						zn++;
						continue;
					}

					if (f_type == 0)
					{
						ad = ptrs_off[u8zn - i_first].i_offset;
					}
					else if ((f_type == 1) || (f_type == 3))
					{
						ad = ptrs_off[u8zn].i_offset;
						ptr = alf + ad;
					}
                    else if ((f_type == 2) && (mode == XOR_PUT))
					{
	
						ad = Get_TTF_Char_Outline(t, u8zn, alft, (long *) &lw, yMax);
						//ad = 0;
						ptr = alft;
						i_first = 0;
						i_num = 1920;

					}
					
					if ((f_type < 2) || (f_type == 3) || ((f_type == 2) && (mode == XOR_PUT)))
					{
						if ((ad != OFFEMPTY) && (u8zn <= (i_first + i_num)))
						{

							if (f_type == 0)
							{
								lw = ptrs_off[u8zn - i_first].vectorsno;
							}
							else if ((f_type == 1) || (f_type == 3))
							{
								lw = ptrs_off[u8zn].vectorsno;
							}

							if (font_index > 0)
                                yind = WysokoscT * (1.0 - font_scale + INDEX_SHIFT);
                            else if (font_index < 0)
                                yind = WysokoscT * (-INDEX_SHIFT);
							else yind = 0.0;

							for (li = 0; li < lw; li++)
							{
								keym = readmouse();
								//p_keyboard=my_poll_keyboard();

								if (mvcurb.akton && (mvcurb.mvc /*|| keypressed()*/))
								{
									w_char_no = k;
									w_lw_no = li;
									return;
								}
								if (mvcurb.aktoff && mvcurb.T == 1 &&
									w_char_no == k && w_lw_no == li)
								{
									//if ((f_type == 2) && (mode == XOR_PUT)) free(alf);
									return;
								}
								arc = FALSE;
								if (f_type > 0)
								{
									memmove(&x1_f, ptr, sizeof(float));
									ptr += sizeof(float);
									if (x1_f > 998)  //it's ARC
									{
										memmove(&angle_f, ptr, sizeof(float));
										ptr += sizeof(float);
										memmove(&x1_f, ptr, sizeof(float));
										ptr += sizeof(float);
										arc = TRUE;
									}
									memmove(&y1_f, ptr, sizeof(float));
									ptr += sizeof(float);
									memmove(&x2_f, ptr, sizeof(float));
									ptr += sizeof(float);
									memmove(&y2_f, ptr, sizeof(float));
									ptr += sizeof(float);
									x1 = x1_f * df_scale_x *4.5;
									y1 = y1_f * df_scale_y *4.5;
									x2 = x2_f * df_scale_x *4.5;
									y2 = y2_f * df_scale_y *4.5;
								}
								else
								{
									x1 = alf[ad++] * df_scale_x;
									y1 = alf[ad++] * df_scale_y;
									x2 = alf[ad++] * df_scale_x;
									y2 = alf[ad++] * df_scale_y;
								}

								y1 += yind;
								y2 += yind;

								if (t->italics == 1)
								{
									x1 += y1 * tsin_it;
									x2 += y2 * tsin_it;
								}
								if (b_hor == FALSE)
								{
									obrd(tsin, tcos, x1, y1, &x1, &y1);
									obrd(tsin, tcos, x2, y2, &x2, &y2);
								}
								if (arc == FALSE)
								{
									linen(pikseleX0(x01 + x1), pikseleY0(y01 + y1), pikseleX0(x01 + x2), pikseleY0(y01 + y2), mode);
								}
								else
								{
									angle = angle_f * 3.141592;
									arcSEA(x1, y1, x2, y2, angle, &r, &l_x0, &l_y0, &l_kat1, &l_kat2);

									xr = pikseleX0(x01 + l_x0);
									yr = pikseleY0(y01 + l_y0);
									lr = pikseleDX(r);

									DrawArc(xr, yr, l_kat1, l_kat2, lr, mode);
								}

							}
						}
					}

					else
					{
						keym = readmouse();
						//p_keyboard=my_poll_keyboard();

						if (mvcurb.akton && (mvcurb.mvc /*|| keypressed()*/))
						{
							return;
						}

					    if (font_index > 0)
                            yind = WysokoscT * (1.0 - font_scale + INDEX_SHIFT);
                        else if (font_index < 0)
                            yind = WysokoscT * (-INDEX_SHIFT);
                        else yind = 0.0;
						xind = 0.0;

						if (t->italics == 1)
						{
							xind += yind * tsin_it;
						}
						if (b_hor == FALSE)
						{
							obrd(tsin, tcos, xind, yind, &xind, &yind);
						}

                        x_char = pikseleX0(x01 + xind);
                        y_char = pikseleY0(y01 + yind);

                        if (t->kat==0.0)
                        {
                            if (x_char>=getmaxx())
                            {
                                while (*zn!='\n')
                                {
                                    zn++;
                                    if (*zn == '\176') //~
                                    {
                                        index_pos=TRUE;

                                        font_index -= 1;

                                        font_scale = pow(INDEX_FACTOR, abs(font_index));
                                        df_scale_x = df_scale_x0 * font_scale;
                                        df_scale_y = df_scale_y0 * font_scale;

                                        if ((f_type == 2) && (mode == COPY_PUT))
                                            Amend_Draw_TTF_text(t0, wysokosc_p*font_scale);

                                        zn++;
                                        if (*zn == '\40') zn++;
                                        continue;
                                    }
                                    else if (*zn == '\136') //^
                                    {
                                        index_pos=TRUE;

                                        font_index += 1;

                                        font_scale = pow(INDEX_FACTOR, abs(font_index));
                                        df_scale_x = df_scale_x0 * font_scale;
                                        df_scale_y = df_scale_y0 * font_scale;

                                        if ((f_type == 2) && (mode == COPY_PUT))
                                            Amend_Draw_TTF_text(t0, wysokosc_p*font_scale);

                                        zn++;
                                        if (*zn == '\40') zn++;
                                        continue;
                                    }
                                    if (*zn=='\0')
                                        return;
                                }
                                continue;
                            }
                            else
                                Draw_TTF_char(t0, u8zn, screenplay, x_char, y_char);
                        }
                        else Draw_TTF_char(t0, u8zn, screenplay, x_char, y_char);
					}


                    df_tl = matrix[i_zn];


                    df_tl += df_tl_;

                    i_zn++;

				}

				zn++;
				
				switch (t->justowanie)
				{
				case 0: //do lewej
					x01 = x001 + df_tl * tcos;
					y01 = y001 - Ky * df_tl * tsin;
					break;
				case 1: //do prawej
					x01 = x001 + (-df_tl0 + df_tl) * tcos;
					y01 = y001 - Ky * (-df_tl0 + df_tl) * tsin;
					break;
				case 2: //srodkowo
					x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos;
					y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin;
					break;
				case 3: //centralnie
					x01 = x001 + (-df_tl0 / 2 + df_tl) * tcos + (WysokoscT / 2) * tsin;  /*centralnie*/
					y01 = y001 - Ky * (-df_tl0 / 2 + df_tl) * tsin - (WysokoscT / 2) * tcos;
					break;
				}

				k++;
			}
		}

	
	if ((f_type == 2) && (mode==COPY_PUT) && (wysokosc_p< MIN_TTF_VIEW) && (!ptr_ind))
	{

		if (strlen(ttf_zn) > 0)
		{
			if ((t->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
			{
				getcolor_RGB_char(&red, &green, &blue, t0_kolor);
			}
			else getcolor_RGB_char(&red, &green, &blue, GetColorAC(8));

			kolor.red = red;
			kolor.gre = green;
			kolor.blu = blue;

			Draw_TTF_text(t0, ttf_zn, screenplay, pikseleX0(ttf_x01), pikseleY0(ttf_y01), t0->kat, wysokosc_p, kolor, mode, t_width, t_height);

		}
	}
	
	if (t->underline == TRUE)
	{ 
		df_tl1 = df_tl0 - Get_Text_Len(t, (char*)zn0, 1.0 /*font_scale*/, 0, NULL, NULL, NULL, NULL);  //_
		df_tl1 += df_tl__;
		df_tl2 = df_tl0 - Get_Text_Len(t, (char*)(zn), font_scale, font_index, NULL, NULL, NULL, NULL);  //_
		df_tl2 += df_tl__;

		if (df_tl1 != df_tl2)
		{
			switch (t->justowanie)
			{
			case 0:
				break;
			case 1:
				ddf_tl = (df_tl2 - df_tl1);
				df_tl1 -= ddf_tl;
				df_tl2 -= ddf_tl;
				break;
			case 2:
				ddf_tl = (df_tl2 - df_tl1) / 2.0;
				df_tl1 -= ddf_tl;
				df_tl2 -= ddf_tl;
				break;
			case 3:
				ddf_tl = (df_tl2 - df_tl1) / 2.0;
				df_tl1 -= ddf_tl;
				df_tl2 -= ddf_tl;
				break;
			}

			df_tl1 = df_tl1 - df_width_marg * 0.1;

			x01__ = x001 + df_tl1 * tcos;
			y01__ = y001 - Ky * df_tl1 * tsin;

			x01___ = x001 + df_tl2 * tcos;
			y01___ = y001 - Ky * df_tl2 * tsin;

			xu01_ = x01__ + Underl_del * tsin;
			yu01_ = y01__ - Underl_del * tcos;
			xu01 = x01___ + Underl_del * tsin;
			yu01 = y01___ - Underl_del * tcos;

			linen(pikseleX0(xu01_), pikseleY0(yu01_),
				pikseleX0(xu01), pikseleY0(yu01), mode);
		}
	}
	
	if (mvcurb.akton)
	{
		w_char_no = k;  ////UWAGA !!!!!!!!!!!!!!!!
		w_lw_no = lw;   ////UWAGA !!!!!!!!!!!!!!!!
		w_text_no = text_no;
	}

  return;  
}

void outtextxy_w_v (TEXT *t, int mode)
/*--------------------------------*/
{ if ((t->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
	{
		if (mode == XOR_PUT)
		{
			if (t->kolor == 0) SetColorAC(15); else SetColorAC(t->kolor);
		}
		else SetColorAC(t->kolor);
	}
    else SetColorAC(8);
  outtextxy_w_ (t, mode) ;
}

void outtextxy_w (TEXT *t, int mode)
/*--------------------------------*/
{
	if ((t->warstwa == Current_Layer) || (options1.view_only_current_layer == 0))
	{
		if (mode == XOR_PUT)
		{
            setcolor(kolory.blok);
		}
		else SetColorAC(t->kolor);
	}
    else SetColorAC(8);
  outtextxy_w_ (t, mode) ;
}

void outtextxy_w_0 (TEXT *t)
/*-------------------------*/
{
	outtextxy_w_ (t, NOT_PUT) ;
}

void outtextxy_w1 (TEXT *t, int mode)
/*--------------------------------*/
{ if ((t->warstwa==Current_Layer) || (options1.view_only_current_layer==0)) 
	{
		if (mode == XOR_PUT)
		{
			if (t->kolor == 0) SetColorAC(15); else SetColorAC(t->kolor);
		}
		else SetColorAC(t->kolor);
	}
    else SetColorAC(8);
  outtextxy_w_ (t, mode) ;
}

void outtextxy_wP0_ (TEXT *t)
/*-----------------------------------*/
{

    setcolor(kolory.paper);
    outtextxy_w_ (t, COPY_PUT) ;
}

void outtextxy_wP_ (TEXT *t, int mode)
/*-----------------------------------*/
{  
	
	if (mode == XOR_PUT)
	{
        setcolor(kolory.blok);
	}
	else SetColorAC(t->kolor);
		
    outtextxy_w_ (t, mode) ;
}

void outtextxy_wP (TEXT *t, int mode)
/*---------------------------------*/
{
  if ((t->warstwa==Current_Layer) || (options1.view_only_current_layer==0))
  {
	  if (mode == XOR_PUT)
	  {
		  if (t->kolor == 0) SetColorAC(15); else SetColorAC(t->kolor);
	  }
	  else SetColorAC(t->kolor);
  }
    else SetColorAC(8);
  outtextxy_wP_ (t, mode) ;
}
