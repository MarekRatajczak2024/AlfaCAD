
#ifdef __cplusplus
extern "C" {
#endif

extern int ResetMouse(void);
extern void ShowMouse (void);
extern void HideMouse (void);
extern void SetPosMouse (int x, int y);
extern void SetMinMaxXMouse (int xmin, int xmax);
extern void SetMinMaxYMouse (int ymin, int ymax);
extern void GetCurPos( int *x, int *y);
extern BOOL Ini_Mouse_Cursor (int, int) ;
extern void Free_Mem_Mouse_Cur (void) ;
extern void Hide_Mouse (void) ;
extern void Show_Mouse (void) ;
extern void Move_Mouse (int, int) ;

#define MV_MAX 10
typedef struct
{
    int no;
    int x[MV_MAX];
    int y[MV_MAX];
    int sum;
} MVECTOR;

#ifdef __cplusplus
}
#endif

