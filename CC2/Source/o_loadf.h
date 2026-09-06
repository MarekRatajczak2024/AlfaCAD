
typedef enum
{
  INI_FILE =0,
  SAVE_FILE_AS,
  INI_BLOK,
  INI_PCX,
  INI_PCX2BMP,
  SAVE_PCX,
  SAVE_BMP,
  SAVE_PNG,
  SAVE_JPEG,
  SAVE_TIFF,
  SAVE_EPS,
  SAVE_PDF,
  INI_PRO,
  INI_GEO,
  SAVE_BLOK,
  CONVERT_OLD_FILE,
  CONVERT_OLD_BLOCK,
  EDIT_TABLE,
  INI_DXF,
  SAVE_DXF,
  WRITE_MAK,
  READ_MAK,
  ZAPIS_CFG,
  ODCZYT_CFG,
  ZAPIS_TXT,
  ODCZYT_TXT,
  ZAPIS_XYZ,
  ZAPIS_PRO,
  ODCZYT_PXYZ,
  OPEN_CATALOG,
  ZAPIS_PLT_CFG,
  ODCZYT_PLT_CFG,
  INI_BLOK_DXF,
  INI_DWG,
  SAVE_DWG,
  INI_BLOK_DWG,
  INI_TTF_FONT,
  INI_OTF_FONT,
  ZAPIS_OUT,
} T_Get_File_Name;

#define EXT_NO   " \n*.*"
#define EXT_FILE "Alfa Files\n*.alf"
#define EXT_BLOK "Alfa Block Files\n*.alx;*.axx"
#define EXT_BLOK_STANDARD "Standard Block Files\n*.axx"
#define EXT_IMAGE "Image Files\n*.pcx;*.png;*.bmp;*.jpg;*.jpeg;*.tiff;*.eps;*.pdf"
#define EXT_IMAGE2BMP "Image Files\n*.pcx;*.png;*.bmp;*.jpg;*.jpeg;*.tiff;*.pbm;*.pgm;*.ppm"
#define EXT_PCX "PCX Files\n*.pcx"
#define EXT_PNG "PNG Files\n*.png"
#define EXT_BMP "BMP Files\n*.bmp"
#define EXT_JPEG "JPEG Files\n*.jpg;*.jpeg"
#define EXT_TIFF "TIFF Files\n*.tiff"
#define EXT_EPS "EPS Files\n*.eps"
#define EXT_PDF "PDF Files\n*.pdf"
#define EXT_PRO "Pro Files\n*.pro"
#define EXT_GEO "Geo Files\n*.geo"
#define EXT_DXF "DXF Files\n*.dxf"
#define EXT_DWG "DWG Files\n*.dwg"
#define EXT_MAK "MACRO Files\n*.mak"
#define EXT_TMP "Temporary Files\n*.tmp"
#define EXT_CFG "Printer Params Files\n*.kdr"
#define EXT_OUT "Static analysis result Files\n*.out"
#define EXT_PLT_CFG "Plotter Params Files\n*.kpl"
#define EXT_CFGK "KPL Files\n*.kpl"
#define EXT_TXT "BLK Files\n*.blk"
#define EXT_XYZ "XYZ Files\n*.xyz.csv"
#define EXT_PXYZ "XYZ, Text Files\n*.xyz.csv;*.txt"
#ifdef LINUX
#define EXT_FONT "Font Files\n*.ttf;*.otf"
#else
#define EXT_FONT "Font Files (*.ttf;*.otf)\n*.ttf;*.otf\nAll Files (*.*)\n*.*\n"
#endif

#define EXT__NO   ""
#define EXT__FILE "alf"
#define EXT__BLOK "alx"
#define EXT__BLOK_STANDARD "axx"
#define EXT__IMAGE "bmp;png;jpeg;tiff;pcx;eps;pdf"
#define EXT__IMAGE2BMP "bmp;png;jpeg;tiff;pcx;pbm;pgm;ppm"
#define EXT__BMP "bmp"
#define EXT__PNG "png"
#define EXT__JPEG "jpeg"
#define EXT__TIFF "tiff"
#define EXT__PCX "pcx"
#define EXT__EPS "eps"
#define EXT__PDF "pdf"
#define EXT__PRO "pro"
#define EXT__GEO "geo"
#define EXT__DXF "dxf"
#define EXT__DWG "dwg"
#define EXT__MAK "mak"
#define EXT__TMP "tmp"
#define EXT__CFG "kdr"
#define EXT__OUT "out"
#define EXT__PLT_CFG "kpl"
#define EXT__CFGK "kpl"
#define EXT__TXT "blk"
#define EXT__XYZ "xyz.cvs"
#define EXT__PXYZ "txt;xyz.cvs"
#define EXT__FONT "ttf;otf"
#define EXT__INI "INI"

#ifdef __cplusplus
extern "C" {
#endif

extern BOOL Load_File       (char *, int, BOOL);

#ifdef __cplusplus
}
#endif
