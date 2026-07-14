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

#pragma warning(disable : 4996)
#ifndef ALLEGRO_AND_MFC
#define ALLEGRO_AND_MFC  //??????????????
#endif

#define FORWIN32

#define TRUE  1
#define FALSE 0

#define MAXHISTORYFILES 16
#define MAXPATH   260
#define MAXDRIVE  3
#define MAXDIR	  256
#define MAXFILE   256
#define MAXEXT	  255

#define WILDCARDS 0x01
#define EXTENSION 0x02
#define FILENAME  0x04
#define DIRECTORY 0x08
#define DRIVE	  0x10 

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2	1.57079632679489661923
#endif
#ifndef M_PI_4
#define M_PI_4	0.78539816339744830962
#endif
#ifndef M_1_PI
#define M_1_PI	0.31830988618379067154
#endif
#ifndef M_2_PI
#define M_2_PI	0.63661977236758134308
#endif
#ifndef PI
#define PI	M_PI
#endif
#ifndef PI2
#define PI2	M_PI_2
#endif

#define MAXDOUBLE   1.79769313486231570e+308 
#ifndef MAXLONG
#define MAXLONG     ((long)0x7fffffff)
#endif
#ifndef MAXINT
#define MAXINT      ((int)0x7fffffff)
#endif

struct ffblk {
  char lfn_magic[6]; // __attribute__((packed));			/* LFN */
  short lfn_handle; // __attribute__((packed));			/* LFN */
  unsigned short lfn_ctime; // __attribute__((packed));		/* LFN */
  unsigned short lfn_cdate; // __attribute__((packed));		/* LFN */
  unsigned short lfn_atime; // __attribute__((packed));		/* LFN */
  unsigned short lfn_adate; // __attribute__((packed));		/* LFN */
  char _ff_reserved[5]; // __attribute__((packed));
  unsigned char  ff_attrib; // __attribute__((packed));
  unsigned short ff_ftime; // __attribute__((packed));
  unsigned short ff_fdate; // __attribute__((packed));
  unsigned long  ff_fsize; // __attribute__((packed));
  char ff_name[260]; // __attribute__((packed));
};
