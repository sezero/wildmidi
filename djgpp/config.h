#define WILDMIDI_CFG "wildmidi.cfg"

#define PACKAGE_URL "https://github.com/Mindwerks/wildmidi"
#define PACKAGE_BUGREPORT "https://github.com/Mindwerks/wildmidi/issues"

#define PACKAGE_VERSION "0.4.6"

#define HAVE_C_INLINE

#if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR >= 96)
#define HAVE___BUILTIN_EXPECT
#endif
#ifndef HAVE___BUILTIN_EXPECT
#define __builtin_expect(x,c) x
#endif

#define WM_NO_LOCK 1 /* don't need locking in MS-DOS */

#define AUDIODRV_DOSSB 1
