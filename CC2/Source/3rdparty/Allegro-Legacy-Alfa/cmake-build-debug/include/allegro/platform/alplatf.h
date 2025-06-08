/* #undef ALLEGRO_LEGACY_BCC32 */
/* #undef ALLEGRO_LEGACY_BEOS */
/* #undef ALLEGRO_LEGACY_DJGPP */
/* #undef ALLEGRO_LEGACY_DMC */
/* #undef ALLEGRO_LEGACY_HAIKU */
/* #undef ALLEGRO_LEGACY_MACOSX */
/* #undef ALLEGRO_LEGACY_MINGW32 */
/* #undef ALLEGRO_LEGACY_MPW */
/* #undef ALLEGRO_LEGACY_MSVC */
/* #undef ALLEGRO_LEGACY_PSP */
/* #undef ALLEGRO_LEGACY_QNX */
#define ALLEGRO_LEGACY_UNIX
/* #undef ALLEGRO_LEGACY_WATCOM */
/* #undef WANT_ALLEGRO_5 */

#ifdef WANT_ALLEGRO_5
    #define ALLEGRO_LEGACY_A5
#endif

/* These are always defined now. */
#define ALLEGRO_LEGACY_NO_ASM
#define ALLEGRO_LEGACY_USE_C
