#ifdef BIT64
#ifdef ALLEGRO5
#ifdef ARM64
#define _ALFACAD_ u8"AlfaCAD (arm64)  v. 3.0 beta"
#else
#define _ALFACAD_ u8"AlfaCAD (x64)  v. 3.0 beta"
#endif
#else
#define _ALFACAD_ u8"AlfaCAD (x64)  v. 2.8 "
#endif
#else 
#define _ALFACAD_ u8"AlfaCAD  v. 2.8 "
#endif

#ifdef MACOS
#define PYTHON "python3"
#else
#define PYTHON "python"
#endif