#ifdef __cplusplus
extern "C" {
#endif

extern void ClearInfo(void);
extern void InfoList(int n);
extern void InfoListStr(int, char *) ;
extern void ClearErr(void);
extern void ErrList(int n);
extern void ErrListStr(char *st);
extern char *get_komunikat_ptr (int n);
extern void Get_Current_Pos (int *x, int *y);
extern void Set_Current_Pos (void);
extern void komunikat(int n);
extern void Add_String_To_List1 (char *ptr_string);
extern void komunikat_str(char *st);
extern void komunikat_str_len(char *st);
extern void komunikat0(int n);
extern void komunikat0_str (int n, char *st);
extern void  Internal_Error (int line, char * file);
extern void layer_info (void);

#ifdef __cplusplus
}
#endif

