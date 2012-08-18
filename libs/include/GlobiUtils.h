#ifndef GLOBIUTILS_H
#define GLOBIUTILS_H

#include "globilibs_global.h"
#include <QtCore>

#define APP_DATA_SUFFIX "AppData/Roaming/.GlobiData"
#define CONF_EXTENSION ".gconf"
#define SEP_CHAR_CONFIG '='
#define ENDL_CHAR_CONFIG '\n'

namespace GlobiUtils
{
	// Returns a string to the standard Globi App Data folder path
	GLOBILIBS_EXPORT QString gAppDataFolder();

	// Returns true if the standard Globi App Data folder exists, false otherwise
	GLOBILIBS_EXPORT bool gAppDataFolderExists();

	// Tries to create the Globi App Data folder, returns true if successful or if already exists, false otherwise
	GLOBILIBS_EXPORT bool createGAppDataFolder();

	// Tries to create the Project floder in GAppData, returns true if successful or already exists, false otherwise
	GLOBILIBS_EXPORT bool createProjectFolder(const QString & projectName);

	// Returns a list of Project folders in GAppData
	GLOBILIBS_EXPORT QStringList existingDataFolders();

	// Returns the path to the project configuration file of the given project name
	GLOBILIBS_EXPORT QString filePathForProject(const QString & projectName);

	// Returns a Qt-Style File Handle to the configuration file of the given project name, NULL if doesn't exist
	GLOBILIBS_EXPORT QFile * confFileFromProject(const QString & projectName);

	// Returns a map of configurations for the given File
	GLOBILIBS_EXPORT QMap <QString,QString> getConfFromFile(QFile * file);

	// Parses a line of a GConf file, returning a pair of strings ( Attribute, Value )
	GLOBILIBS_EXPORT QPair <QString,QString> parseConfLine(QString line);

	// Tries to write the given map of ( String, String ) in the GConf file of the given project, returns true if successful, false otherwise
	GLOBILIBS_EXPORT bool writeConfFile(const QString & projectName,QMap <QString,QString> conf);
}

#endif