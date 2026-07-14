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

#include <forwin.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef LINUX
#include <unistd.h>
#endif
#define ALLEGWIN
#include <allegext.h>
#include<math.h>
#include<string.h>

#include "bib_e.h"
#include "rysuj_e.h"
#include "o_bloklb.h"
#include "bgiext.h"

#include<fcntl.h>
#include "lodepng.h"

#include "leak_detector_c.h"

#ifdef LINUX
#include <arpa/inet.h>
#endif

static B_PCX *adr_pcx;
static unsigned int dlugosc_bloku_pcx;

#ifdef LINUX
extern long filelength(int f);
#endif
extern void InitBufPCX(unsigned long buffer_size);
extern void FreeBufPCX(void);

#ifndef LINUX
////#ifndef BIT64
static uint32_t ntohl(uint32_t const net) {
	uint8_t data[4] = {0};
	memcpy(&data, &net, sizeof(data));

	return ((uint32_t)data[3] << 0)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[0] << 24);
}
////#endif
#endif

static unsigned int Ntohl(int chunklen)
{
    return  ntohl(chunklen);
}

int LoadPNGinfo(char *filename, unsigned int *width, unsigned int *height, unsigned int *dpiX, unsigned int *dpiY, unsigned char * bits_per_pixel)
{
	int i;
	unsigned char key;
	int fp;
	unsigned char png_signature[8];
	unsigned char png_template[8] = { 0x89,0x50,0x4E,0x47,0x0D,0x0A,0x1A,0x0A };
	unsigned char IHDR_signature[4];
	unsigned char IHDR_template[4] = { 0x49, 0x48, 0x44, 0x52 };
	unsigned int width_, height_;
	unsigned char bit_depth_, color_type_;
	unsigned int chunklen;
	unsigned char chunktype[4];
	unsigned int chunkcrc;
	unsigned char pHYs[4] = { 'p','H','Y','s' };
	unsigned int Pixels_X_per_unit;
	unsigned int Pixels_Y_per_unit;
	unsigned char Unit_specifier;

#ifdef LINUX
    off_t new_position;
    off_t end_position;
#else
	long new_position;
	long end_position;
#endif
    BOOL e_of_f=FALSE;

	*dpiX = 300;
	*dpiY = 300;

	fp = open(filename, O_RDWR | O_BINARY);

    end_position = lseek(fp, 0, SEEK_END);

	lseek(fp, 0, SEEK_SET);
	if (read(fp, png_signature, 8) != 8) goto error;
	for (i = 0; i < 8; i++) if (png_signature[i] != png_template[i]) goto error;

	lseek(fp, 12, SEEK_SET);
	if (read(fp, IHDR_signature, 4) != 4) goto error;
	for (i = 0; i < 4; i++) if (IHDR_signature[i] != IHDR_template[i]) goto error;

	lseek(fp, 16, SEEK_SET);
	if (read(fp, (char *)&width_, 4) != 4) goto error;
	if (read(fp, (char *)&height_, 4) != 4) goto error;
	if (read(fp, (char*)&bit_depth_, 1) != 1) goto error;  // values 1, 2, 4, 8, or 16)
	if (read(fp, (char*)&color_type_, 1) != 1) goto error; // values 0, 2, 3, 4, or 6)

	lseek(fp, 8, SEEK_SET);

#ifndef LINUX
	while (!eof(fp))
#else
    while (e_of_f==FALSE)
#endif
    {
        if (read(fp, (char *) &chunklen, 4) != 4) goto error;
        if (read(fp, (char *) &chunktype, 4) != 4) goto error;
        //if (read(fp, (char *)&chunkcrc, 4) != 4) goto error;
        chunklen = Ntohl(chunklen);
        if (memcmp(&chunktype, &pHYs, 4) == 0) {
            /*
            The pHYs chunk specifies the intended pixel size or aspect ratio for display of the image. It contains:

               Pixels per unit, X axis: 4 bytes (unsigned integer)
               Pixels per unit, Y axis: 4 bytes (unsigned integer)
               Unit specifier:          1 byte
            The following values are defined for the unit specifier:

               0: unit is unknown
               1: unit is the meter
            When the unit specifier is 0, the pHYs chunk defines pixel aspect ratio only; the actual size of the pixels remains unspecified.

            Conversion note: one inch is equal to exactly 0.0254 meters.

            If this ancillary chunk is not present, pixels are assumed to be square, and the physical size of each pixel is unknown.

            If present, this chunk must precede the first IDAT chunk.
            */
            if (read(fp, (char *) &Pixels_X_per_unit, 4) != 4) goto error;
            if (read(fp, (char *) &Pixels_Y_per_unit, 4) != 4) goto error;
            if (read(fp, (char *) &Unit_specifier, 1) != 1) goto error;
            if (Unit_specifier == 1) {
                *dpiX = (int) ((double) Ntohl(Pixels_X_per_unit) * 0.0254 + 0.5);
                *dpiY = (int) ((double) Ntohl(Pixels_Y_per_unit) * 0.0254 + 0.5);
            }

            break;
        }

        new_position = lseek(fp, chunklen + 4, SEEK_CUR);

#ifdef LINUX
        if (new_position == end_position) e_of_f = TRUE;
#endif
    }

	close(fp);

	*width = Ntohl(width_);
	*height = Ntohl(height_);
	if ((bit_depth_ == 8) && (color_type_ == 6)) *bits_per_pixel = 32;
	else  *bits_per_pixel = 24;  //fixed
	return 1;

	error:
	close(fp);
	return 0;
}

uint16_t swap_uint16(uint16_t val)
{
	return (val << 8) | (val >> 8);
}

#pragma pack (1)
typedef struct _JFIFHeader
{
	unsigned char SOI[2];          /* 00h  Start of Image Marker     */
	unsigned char APP0[2];         /* 02h  Application Use Marker    */
	unsigned short Length;       /* 04h  Length of APP0 Field      */
	char Identifier[5];            /* 06h  "JFIF" (zero terminated) Id String */
	unsigned short Version;      /* 07h  JFIF Format Revision      */
	unsigned char Units;           /* 09h  Units used for Resolution */
	unsigned short Xdensity;     /* 0Ah  Horizontal Resolution     */
	unsigned short Ydensity;     /* 0Ch  Vertical Resolution       */
	unsigned char XThumbnail;      /* 0Eh  Horizontal Pixel Count    */
	unsigned char YThumbnail;      /* 0Fh  Vertical Pixel Count      */
} JFIFHEAD;

typedef struct _JFIFHeaderExt
{
	unsigned char APP0[2];         /* 02h  Application Use Marker FF E0    */
	unsigned short Length;       /* 04h  Length of APP0 Field      */
	char Identifier[5];            /* 06h  "JFXX" (zero terminated) Id String */
	unsigned char ThumbnailFormat;      /* 10: JPEG format, 11: 1byte per pixel palletized format, 13: 3byte per pixel RGB format    */
	unsigned char ThumbnailData;      /* Depends on the thumbnail format     */
} JFIFHEADEXT;

BOOL CheckJPGExif(char* filename)
{

	JFIFHEAD jpgheader;
	JFIFHEADEXT jpgheaderext;
	int thumbnals_size;

	int iPos, i;

	FILE* fp = fopen(filename, "rb");
	fread(&jpgheader, 1, sizeof(jpgheader), fp);
	
	if (strcmp(jpgheader.Identifier, "Exif") == 0)
	{
		fclose(fp);
		return TRUE;
	}
	thumbnals_size = jpgheader.XThumbnail * jpgheader.YThumbnail;
	if (thumbnals_size > 0)
	{
		unsigned char* thumbnailBuffer = (unsigned char*)malloc(thumbnals_size + 1);
		fread(thumbnailBuffer, 1, sizeof(thumbnals_size), fp);
		free(thumbnailBuffer);
	}

	fread(&jpgheaderext, 1, sizeof(jpgheaderext), fp);
	if (strcmp(jpgheaderext.Identifier, "Exif") == 0)
	{
		fclose(fp);
		return TRUE;
	}

	fclose(fp);
	return FALSE;
}

int LoadJPGinfo(char* filename, unsigned int* width, unsigned int* height, unsigned int* dpiX, unsigned int* dpiY, unsigned char* bits_per_pixel)
{

	JFIFHEAD jpgheader;
	JFIFHEADEXT jpgheaderext;
	int thumbnals_size;
	size_t count;

	int iHeight = 0, iWidth = 0, iPos, i;

	FILE* fp = fopen(filename, "rb");
    if (fp==NULL) return 0;
	count = fread(&jpgheader, 1, sizeof(jpgheader), fp);
	if (count < sizeof(jpgheader)) return 0;

	jpgheader.Xdensity = swap_uint16(jpgheader.Xdensity);
	jpgheader.Ydensity = swap_uint16(jpgheader.Ydensity);

	thumbnals_size = jpgheader.XThumbnail * jpgheader.YThumbnail;
	if (thumbnals_size > 0)
	{
		unsigned char* thumbnailBuffer = (unsigned char*)malloc(thumbnals_size + 1);
		fread(thumbnailBuffer, 1, sizeof(thumbnals_size), fp);
		free(thumbnailBuffer);
	}

	fread(&jpgheaderext, 1, sizeof(jpgheaderext), fp);


	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	unsigned char* ucpImageBuffer = (unsigned char*)malloc(len + 1);
	fread(ucpImageBuffer, 1, len, fp);
	fclose(fp);

	int n = 0;
	/*Extract start of frame marker(FFCO) of width and hight and get the position*/
	for (i = 0; i < len; i++)
	{
		if ((ucpImageBuffer[i] == 0xFF) && ((ucpImageBuffer[i + 1] == 0xC0) || (ucpImageBuffer[i + 1] == 0xC2)))
		{
			iPos = i;
			break;
		}
	}

	/*Moving to the particular byte position and assign byte value to pointer variable*/
	iPos = iPos + 5;
	iHeight = ucpImageBuffer[iPos] << 8 | ucpImageBuffer[iPos + 1];
	iWidth = ucpImageBuffer[iPos + 2] << 8 | ucpImageBuffer[iPos + 3];

	*width = iWidth;
	*height = iHeight;

	if (strcmp(jpgheader.Identifier, "Exif") == 0)
	{
		*dpiX = 72;
		*dpiY = 72;
	}
	else
	{
		*dpiX = jpgheader.Xdensity;
		*dpiY = jpgheader.Ydensity;
	}

	*bits_per_pixel = 24;  //fixed

	free(ucpImageBuffer);

	return 1;

}


int Load_PNG_JPG_real(B_PCX *b_pcx, char *nazwa_pcx, size_t width, size_t height, int horz_res, int vert_res, unsigned char bits_per_pixel, int type)
{
	int i, j;
	int byte_x;
	int count = 0;          // current pixel number in the image
	int total;            // number of total pixels in the image
	COLOR_ key_pal[256];             // current byte read from the image
	PCXheader head;
	byte_ key0;
	int num_bytes;        // number of bytes in a "run"
	double pcx_x, pcx_x1, pcx_x2;
	double pcx_y, pcx_y1, pcx_y2;
	BOOL empty_line = TRUE;
	long size_of_file;
	long nr_byte;
	char str1[30];
	int del_rgb, min_rgb;
	int uchwyt;
	FILE *stru;
	int iin, ii, k;
	char str01[255], str02[80], str03[80], str04[80];
	double xx, yy;
	int fp;
	COLOR palette;
	int liczba_kolorow;
	STRIPS *strips;
	COLOR *pcxpalette;
	long file_len;
	char *adr_pcx_data;
	
	xx = b_pcx->x; yy = b_pcx->y;
	key0 = 0;

	fp = open(nazwa_pcx, O_RDWR | O_BINARY);

	if (fp == -1)
	{
		ErrList(202);
		return 0;
	}

	file_len = filelength(fp);

	InitBufPCX(sizeof(B_PCX) + file_len + sizeof(STRIPS) + 64);

	if (bufor_pcx == NULL)
	{
		ErrList(211);
		return 0;
	}

	adr_pcx = (B_PCX *)bufor_pcx;

	head.bits_per_pixel = bits_per_pixel;
	head.xmin = 0;
	head.ymin = 0;
	head.xmax = width;
	head.ymax = height;
	head.horz_res = horz_res;
	head.vert_res = vert_res;
	head.bytes_per_line = 0;
	//head.ega_palette
	head.encoding = 1;
	head.num_color_planes = 1;
	//head.padding
	head.palette_type = 1;
	head.manufacturer = 10;
	head.version = 5;
	head.reserved = 0;

	width = head.xmax - head.xmin + 1;         // image dimensions...
	height = head.ymax - head.ymin + 1;

	adr_pcx->atrybut = Anormalny;
	adr_pcx->obiekt = Opcx;
	adr_pcx->obiektt1 = 0;
	adr_pcx->obiektt2 = 0;
	adr_pcx->obiektt3 = 0;
	adr_pcx->widoczny = 1;
	adr_pcx->przec = 0;
	adr_pcx->blok = 0;
	adr_pcx->n = T294 + sizeof(head) + filelength(fp);
	adr_pcx->warstwa = Current_Layer;
	adr_pcx->kod_obiektu = type+2;  //1=PCX,  2=PNG, 3=JPG
	adr_pcx->inwersja = b_pcx->inwersja;
	adr_pcx->ignore_background = b_pcx->ignore_background;
	adr_pcx->ignore_print_background = b_pcx->ignore_print_background;
	adr_pcx->set_foreground = b_pcx->set_foreground;
	adr_pcx->background = b_pcx->background;
	adr_pcx->foreground = b_pcx->foreground;
	adr_pcx->markers = b_pcx->markers;
	adr_pcx->stripped = b_pcx->stripped;
	adr_pcx->buffered = 0;
	adr_pcx->on_front = b_pcx->on_front;
	adr_pcx->h_flip = b_pcx->h_flip;
	adr_pcx->v_flip = b_pcx->v_flip;
	adr_pcx->kat = b_pcx->kat;
	adr_pcx->x = b_pcx->x;
	adr_pcx->y = b_pcx->y;
	adr_pcx->dx = b_pcx->dx;
	adr_pcx->dy = b_pcx->dy;
	adr_pcx->i_x = head.xmax - head.xmin + 1;
	adr_pcx->i_y = head.ymax - head.ymin + 1;

	adr_pcx->len_pcx = file_len + sizeof(head); // filelength(fp);

	if (head.manufacturer != 10         // check for errors
		|| head.version < 5
		|| head.encoding != 1
		|| head.xmin >= head.xmax
		|| head.ymin >= head.ymax)
	{
		close(fp);
		ErrList(202);
		FreeBufPCX();
		return 0;
	}

	memmove(adr_pcx->pcx, &head, sizeof(head)); //header is alread read

	lseek(fp, 0, SEEK_SET);

	dlugosc_bloku_pcx = 0;

	nr_byte = 128;
	adr_pcx_data = adr_pcx->pcx + sizeof(head);

	if (read(fp, adr_pcx_data, filelength(fp)) != (filelength(fp)))
	{
		ErrList(202);
		FreeBufPCX();
		return 0;
	}

	//na koncu bloku pcx dostawiony jest blok strips
	adr_pcx->n += sizeof(STRIPS);
	adr_pcx->markers = 1;   //wystepuja adresy paskow
	adr_pcx->stripped = 0;  //jeszcze nie pociety na paski

	//oznacza to adres palety dla 8bpp znajduje sie

	strips = (STRIPS *)(adr_pcx->pcx +sizeof(PCXheader) + filelength(fp));

	for (k = 0; k < 100; k++)
	{
		strips->strip[k].wiersz = 0;
		strips->strip[k].adr = 0;
		strips->strip[k].count = 0;
	}

	close(fp);

	if (dodaj_obiekt(NULL, adr_pcx) == NULL)
	{
		bitmap_exist = FALSE;
		bitmap_view = TRUE;
	}

	FreeBufPCX();
	return 1;
}