#ifdef __cplusplus
extern "C" {
#endif

extern void outtext_r_e (int , int , int , int, char *, BOOL, BOOL, int, int, int, int y_4_);
extern int Get_Key (void);
extern int Get_Legal_Key (char *);
extern int getkey(void);
extern int editstring(unsigned char *s, char *legal, int maxlength, float width0, BOOL b_graph_value, int expand, BOOL last_edit, int y_4_, int y_3_);
extern int get_string (char *, char *, int , int , int );
extern int read_esc1 (char *tekst,int lmax,int kom);
extern int  read_esc(char *tekst,int lmax,int kom);
extern int read_esc_legal (char *tekst, char *legal, int lmax,int kom);
extern void outetextxy (int x, int y, int maxlength, int width, char *s,int ink, int paper);

#ifdef __cplusplus
}
#endif
