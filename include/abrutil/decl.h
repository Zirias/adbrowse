#ifndef ABRUTIL_DECL_H
#define ABRUTIL_DECL_H

#ifdef __cplusplus
#define adbrowselibs___cdecl extern "C"
#else
#define adbrowselibs___cdecl
#endif

#ifdef WIN32
#define SOEXPORT adbrowselibs___cdecl __declspec(dllexport)
#define SOLOCAL
#else
#define DECLEXPORT adbrowselibs___cdecl
#if __GNUC__ >= 4
#define SOEXPORT adbrowselibs___cdecl __attribute__ ((visibility ("default")))
#define SOLOCAL __attribute__ ((visibility ("hidden")))
#else
#define SOEXPORT adbrowselibs___cdecl
#define SOLOCAL
#endif
#endif


#endif

#ifdef WIN32
#undef DECLEXPORT
#ifdef BUILDING_ABRUTIL
#define DECLEXPORT __declspec(dllexport)
#else
#define DECLEXPORT adbrowselibs___cdecl __declspec(dllimport)
#endif
#endif

