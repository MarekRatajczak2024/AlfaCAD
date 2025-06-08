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

#define __O_TEXT3PDF__

#include <setjmp.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "forwin.h"
#include "hpdf.h"
#include "hpdf_utils.h"
#ifndef LINUX
#include "hpdf_errorcodes.h"
#endif

#include "message.h"

/*
#define HEADER                   "<?xpacket begin='' id='W5M0MpCehiHzreSzNTczkc9d'?><x:xmpmeta xmlns:x='adobe:ns:meta/' x:xmptk='XMP toolkit 2.9.1-13, framework 1.6'><rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#' xmlns:iX='http://ns.adobe.com/iX/1.0/'>"
#define DC_HEADER                "<rdf:Description xmlns:dc='http://purl.org/dc/elements/1.1/' rdf:about=''>"
#define DC_TITLE_STARTTAG        "<dc:title><rdf:Alt><rdf:li xml:lang=\"x-default\">"
#define DC_TITLE_ENDTAG          "</rdf:li></rdf:Alt></dc:title>"
#define DC_CREATOR_STARTTAG      "<dc:creator><rdf:Seq><rdf:li>"
#define DC_CREATOR_ENDTAG        "</rdf:li></rdf:Seq></dc:creator>"
#define DC_DESCRIPTION_STARTTAG  "<dc:description><rdf:Alt><rdf:li xml:lang=\"x-default\">"
#define DC_DESCRIPTION_ENDTAG    "</rdf:li></rdf:Alt></dc:description>"
#define DC_FOOTER                "</rdf:Description>"
#define XMP_HEADER               "<rdf:Description xmlns:xmp='http://ns.adobe.com/xap/1.0/' rdf:about=''>"
#define XMP_CREATORTOOL_STARTTAG "<xmp:CreatorTool>"
#define XMP_CREATORTOOL_ENDTAG   "</xmp:CreatorTool>"
#define XMP_CREATE_DATE_STARTTAG "<xmp:CreateDate>"
#define XMP_CREATE_DATE_ENDTAG   "</xmp:CreateDate>"
#define XMP_MOD_DATE_STARTTAG    "<xmp:ModifyDate>"
#define XMP_MOD_DATE_ENDTAG      "</xmp:ModifyDate>"
#define XMP_FOOTER               "</rdf:Description>"
#define PDF_HEADER               "<rdf:Description xmlns:pdf='http://ns.adobe.com/pdf/1.3/' rdf:about=''>"
#define PDF_KEYWORDS_STARTTAG    "<pdf:Keywords>"
#define PDF_KEYWORDS_ENDTAG      "</pdf:Keywords>"
#define PDF_PRODUCER_STARTTAG    "<pdf:Producer>"
#define PDF_PRODUCER_ENDTAG      "</pdf:Producer>"
#define PDF_FOOTER               "</rdf:Description>"
#define PDFAID_PDFA1A            "<rdf:Description rdf:about='' xmlns:pdfaid='http://www.aiim.org/pdfa/ns/id/' pdfaid:part='1' pdfaid:conformance='A'/>"
#define PDFAID_PDFA1B            "<rdf:Description rdf:about='' xmlns:pdfaid='http://www.aiim.org/pdfa/ns/id/' pdfaid:part='1' pdfaid:conformance='B'/>"
#define FOOTER                   "</rdf:RDF></x:xmpmeta><?xpacket end='w'?>"
*/


jmp_buf env;

static HPDF_STATUS pdf_error = 0;
static HPDF_STATUS pdf_temp_error = 0;

extern int ask_question(int n_buttons, char *esc_string, char *ok_string, char *cont_string, char *comment_string, int color_comment, char *comment1_string, int color1_comment, int cien, int image);

static char confirm[] = u8"Confirm";
extern char *hpdf_error_detail[];
extern size_t utf8_num_bytes(char * s);
extern int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);
extern int my_file_exists(char *name);
extern char* strupr(char* s);
extern char *winfont;
extern char *otffont;
extern int findfile_recursive(char *folder,const char *filename, char *fullpath);
extern void find_any_font_face(char *Font_File, char *face_name);

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void* user_data);
/*

                GLOBAL FUNCTIONS

*/

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler(HPDF_STATUS   error_no,
              HPDF_STATUS   detail_no,
              void         *user_data)
{
    char error_str[64];
    char comment_str[64];
    int ret;
    char *ptr;

    sprintf(error_str, "ERROR: error_no=%04X", (HPDF_UINT)error_no);
    strncpy(comment_str, hpdf_error_detail[error_no - 0x1000], 63);
    comment_str[63] = '\0';
    ptr = strchr(comment_str, '\n');
    if (ptr) *ptr = '\0';
    ret = ask_question(1, "", confirm, "", error_str, 12, comment_str, 11, 1, 62);
    pdf_temp_error = error_no;
}


/* Write XMP Metadata for PDF/A */
/*
HPDF_STATUS
HPDF_PDFA_SetPDFAConformance1(HPDF_Doc pdf, HPDF_PDFAType pdfatype)
{
    HPDF_OutputIntent xmp;
    HPDF_STATUS ret;

    const char* dc_title = NULL;
    const char* dc_creator = NULL;
    const char* dc_description = NULL;

    const char* xmp_CreatorTool = NULL;
    const char* xmp_CreateDate = NULL;
    const char* xmp_ModifyDate = NULL;

    const char* pdf_Keywords = NULL;
    const char* pdf_Producer = NULL;

    const char* info = NULL;

    if (!HPDF_HasDoc(pdf)) {
        return HPDF_INVALID_DOCUMENT;
    }

    dc_title = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_TITLE);
    dc_creator = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_AUTHOR);
    dc_description = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_SUBJECT);

    xmp_CreateDate = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_CREATION_DATE);
    xmp_ModifyDate = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_MOD_DATE);
    xmp_CreatorTool = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_CREATOR);

    pdf_Keywords = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_KEYWORDS);
    pdf_Producer = (const char*)HPDF_GetInfoAttr(pdf, HPDF_INFO_PRODUCER);

    if ((dc_title != NULL) || (dc_creator != NULL) || (dc_description != NULL)
        || (xmp_CreateDate != NULL) || (xmp_ModifyDate != NULL) || (xmp_CreatorTool != NULL)
        || (pdf_Keywords != NULL)) {

        xmp = HPDF_DictStream_New(pdf->mmgr, pdf->xref);
        if (!xmp) {
            return HPDF_INVALID_STREAM;
        }

        // Update the PDF number version
        pdf->pdf_version = HPDF_VER_14;

        HPDF_Dict_AddName(xmp, "Type", "Metadata");
        HPDF_Dict_AddName(xmp, "SubType", "XML");

        ret = HPDF_OK;
        ret += HPDF_Stream_WriteStr(xmp->stream, HEADER);

        // Add the dc block
        if ((dc_title != NULL) || (dc_creator != NULL) || (dc_description != NULL)) {
            ret += HPDF_Stream_WriteStr(xmp->stream, DC_HEADER);

            if (dc_title != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, DC_TITLE_STARTTAG);
                ret += HPDF_Stream_WriteStr(xmp->stream, dc_title);
                ret += HPDF_Stream_WriteStr(xmp->stream, DC_TITLE_ENDTAG);
            }

            if (dc_creator != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, DC_CREATOR_STARTTAG);
                ret += HPDF_Stream_WriteStr(xmp->stream, dc_creator);
                ret += HPDF_Stream_WriteStr(xmp->stream, DC_CREATOR_ENDTAG);
            }

            if (dc_description != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, DC_DESCRIPTION_STARTTAG);
                ret += HPDF_Stream_WriteStr(xmp->stream, dc_description);
                ret += HPDF_Stream_WriteStr(xmp->stream, DC_DESCRIPTION_ENDTAG);
            }

            ret += HPDF_Stream_WriteStr(xmp->stream, DC_FOOTER);
        }

        // Add the xmp block
        if ((xmp_CreateDate != NULL) || (xmp_ModifyDate != NULL) || (xmp_CreatorTool != NULL)) {
            ret += HPDF_Stream_WriteStr(xmp->stream, XMP_HEADER);

            // Add CreateDate, ModifyDate, and CreatorTool
            if (xmp_CreatorTool != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, XMP_CREATORTOOL_STARTTAG);
                ret += HPDF_Stream_WriteStr(xmp->stream, xmp_CreatorTool);
                ret += HPDF_Stream_WriteStr(xmp->stream, XMP_CREATORTOOL_ENDTAG);
            }

            if (xmp_CreateDate != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, XMP_CREATE_DATE_STARTTAG);
                // Convert date to XMP compatible format
                ret += ConvertDateToXMDate(xmp->stream, xmp_CreateDate);
                ret += HPDF_Stream_WriteStr(xmp->stream, XMP_CREATE_DATE_ENDTAG);
            }

            if (xmp_ModifyDate != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, XMP_MOD_DATE_STARTTAG);
                ret += ConvertDateToXMDate(xmp->stream, xmp_ModifyDate);
                ret += HPDF_Stream_WriteStr(xmp->stream, XMP_MOD_DATE_ENDTAG);
            }

            ret += HPDF_Stream_WriteStr(xmp->stream, XMP_FOOTER);
        }

        // Add the pdf block
        if ((pdf_Keywords != NULL) || (pdf_Producer != NULL)) {
            ret += HPDF_Stream_WriteStr(xmp->stream, PDF_HEADER);

            if (pdf_Keywords != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, PDF_KEYWORDS_STARTTAG);
                ret += HPDF_Stream_WriteStr(xmp->stream, pdf_Keywords);
                ret += HPDF_Stream_WriteStr(xmp->stream, PDF_KEYWORDS_ENDTAG);
            }

            if (pdf_Producer != NULL) {
                ret += HPDF_Stream_WriteStr(xmp->stream, PDF_PRODUCER_STARTTAG);
                ret += HPDF_Stream_WriteStr(xmp->stream, pdf_Producer);
                ret += HPDF_Stream_WriteStr(xmp->stream, PDF_PRODUCER_ENDTAG);
            }

            ret += HPDF_Stream_WriteStr(xmp->stream, PDF_FOOTER);
        }

        // Add the pdfaid block
        switch (pdfatype) {
        case HPDF_PDFA_1A:
            ret += HPDF_Stream_WriteStr(xmp->stream, PDFAID_PDFA1A);
            break;
        case HPDF_PDFA_1B:
            ret += HPDF_Stream_WriteStr(xmp->stream, PDFAID_PDFA1B);
            break;
        }

        ret += HPDF_Stream_WriteStr(xmp->stream, FOOTER);

        if (ret != HPDF_OK) {
            return HPDF_INVALID_STREAM;
        }

        if ((ret = HPDF_Dict_Add(pdf->catalog, "Metadata", xmp)) != HPDF_OK) {
            return ret;
        }

        return HPDF_PDFA_GenerateID(pdf);
    }

    return HPDF_OK;
}
*/


HPDF_Image HPDF_LoadPngImageFromFile_(HPDF_Doc  pdf_doc, char *png_file)
{
    return  HPDF_LoadPngImageFromFile(pdf_doc, png_file);
}

HPDF_Page SetupNewPage(HPDF_Doc pdf, HPDF_Font font)
{
    HPDF_REAL height;
    HPDF_REAL width;
    HPDF_Page page;
    page = HPDF_AddPage (pdf);

    height = HPDF_Page_GetHeight (page);
    width = HPDF_Page_GetWidth (page);

    HPDF_Page_BeginText (page);
    HPDF_Page_SetFontAndSize (page, font, 7);
    HPDF_Page_MoveTextPos (page, 20, height - 20);
    HPDF_Page_SetTextLeading (page, 12);
    
    return page;
}

/*
void addColorOutputIntent(HPDF_Doc pdf)
{
#ifndef LINUX
    HPDF_OutputIntent icc;
    HRSRC resource = FindResource(NULL, MAKEINTRESOURCE(IDR_RCDATA1), RT_RCDATA);
    unsigned int size = SizeofResource(NULL, resource);

    HGLOBAL resource_data = LoadResource(NULL, resource);
    void* icc_data = LockResource(resource_data);
#else
    HPDF_OutputIntent icc;
#endif

    icc = HPDF_DictStream_New (pdf->mmgr, pdf->xref);
    HPDF_Dict_AddNumber (icc, "N", 1); // 1 is DeviceGray
    HPDF_Dict_AddName (icc, "Alternate", "DeviceGray");
    HPDF_PDFA_AppendOutputIntents(pdf, "DeviceGray", icc);
}
*/

int text2PDF (char *in_file_name, char *out_file_name)
{
    const int MAX_LINE_LENGTH = 1024;
    HPDF_Doc  pdf_doc;
    HPDF_Page page;
    HPDF_Font def_font;
    FILE* in_file = NULL;
    char text_line[1024];
    char new_line[1024];
    const char* font_name;
    HPDF_STATUS status;
    HPDF_Date hpdfDate;
    struct tm *current_time = NULL;                   
    time_t t_val = 0;
    int length;
    int lines = 0;
    int ret;
    char *ptr;
    int i, cnt, len, lenc;

    int flags;
    char drive[MAXDRIVE]="";
    char dir[MAXDIR]="";
    char file[MAXFILE]="";
    char ext[MAXEXT]="", ext_[MAXEXT]="";
    char ttf_file_path[MAXPATH]="";
    char *fontdir;
    char face_name[128]="";

#define TABS 4

    time(&t_val);
    current_time = gmtime(&t_val);


    pdf_doc = HPDF_New ((HPDF_Error_Handler)error_handler, NULL);
    if (!pdf_doc) {
        ret = ask_question(1, "", (char*)confirm, "", (char*)_CANNOT_CREATE_PDF_, 12, "", 11, 1, 62);
        return 0;
    }

    HPDF_UseUTFEncodings(pdf_doc);
    HPDF_SetCurrentEncoder(pdf_doc, "UTF-8");
    HPDF_SetCompressionMode(pdf_doc, HPDF_COMP_ALL);

    // Metadata
    HPDF_SetInfoAttr(pdf_doc, HPDF_INFO_AUTHOR, "AlfaCAD static");
    HPDF_SetInfoAttr(pdf_doc, HPDF_INFO_CREATOR, "AlfaCAD static");
    HPDF_SetInfoAttr(pdf_doc, HPDF_INFO_PRODUCER, "AlfaCAD");

    hpdfDate.year        = current_time->tm_year + 1900;
    hpdfDate.month       = current_time->tm_mon + 1;
    hpdfDate.day         = current_time->tm_mday;
    hpdfDate.hour        = current_time->tm_hour;
    hpdfDate.minutes     = current_time->tm_min;
    hpdfDate.seconds     = current_time->tm_sec;
    hpdfDate.ind         = '+';
    hpdfDate.off_hour    = 0;
    hpdfDate.off_minutes = 0;
    HPDF_SetInfoDateAttr(pdf_doc, HPDF_INFO_CREATION_DATE, hpdfDate);
    HPDF_SetInfoDateAttr(pdf_doc, HPDF_INFO_MOD_DATE, hpdfDate);

    char *ptrsz_file="cour.ttf";

    flags = fnsplit(ptrsz_file, drive, dir, file, ext);
    if (my_file_exists(ptrsz_file)) strcpy(ttf_file_path, ptrsz_file);
    else
    {

        strcpy(ext_, ext);
#ifndef LINUX
        if (strcmp(_strupr(ext_),".OTF")==0)
#else
        if (strcmp(strupr(ext_),".OTF")==0)
#endif
            fontdir=otffont;
        else fontdir=winfont;
        if (!findfile_recursive(fontdir, ptrsz_file, &ttf_file_path)) return 0;
    }

    find_any_font_face(ttf_file_path, &face_name);
    if (strlen(face_name)==0) strcpy(&face_name, &file);  //TO CHANGE

    font_name = HPDF_LoadTTFontFromFile(pdf_doc, ttf_file_path, HPDF_TRUE);
    if (pdf_temp_error > 0)
    {
        pdf_error = 0x1050;
        pdf_temp_error = 0;
        return 0;
    }
    if (font_name == NULL)
    {
        return 0;
    }

    def_font = HPDF_GetFont(pdf_doc, font_name, "UTF-8");  //"UTF-8" "WinAnsiEncoding"
    
    HPDF_SetCompressionMode(pdf_doc, HPDF_COMP_NONE);
    page = SetupNewPage(pdf_doc, def_font);

    HPDF_Page_SetRGBFill(page, 0, 0, 0);

    in_file = fopen(in_file_name, "rt");
    if(in_file != NULL)
    {
       while(feof(in_file) == 0)
       {
          fgets(text_line, MAX_LINE_LENGTH, in_file);

          length = strlen(text_line);
          text_line[length - 1] = '\0'; // Eat line feeds

          if (lines > 65 || (int)text_line[0] == 12) // Encountered page feed characters
          {
            text_line[0] = ' ';
            HPDF_Page_EndText(page);
            page = SetupNewPage(pdf_doc, def_font);
            HPDF_Page_SetRGBFill(page, 0, 0, 0);
            lines = 0;
          }
#define TABS 4
          if (feof(in_file) == 0) 
          {
            ptr=strchr(text_line,'\t');
            if (ptr==NULL) HPDF_Page_ShowTextNextLine(page, text_line);
            else
            {
                 len=strlen(text_line);
                 cnt=0;
                 new_line[0]='\0';
                 i=0;
                 while (i<len)
                 {
                     if (text_line[i] == '\t')
                     {
                         do
                         {
                             new_line[cnt] = ' ';
                             ++cnt;
                         } while (cnt % TABS);  //find the Tab spot by modulus. Clean division (no remainder) defines a Tab spot.
                         i++;
                     }
                     else
                     {
                         lenc = utf8_num_bytes(&text_line[i]);
                         for (int ii = 0; ii < lenc; ii++)
                             new_line[cnt++] = text_line[i++];
                     }
                 }
                 new_line[cnt] = '\0';
                 HPDF_Page_ShowTextNextLine(page, new_line);
            }
            lines++;
          }
       }
       fclose(in_file);
    }
    else
    {
       printf("Could not open input file.");
       HPDF_Free(pdf_doc);
       return 0;
    }
    HPDF_Page_EndText(page);

    status = HPDF_PDFA_SetPDFAConformance(pdf_doc, HPDF_PDFA_1B);
    if (status != HPDF_OK)
    {
      printf("Could not convert to PDF/A.");
      return 0;
    }

    ////addColorOutputIntent(pdf);

    HPDF_SaveToFile(pdf_doc, out_file_name);
    HPDF_Free(pdf_doc);

    return 1;
}

#undef __O_TEXT3PDF__