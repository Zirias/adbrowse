#ifndef ADBHOST_COMMON_H
#define ADBHOST_COMMON_H

#ifdef __cplusplus
#define adbhost___cdecl extern "C"
#else
#define adbhost___cdecl
#endif

#ifdef WIN32
#define SOEXPORT adbhost___cdecl __declspec(dllexport)
#ifdef BUILDING_ADBHOST
#define DECLEXPORT __declspec(dllexport)
#else
#define DECLEXPORT adbhost___cdecl __declspec(dllimport)
#endif
#define SOLOCAL
#else
#define DECLEXPORT adbhost___cdecl
#if __GNUC__ >= 4
#define SOEXPORT adbhost___cdecl __attribute__ ((visibility ("default")))
#define SOLOCAL __attribute__ ((visibility ("hidden")))
#else
#define SOEXPORT adbhost___cdecl
#define SOLOCAL
#endif
#endif


#endif
