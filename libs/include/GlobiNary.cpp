#include "GlobiNary.h"

QList <QByteArray> Globinary::pngsFromBinary(const QString & fileName)
{
	QFile in(fileName);
	QList <QByteArray> pngsData;
	
	if(in.open(QIODevice::ReadOnly))
	{
		QByteArray data = in.readAll();
		int from = 0;

		while((from = data.indexOf(PNG_START_CHUNK,from)) != -1)
		{
			int end = data.indexOf(PNG_END_CHUNK,from) + QString(PNG_END_CHUNK).size();
			if(end != -1)
			{
				QByteArray pngData = pngHeader();

				for(int i = from; i < end; i ++)
					pngData += data.at(i);

				if(!pngData.isEmpty())
					pngsData.append(pngData);
			}
			from ++;
		}
		in.close();
	}
	return pngsData;
}

QByteArray Globinary::pngHeader()
{
	QByteArray out;
	QDataStream headerStream(&out, QIODevice::WriteOnly);

	headerStream << PNG_HEADER_FIRST << PNG_HEADER_SECOND << PNG_HEADER_THIRD;

	return out;
}