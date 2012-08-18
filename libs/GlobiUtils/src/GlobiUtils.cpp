#include "GlobiUtils.h"

QString GlobiUtils::gAppDataFolder()
{
	return QDir::toNativeSeparators(QDir::homePath() + QDir::separator() + (APP_DATA_SUFFIX));
}

bool GlobiUtils::gAppDataFolderExists()
{
	return QDir(gAppDataFolder()).exists();
}

bool GlobiUtils::createGAppDataFolder()
{
	if(gAppDataFolderExists())
		return true;
	return QDir().mkpath(gAppDataFolder());
}

bool GlobiUtils::createProjectFolder(const QString & projectName)
{
	if(QDir(gAppDataFolder() + QDir::separator() + projectName).exists())
		return true;
	return QDir().mkpath(gAppDataFolder() + QDir::separator() + projectName);
}

QStringList GlobiUtils::existingDataFolders()
{
	return QDir(gAppDataFolder()).entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
}

QString GlobiUtils::filePathForProject(const QString & projectName)
{
	return gAppDataFolder() + QDir::separator() + projectName + QDir::separator() + projectName + CONF_EXTENSION;
}

QFile * GlobiUtils::confFileFromProject(const QString & projectName)
{
	if(!existingDataFolders().contains(projectName))
		return NULL;
	QString fileName = filePathForProject(projectName);
	if(!QFile::exists(fileName))
		return NULL;
	else
		return new QFile(fileName);
}

QMap <QString,QString> GlobiUtils::getConfFromFile(QFile * file)
{
	QMap <QString,QString> confValues;

	if(file == NULL)
		return confValues;
	if(!file->open(QIODevice::ReadOnly))
		return confValues;

	while(!file->atEnd())
	{
		QPair <QString,QString> parsedLine = parseConfLine(QString(file->readLine()).remove('\n'));
		confValues.insert(parsedLine.first,parsedLine.second);
	}
	return confValues;
}

QPair <QString,QString> GlobiUtils::parseConfLine(QString line)
{
	return QPair <QString,QString> (line.section(SEP_CHAR_CONFIG,0,0),line.section(SEP_CHAR_CONFIG,1));
}

bool GlobiUtils::writeConfFile(const QString & projectName,QMap <QString,QString> conf)
{
	if(!createProjectFolder(projectName))
		return false;

	QString fileName = filePathForProject(projectName);
	QFile confFile(fileName);

	if(confFile.open(QIODevice::WriteOnly))
	{
		QTextStream out(&confFile);
		foreach(QString key,conf.keys())
		{
			out << key;
			out << SEP_CHAR_CONFIG;
			out << conf.value(key);
			out << ENDL_CHAR_CONFIG;
		}
		confFile.close();
		return true;
	}
	return false;
}