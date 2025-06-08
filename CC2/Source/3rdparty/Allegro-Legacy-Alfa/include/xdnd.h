/*         ______   ___    ___
 *        /\  _  \ /\_ \  /\_ \
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      X header file for the Allegro library.
 *
 *      This prototypes some things which might be useful to 
 *      the calling application, but you don't need it.
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* X11 XDND result structure*/
typedef struct
{
    int mflag;
    int iflag;
    char filebuf[4192];  //up to 16 files * (260+2)
} _xdnd_struct;

extern _xdnd_struct xdnd_buf;


#ifdef __cplusplus
   }
#endif


