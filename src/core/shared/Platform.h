#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H

#ifdef WIN32

//Undefine Windows header stuff.
#undef GetCurrentTime
#undef ARRAYSIZE
#undef SendMessage

#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#define getcwd _getcwd
#define setcwd _setcwd

#define DLLEXPORT __declspec( dllexport )
#define DLLIMPORT __declspec( dllimport )

#else

#error "Not implemented"

#define DLLEXPORT __attribute__( ( visibility( "default" ) ) )
#define DLLIMPORT

#endif

/**
*	Cross-platform maximum file path length.
*/
extern const size_t MAX_PATH_LENGTH;

#endif //CORE_PLATFORM_H
