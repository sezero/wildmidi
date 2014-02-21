/* include/config.h.  Generated from config.h.cmake by configure.  */

/* Name of package */
#define PACKAGE "wildmidi"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.mindwerks.net/projects/wildmidi/"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "https://github.com/psi29a/wildmidi/issues"

/* Define to the full name of this package. */
#define PACKAGE_NAME "WildMidi"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "WildMidi @WILDMIDI_VERSION@"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "wildmidi"

/* Define to the version of this package. */
#define PACKAGE_VERSION "@WILDMIDI_VERSION@"

/* Version number of package */
#define VERSION "@WILDMIDI_VERSION@"

/* Define this to the location of the wildmidi config file */
#define WILDMIDI_CFG "@WILDMIDI_CFG@"

/* Define if the C compiler supports the `inline' keyword. */
#cmakedefine HAVE_C_INLINE

/* Define if the compiler has the `__builtin_expect' built-in function */
#cmakedefine HAVE___BUILTIN_EXPECT

/* define this if you are running a bigendian system (motorola, sparc, etc) */
#cmakedefine WORDS_BIGENDIAN 1

/* Define our audio drivers */
#cmakedefine HAVE_ALSA_H
#cmakedefine HAVE_LINUX_SOUNDCARD_H
#cmakedefine HAVE_SYS_SOUNDCARD_H
#cmakedefine HAVE_MACHINE_SOUNDCARD_H
#cmakedefine HAVE_OPENAL_H

/* set some windows magic */
#define WIN32_LEAN_AND_MEAN
