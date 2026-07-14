#ifdef BIT64
#ifdef ALLEGRO5
#ifdef ARM64
#define _ALFACAD_ u8"AlfaCAD (arm64)  v. 3.0"
#else
#define _ALFACAD_ u8"AlfaCAD (x64)  v. 3.0"
#endif
#else
#define _ALFACAD_ u8"AlfaCAD (x64)  v. 2.9 "
#endif
#else 
#define _ALFACAD_ u8"AlfaCAD  v. 2.9 "
#endif

#ifdef MACOS
#define PYTHON "python3"
#else
#define PYTHON "python"
#endif