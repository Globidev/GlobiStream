#ifndef GLOBILIBS_GLOBAL_H
#define GLOBILIBS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef GLOBILIBS_LIB
# define GLOBILIBS_EXPORT Q_DECL_EXPORT
#else
# define GLOBILIBS_EXPORT Q_DECL_IMPORT
#endif

#endif // GLOBILIBS_GLOBAL_H
