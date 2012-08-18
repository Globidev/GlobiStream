#ifndef GLOBINARY_H
#define GLOBINARY_H

#include "globilibs_global.h"
#include <QtCore>

#define PNG_START_CHUNK "IHDR"
#define PNG_END_CHUNK "IEND"

#define PNG_HEADER_FIRST 0x89504e47
#define PNG_HEADER_SECOND 0x0D0A1A0A
#define PNG_HEADER_THIRD 0x0000000D

namespace Globinary
{
	// returns a list of arrays containing image data from the pngs found in the given file name
	GLOBILIBS_EXPORT QList <QByteArray> pngsFromBinary(const QString & fileName);

	// Returns a array of bytes containing the standard PNG Header
	GLOBILIBS_EXPORT QByteArray pngHeader();
}

#endif