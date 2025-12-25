#include <string>

#define ErrorWeWy -1
#define OkWeWy 0
#define ACADV26 2
#define ACADV10 10
#define MaxTextLen 254

typedef struct
  { char nazwa_bloku[MaxTextLen];
    char kod_obiektu;
    float x0;
    float y0;
    float z0;
	float angle;
    char invisible;
  } INS_BLOCK;  

/*------  class PISZ_GRUPA  -------------------------------------*/

class PISZ_GRUPA
{ private :
   virtual int PiszKodGrupy(int )=0;
   virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
   virtual int PiszString(char *)=0;
  public :
   PISZ_GRUPA(void);
   int PiszGrupa(int kod, int n);
   int PiszGrupa(int kod, double f);
   int PiszGrupa(int kod, char *str);
};


/*--------------------------------------------*/

/*----------  class PISZ_HEADER  -----------------------------*/

class PISZ_HEADER : virtual public PISZ_GRUPA
{ private :
   virtual int PiszKodGrupy(int )=0;
   virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
   virtual int PiszString(char *)=0;
   virtual int PiszHeader0(void)=0;
   virtual int ChangeHeader(void)=0;
  public :
   PISZ_HEADER(void);
   int PiszHeader(void);
};

/*------------------------------------------------------------*/
/*-------  class PISZ_CLASSES  -------------------------------------*/

class PISZ_CLASSES : virtual public PISZ_GRUPA
{
private:
	virtual int PiszKodGrupy(int) = 0;
	virtual int PiszInt(int) = 0;
	virtual int PiszFloat(double) = 0;
	virtual int PiszString(char *) = 0;
	//virtual int PiszImageDef(void);
public:
	PISZ_CLASSES(void);
	int PiszClasses(void);
};

/*-------  class PISZ_OBJECTSDEF  -------------------------------------*/

class PISZ_OBJECTSDEF : virtual public PISZ_GRUPA
{
private:
	virtual int PiszKodGrupy(int) = 0;
	virtual int PiszInt(int) = 0;
	virtual int PiszFloat(double) = 0;
	virtual int PiszString(char *) = 0;
	virtual int PiszImageDef(void);
public:
	PISZ_OBJECTSDEF(void);
	int PiszObjects (void);
};


/*-------  class PISZ_TABLES  -------------------------------------*/

class PISZ_TABLES : virtual public PISZ_GRUPA
{ private :
   virtual int PiszKodGrupy(int )=0;
   virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
   virtual int PiszString(char *)=0;
   virtual int PiszVport(void);
   virtual int PiszViewPorts();
   virtual int PiszLtype();
   virtual int PiszLayer();
   virtual int PiszStyle();
   virtual int PiszView();
   virtual int PiszUcs();
   virtual int PiszAppid();
   virtual int PiszDimstyle();
   virtual int PiszBlockRecord();
  public :
   PISZ_TABLES(void);
   int PiszTables(void);
};

/*-------------------------------------------------------*/



/*------  class PISZ_OBJECTS  -------------------------------------*/

class PISZ_OBJECTS : virtual public PISZ_GRUPA
{ private :
   virtual int PiszKodGrupy(int )=0;
   virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
	virtual int PiszString(char *)=0;
  protected :
        int InsBlock(INS_BLOCK ins_b1);
        int IniBlock(INS_BLOCK ins_b1);
		int RecordBlock(INS_BLOCK ins_b1);
        int EndBlock(void);
	int RysujLinia(LINIA* L);
   int RysujLinia3D(LINIA3D* L);
	int RysujText(TEXT* t);
	int RysujMText(TEXT* t);
	int RysujKolo(OKRAG* k);
    int RysujTransKolo(OKRAG* k, int translucency);
    int RysujFilledEllipse(ELLIPSE* fe);
	int RysujOkrag(OKRAG* o);
    int RysujEllipse(ELLIPSE* e);
	int RysujLuk(LUK* l);
    int RysujEllipticalArc(ELLIPTICALARC* ea);
	int RysujSolid(WIELOKAT* s);
    int RysujSolidArc(SOLIDARC* sa);
    int RysujSolid3D(WIELOKAT* s);
    int RysujSpline(SPLINE* s);
    int RysujPoint(T_Point* p, int *p_block_sufix);
    int RysujVector(AVECTOR* v, int *v_block_sufix);
    int RysujImage(B_PCX *b_pcx);
    void make_arcarrows_to_DXF(LUK *l, AVECTOR *v, double kat);
    void make_arrows_to_DXF(float x1, float y1, float x2, float y2, float x11, float y11, float x12, float y12, double angle0, AVECTOR *v, double kat);
    void draw_wave_to_DXF(double x0, double y0, double x1, double y1, double x2, double y2, double koc, double kos, double n1, double ra, AVECTOR *v);
    void draw_arrow_to_DXF(double x0, double y0, double x1, double y1, double x2, double y2, double koc1, double kos1, double koc2, double kos2, double psize, AVECTOR *v);
public  :
	PISZ_OBJECTS(void);
};

/*--------------------------------------------*/

/*-------- class PISZ_BLOCKS ------------------------------------*/

class PISZ_BLOCKS : virtual /* private */ public PISZ_OBJECTS
{ private :
   virtual int PiszKodGrupy(int )=0;
   virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
   virtual int PiszString(char *)=0;
   virtual int MyBlocks(void)=0;
  public :
  int PiszEndSec(void);
  int PiszBlocks(void);
  PISZ_BLOCKS(void);
};

/*----------------------------------------------------------------*/

/*------  class PISZ_ENTITIES  -------------------------------------*/

class PISZ_ENTITIES : virtual /* private*/ public PISZ_OBJECTS
{ private :
	virtual int PiszKodGrupy(int )=0;
	virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
   virtual int PiszString(char *)=0;
   virtual int MyEntities(void)=0;
  public  :
	PISZ_ENTITIES(void);
	int PiszEntities(void);
};

/*--------------------------------------------*/


/*----------  class PISZ_DXF  ----------------------------*/

class PISZ_DXF : public PISZ_HEADER, public PISZ_CLASSES, public PISZ_TABLES,public PISZ_BLOCKS, public PISZ_OBJECTSDEF, public PISZ_ENTITIES
{ private :
   virtual int PiszKodGrupy(int )=0;
   virtual int PiszInt(int )=0;
   virtual int PiszFloat(double )=0;
   virtual int PiszString(char *)=0;
   virtual int PiszHeader0(void)=0;
   virtual int ChangeHeader(void)=0;
   virtual int MyEntities(void)=0;
	virtual int MyBlocks(void)=0;
  public :
   int Pisz_Dxf(void);
   PISZ_DXF(void);
};

/*------------------------------------------------------------*/


/*----------  class PISZ_DXF_ASCI  ----------------------------*/

class PISZ_DXF_ASCII : public PISZ_DXF
{ private :
   virtual int PiszKodGrupy(int );
   virtual int PiszInt(int );
   virtual int PiszFloat(double );
   virtual int PiszString(char *);
   virtual int PiszHeader0(void);
   virtual int ChangeHeader(void)=0;
   virtual int MyEntities(void)=0;
   virtual int MyBlocks(void)=0;
  public :
   PISZ_DXF_ASCII(char *path);
	~PISZ_DXF_ASCII(void);
   int Pisz_Dxf_ASCII(void);
};

/*------------------------------------------------------------*/

#define UTF8STRING std::string

	typedef struct{
	//V12
	UTF8STRING dimpost;       /*!< code 3 */
	UTF8STRING dimapost;      /*!< code 4 */
/* handle are code 105 */
	UTF8STRING dimblk;        /*!< code 5, code 342 V2000+ */
	UTF8STRING dimblk1;       /*!< code 6, code 343 V2000+ */
	UTF8STRING dimblk2;       /*!< code 7, code 344 V2000+ */
	double dimscale;          /*!< code 40 */
	double dimasz;            /*!< code 41 */
	double dimexo;            /*!< code 42 */
	double dimdli;            /*!< code 43 */
	double dimexe;            /*!< code 44 */
	double dimrnd;            /*!< code 45 */
	double dimdle;            /*!< code 46 */
	double dimtp;             /*!< code 47 */
	double dimtm;             /*!< code 48 */
	double dimfxl;            /*!< code 49 V2007+ */
	double dimtxt;            /*!< code 140 */
	double dimcen;            /*!< code 141 */
	double dimtsz;            /*!< code 142 */
	double dimaltf;           /*!< code 143 */
	double dimlfac;           /*!< code 144 */
	double dimtvp;            /*!< code 145 */
	double dimtfac;           /*!< code 146 */
	double dimgap;            /*!< code 147 */
	double dimaltrnd;         /*!< code 148 V2000+ */
	int dimtol;               /*!< code 71 */
	int dimlim;               /*!< code 72 */
	int dimtih;               /*!< code 73 */
	int dimtoh;               /*!< code 74 */
	int dimse1;               /*!< code 75 */
	int dimse2;               /*!< code 76 */
	int dimtad;               /*!< code 77 */
	int dimzin;               /*!< code 78 */
	int dimazin;              /*!< code 79 V2000+ */
	int dimalt;               /*!< code 170 */
	int dimaltd;              /*!< code 171 */
	int dimtofl;              /*!< code 172 */
	int dimsah;               /*!< code 173 */
	int dimtix;               /*!< code 174 */
	int dimsoxd;              /*!< code 175 */
	int dimclrd;              /*!< code 176 */
	int dimclre;              /*!< code 177 */
	int dimclrt;              /*!< code 178 */
	int dimadec;              /*!< code 179 V2000+ */
	int dimunit;              /*!< code 270 R13+ (obsolete 2000+, use dimlunit & dimfrac) */
	int dimdec;               /*!< code 271 R13+ */
	int dimtdec;              /*!< code 272 R13+ */
	int dimaltu;              /*!< code 273 R13+ */
	int dimalttd;             /*!< code 274 R13+ */
	int dimaunit;             /*!< code 275 R13+ */
	int dimfrac;              /*!< code 276 V2000+ */
	int dimlunit;             /*!< code 277 V2000+ */
	int dimdsep;              /*!< code 278 V2000+ */
	int dimtmove;             /*!< code 279 V2000+ */
	int dimjust;              /*!< code 280 R13+ */
	int dimsd1;               /*!< code 281 R13+ */
	int dimsd2;               /*!< code 282 R13+ */
	int dimtolj;              /*!< code 283 R13+ */
	int dimtzin;              /*!< code 284 R13+ */
	int dimaltz;              /*!< code 285 R13+ */
	int dimaltttz;            /*!< code 286 R13+ */
	int dimfit;               /*!< code 287 R13+  (obsolete 2000+, use dimatfit & dimtmove)*/
	int dimupt;               /*!< code 288 R13+ */
	int dimatfit;             /*!< code 289 V2000+ */
	int dimfxlon;             /*!< code 290 V2007+ */
	UTF8STRING dimtxsty;      /*!< code 340 R13+ */
	UTF8STRING dimldrblk;     /*!< code 341 V2000+ */
	int dimlwd;               /*!< code 371 V2000+ */
	int dimlwe;               /*!< code 372 V2000+ */
} DRW_Dimstyle;
