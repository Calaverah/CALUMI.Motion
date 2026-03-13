#include "stdafx.h"
#include "SettingsRegistry.h"
#include "Widgets/Settings/SettingsDialog.h"

SettingsRegistry::SettingsRegistry()
{
	_iniPath = QApplication::applicationDirPath() + "/CALUMIMotionPrefs.ini";
	_themePath = QApplication::applicationDirPath() + "/themes/";
	CreateDirectoryIfNDef(_themePath);

	_settings = new QSettings(_iniPath, QSettings::IniFormat);
	_dataStorage = new QSettings(QApplication::applicationDirPath() + "/appcache.ini", QSettings::IniFormat);

	_translator = new QTranslator();
	QCoreApplication::installTranslator(_translator);

	_sfbgsDirectory = _dataStorage->value("SFBGS/Directory").toString();
	_lastDirectory = _dataStorage->value("Default/Directory").toString();

	if (_lastDirectory.isEmpty()) _lastDirectory = QDir::homePath();
	if (_sfbgsDirectory.isEmpty()) _sfbgsDirectory = QDir::homePath();

	if (_settings->contains("Language"))
		SetLanguage(LanguageCodeFromString(_settings->value("Language").toString()));

	if (_settings->contains("Sidebar"))
		SetPropertySidebarVisibilityPreference(_settings->value("Sidebar").toInt());

	if (_settings->contains("Log/Startup"))
		SetConsoleVisibilityPreference(_settings->value("Log/Startup").toInt());

	if (_settings->contains("ConvertFunctions"))
		SetConvertFunctionsSetting(_settings->value("ConvertFunctions").toBool());

	if (_settings->contains("UseSaveGeometry"))
		SetUseSavedGeometry(_settings->value("UseSaveGeometry").toBool());

	if (_settings->contains("Log/UseSaveGeometry"))
		SetUseSavedConsoledGeometry(_settings->value("Log/UseSaveGeometry").toBool());

	{
		QStringList catList = _settings->value("Categories", QStringList()).toStringList();
		_categories_Default = QSet<QString>(catList.begin(), catList.end());
	}

	{
		QStringList catList = _settings->value("SFBGS/Categories", QStringList()).toStringList();
		_categories_SFBGS = QSet<QString>(catList.begin(), catList.end());
	}

	{
		HighlightingRule rule;

		QColor symColor = _settings->value("Colors/Symbol").toString();
		symColor = symColor.isValid() ? symColor : _symColorDefault;
		symColor.setAlphaF(1.0f);

		QColor decodedColor = _settings->value("Colors/Decoded").toString();
		decodedColor = decodedColor.isValid() ? decodedColor : _decodedColorDefault;
		decodedColor.setAlphaF(1.0f);

		_symbolFormat.setForeground(symColor);
		rule.pattern = QRegularExpression(R"([<=>&])");
		rule.format = _symbolFormat;
		_highlightingRules.append(rule);

		_decodedFormat.setForeground(decodedColor);
		rule.pattern = QRegularExpression(R"(!.*?!)");
		rule.format = _decodedFormat;
		_highlightingRules.append(rule);
	}

	{
		if (_settings->contains("Theme"))
			_themeFileName = _settings->value("Theme").toString();
	}

}

SettingsRegistry::~SettingsRegistry()
{
	SyncSettings();
}


void SettingsRegistry::SyncSettings() const
{
	if (!_sfbgsDirectory.isEmpty())
	{
		_dataStorage->setValue("SFBGS/Directory", _sfbgsDirectory);
	}

	if (!_lastDirectory.isEmpty())
	{
		_dataStorage->setValue("Default/Directory", _lastDirectory);
	}

	_settings->setValue("Language", LanguageCodeToString(_language));

	_settings->setValue("Sidebar", static_cast<unsigned int>(_sidebarVis));
	_settings->setValue("Log/Startup", static_cast<unsigned int>(_consoleVis));
		
	_settings->setValue("ConvertFunctions", _convertFunctions);
	_settings->setValue("Log/UseSaveGeometry", _rememberConsoleGeometry);
	_settings->setValue("UseSaveGeometry", _rememberMainGeometry);

	if(!_categories_Default.isEmpty())
		_settings->setValue("Categories", _categories_Default.values());
	
	if(!_categories_SFBGS.isEmpty())
		_settings->setValue("SFBGS/Categories", _categories_SFBGS.values());
	
	_dataStorage->setValue("LastVersion", QCoreApplication::applicationVersion());

	
	_settings->setValue("Colors/Symbol", GetSymbolColor().name());
	
	_settings->setValue("Colors/Decoded", GetDecodedColor().name());

	_settings->setValue("Theme", _themeFileName);

	_settings->sync();
	_dataStorage->sync();
}

QByteArray SettingsRegistry::GetSavedWindowGeometry(const QString& key) const
{
	if (_dataStorage->contains(key))
		return _dataStorage->value(key).toByteArray();

	return QByteArray();
}

void SettingsRegistry::SaveWindowGeometry(const QString& key, const QByteArray& array)
{
	_dataStorage->setValue(key, array);
	_dataStorage->sync();
}

bool SettingsRegistry::GetLastState(const QString& key, bool defValue) const
{
	if (_dataStorage->contains(key))
		return _dataStorage->value(key).toBool();

	return defValue;
}

void SettingsRegistry::SaveLastState(const QString& key, bool state)
{
	_dataStorage->setValue(key, state);
	_dataStorage->sync();
}

QString SettingsRegistry::GetSavedVersion()
{
	if (_dataStorage->contains("LastVersion"))
		return _dataStorage->value("LastVersion").toString();

	return QString();
}

QString SettingsRegistry::IniPath() const
{
	return _iniPath;
}

bool SettingsRegistry::CreateDirectoryIfNDef(const QString& fileString)
{
	QString dirPath = QFileInfo(fileString).absolutePath();

	QDir dir;
	if (!dir.mkpath(dirPath))
	{
		qCritical() << "Could not create directory path: " << dirPath;
		return false;
	}

	qInfo() << "Directory found: " << dirPath;
	return true;
}

void SettingsRegistry::SetSymbolColor(QColor color)
{
	_highlightingRules[0].format.setForeground(color);
}

const QColor& SettingsRegistry::GetSymbolColor() const
{
	return _highlightingRules.at(0).format.foreground().color();
}

void SettingsRegistry::SetDecodedColor(QColor color)
{
	_highlightingRules[1].format.setForeground(color);
}

const QColor& SettingsRegistry::GetDecodedColor() const
{
	return _highlightingRules.at(1).format.foreground().color();
}

QString SettingsRegistry::LastDirectory(AgxGameType type) const
{
	switch (type)
	{
		case AgxGameType::SFBGS:
			return _sfbgsDirectory;
		default:
			return _lastDirectory;
	}
}

void SettingsRegistry::SetLastDirectory(const QString& dir, AgxGameType type)
{
	QFileInfo info(dir);
	
	if (!info.exists()) return;

	QString path = info.isFile() ? info.path() : dir;

	switch (type)
	{
		case AgxGameType::SFBGS:
			_sfbgsDirectory = path;
			break;
	}

	_lastDirectory = path;
}

QString SettingsRegistry::GetRelativeDataPath(AgxGameType type)
{
	QString path;
	if(!_settings) return path;
	
	switch (type)
	{
		case AgxGameType::SFBGS: {	
			path = _settings->contains("SFBGS/GraphRelativePath") ? _settings->value("SFBGS/GraphRelativePath").toString() : _defaultRelDataPath_SFBGS;
			break;
		}
	}

	return path;
}

void SettingsRegistry::SaveRelativeDataPath(AgxGameType type, const QString& pathToSave)
{
	if (!_settings) return;

	switch (type)
	{
		case AgxGameType::SFBGS:
			if(pathToSave.compare(R"(Data\Meshes\AnimTextData\Tables\Graphs\)") != 0)
				_settings->setValue("SFBGS/GraphRelativePath", pathToSave);
			break;
	}
}

void SettingsRegistry::SetLanguage(LanguageCode code)
{
	if (code == _language) return;

	QString prefixPath = ":/CALUMIMotion/Localization/";
	QString file;


	switch (code)
	{
		case LanguageCode::Spanish:
			file = "motion_es.qm";
			break;
		case LanguageCode::French:
			file = "motion_fr.qm";
			break;
		case LanguageCode::German:
			file = "motion_de.qm";
			break;
		case LanguageCode::Russian:
			file = "motion_ru.qm";
			break;
		case LanguageCode::Ukranian:
			file = "motion_uk.qm";
			break;
		default:
			SetDefaultLanguage();
			return;
	}

	//_translator = new QTranslator();
	if (_translator->load(prefixPath + file)) {
		//QCoreApplication::installTranslator(_translator);
		_language = code;
		qInfo() << QObject::tr("Language Changed To: %1 (%2)").arg(LanguageCodeToString(code)).arg(file);
	} else {
		SetDefaultLanguage();
	}

}

QSet<QString> SettingsRegistry::GetCustomCategories(AgxGameType game, bool withDefault) const
{
	QSet<QString> customList;

	if (withDefault)
	{
		customList.unite(_categories_Default);
	}

	switch (game)
	{
		case AgxGameType::SFBGS:
			customList.unite(_categories_SFBGS);
			break;
	}

	QString none;

	for (auto& string : customList) {
		if (string.compare("none", Qt::CaseInsensitive) == 0 || string.compare("<none>", Qt::CaseInsensitive) == 0) {
			none = string;
		}
	}

	if (!none.isEmpty()) {
		customList.remove(none);
	}

	return customList;
}

void SettingsRegistry::AddCustomCatgeory(const QString& item, AgxGameType game)
{
	switch (game)
	{
		case AgxGameType::SFBGS:
			_categories_SFBGS.insert(item);
			break;
		default:
			_categories_Default.insert(item);
			break;
	}
}

QString SettingsRegistry::GetThemeFilePath() const
{
	return _themePath + _themeFileName;
}

QString SettingsRegistry::GetThemeFileName() const
{
	return _themeFileName;
}

void SettingsRegistry::SetThemeFileName(const QString& path)
{
	QFileInfo file(path);
	
	auto fileName = file.fileName();
	
	if(_themeFileName != fileName)
	{
		_themeFileName = fileName;
		qInfo() << "Theme Change Requires Application Restart!";
	}
}

void SettingsRegistry::SetDefaultLanguage()
{
	_language = LanguageCode::English;
	qInfo() << QObject::tr("Language Changed To: %1").arg(LanguageCodeToString(_language));
	
	QCoreApplication::removeTranslator(_translator);
	delete _translator;

	_translator = new QTranslator();
	QCoreApplication::installTranslator(_translator);
}

static StartupVisibiltyPreference Helper_GetVisibilitySafe(int pref)
{
	switch (pref)
	{
		case 0:
			return StartupVisibiltyPreference::Never;
		case 2:
			return StartupVisibiltyPreference::Always;
		default:
			return StartupVisibiltyPreference::Remember;
	}
}

void SettingsRegistry::SetPropertySidebarVisibilityPreference(int pref)
{
	SetPropertySidebarVisibilityPreference(Helper_GetVisibilitySafe(pref));
}

void SettingsRegistry::SetConsoleVisibilityPreference(int pref)
{
	SetConsoleVisibilityPreference(Helper_GetVisibilitySafe(pref));
}

QString LanguageCodeToString(LanguageCode code)
{
	if (LanguageStrings.size() <= static_cast<unsigned int>(code)) return "Unknown Language";

	return LanguageStrings.at(static_cast<unsigned int>(code));
}

LanguageCode LanguageCodeFromString(const QString& code)
{
	auto index = LanguageStrings.indexOf(code, 0i64, Qt::CaseInsensitive);

	//if we don't find the string in the language string list we compare to some backups
	if (index < 0) {

		if (code.compare("Spanish", Qt::CaseInsensitive) == 0 || code.compare("ES", Qt::CaseInsensitive) == 0)
			return LanguageCode::Spanish;
		if (code.compare("French", Qt::CaseInsensitive) == 0 || code.compare("FR", Qt::CaseInsensitive) == 0)
			return LanguageCode::French;
		if (code.compare("German", Qt::CaseInsensitive) == 0 || code.compare("DE", Qt::CaseInsensitive) == 0 || code.compare("GER", Qt::CaseInsensitive) == 0 || code.compare("DEU", Qt::CaseInsensitive) == 0)
			return LanguageCode::German;
		if (code.compare("Ukranian", Qt::CaseInsensitive) == 0 || code.compare("UK", Qt::CaseInsensitive) == 0 || code.compare("UKR", Qt::CaseInsensitive) == 0)
			return LanguageCode::Ukranian;
		if (code.compare("Russian", Qt::CaseInsensitive) == 0 || code.compare("RU", Qt::CaseInsensitive) == 0 || code.compare("RUS", Qt::CaseInsensitive) == 0)
			return LanguageCode::Russian;

		return LanguageCode::English;

	}else if (static_cast<unsigned int>(LanguageCode::Max) < index) {
		return LanguageCode::English;
	}

	return static_cast<LanguageCode>(index);
}

//QString StartupVisibilityToString(StartupVisibiltyPreference pref)
//{
//	if (StartupVisibilityStrings.size() <= static_cast<unsigned int>(pref)) return "Remember";
//
//	return StartupVisibilityStrings.at(static_cast<unsigned int>(pref));
//}
//
//StartupVisibiltyPreference StartupVisibilityFromString(const QString& pref)
//{
//	auto index = LanguageStrings.indexOf(pref, 0i64, Qt::CaseInsensitive);
//
//	//if we don't find the string in the language string list we compare to some backups
//	if (index < 0) {
//
//		if (pref.compare("Always", Qt::CaseInsensitive) == 0)
//			return StartupVisibiltyPreference::Always;
//		if (pref.compare("Never", Qt::CaseInsensitive) == 0)
//			return StartupVisibiltyPreference::Never;
//
//		return StartupVisibiltyPreference::Remember;
//
//	} else if (static_cast<unsigned int>(LanguageCode::Max) < index) {
//		return StartupVisibiltyPreference::Remember;
//	}
//
//	return static_cast<StartupVisibiltyPreference>(index);
//}
