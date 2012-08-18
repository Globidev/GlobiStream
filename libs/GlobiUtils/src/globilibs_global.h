#ifndef GLOBILIBS_GLOBAL_H
#define GLOBILIBS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(Q_WS_WIN)
#  if !defined(GLOBILIBS_EXPORT) && !defined(GLOBILIBS_IMPORT)
#    define GLOBILIBS_EXPORT
#  elif defined(GLOBILIBS_IMPORT)
#    if defined(GLOBILIBS_EXPORT)
#      undef GLOBILIBS_EXPORT
#    endif
#    define GLOBILIBS_EXPORT __declspec(dllimport)
#  elif defined(GLOBILIBS_EXPORT)
#    undef GLOBILIBS_EXPORT
#    define GLOBILIBS_EXPORT __declspec(dllexport)
#  endif
#else
#  define GLOBILIBS_EXPORT
#endif

#endif // GLOBILIBS_GLOBAL_H
