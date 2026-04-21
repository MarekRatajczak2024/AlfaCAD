
enum ADDTYPE {ADD_MODE=0, DEL_MODE} ;


#ifdef __cplusplus
extern "C" {
#endif


extern OKNO *Get_Stretch_Window (void);
extern OKNO *Get_Block_Window (void);
extern BOOL Get_Stretch_Mode (void);
extern void Set_Stretch_Mode (BOOL);
extern int *Block_Proc_Win_Mode_Ptr (void);
extern BOOL *Block_Proc_Add_Mode_Ptr (void);
extern void Set_Block_Proc_Auto_Mode (BOOL);
extern void addrem_obiekty(int dwc, OKNO *, int (*)(void *), int (*)(void *) );
extern BOOL check_if_block_in_win (long_long, long_long, int);
//extern void blok(int (*)(void *), int (*)(void *), void (*)(void),int (*[])(void));
extern void blok(int (*DZI)(void *), int (*ODZI)(void *), void (*REDROWd)(void),const int (*COMND[])(void));
extern BOOL Block_Proc_Wez_w (int (*)(void *), int (*)(void *), void (*)(void), int (*[])(void), int info) ;
extern BOOL Block_Proc (int (*)(void *), int (*)(void *), void (*)(void), int (*[])(void)) ;
extern int blokc(int (*)(void *), int (*COMND[])(void));
extern int blokq(int (*)(void *), int (*)(void *));


#ifdef __cplusplus
}
#endif
