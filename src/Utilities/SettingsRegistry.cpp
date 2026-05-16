#include "stdafx.h"
#include "SettingsRegistry.h"
#include "Widgets/Settings/SettingsDialog.h"

SettingsRegistry::SettingsRegistry()
{
	m_iniPath = QApplication::applicationDirPath() + "/CALUMIMotionPrefs.ini";
	m_themePath = QApplication::applicationDirPath() + "/themes/";
	CreateDirectoryIfNDef(m_themePath);

	m_settings = new QSettings(m_iniPath, QSettings::IniFormat);
	m_dataStorage = new QSettings(QApplication::applicationDirPath() + "/appcache.ini", QSettings::IniFormat);

	m_translator = new QTranslator();
	QCoreApplication::installTranslator(m_translator);

	m_sfbgsDirectory = m_dataStorage->value("SFBGS/Directory").toString();
	m_lastDirectory = m_dataStorage->value("Default/Directory").toString();

	if (m_lastDirectory.isEmpty()) m_lastDirectory = QDir::homePath();
	if (m_sfbgsDirectory.isEmpty()) m_sfbgsDirectory = QDir::homePath();

	if (m_settings->contains("Language"))
		SetLanguage(LanguageCodeFromString(m_settings->value("Language").toString()));

	if (m_settings->contains("Sidebar"))
		SetPropertySidebarVisibilityPreference(m_settings->value("Sidebar").toInt());

	if (m_settings->contains("Log/Startup"))
		SetConsoleVisibilityPreference(m_settings->value("Log/Startup").toInt());

	if (m_settings->contains("ConvertFunctions"))
		SetConvertFunctionsSetting(m_settings->value("ConvertFunctions").toBool());

	if (m_settings->contains("UseSaveGeometry"))
		SetUseSavedGeometry(m_settings->value("UseSaveGeometry").toBool());

	if (m_settings->contains("Log/UseSaveGeometry"))
		SetUseSavedConsoledGeometry(m_settings->value("Log/UseSaveGeometry").toBool());

	{
		QStringList catList = m_settings->value("Categories", QStringList()).toStringList();
		m_categories_Default = QSet(catList.begin(), catList.end());
	}

	{
		QStringList catList = m_settings->value("SFBGS/Categories", QStringList()).toStringList();
		m_categories_SFBGS = QSet(catList.begin(), catList.end());
	}

	{
		HighlightingRule rule;

		QColor symColor = m_settings->value("Colors/Symbol").toString();
		symColor = symColor.isValid() ? symColor : m_symColorDefault;
		symColor.setAlphaF(1.0f);

		QColor decodedColor = m_settings->value("Colors/Decoded").toString();
		decodedColor = decodedColor.isValid() ? decodedColor : m_decodedColorDefault;
		decodedColor.setAlphaF(1.0f);

		m_symbolFormat.setForeground(symColor);
		rule.pattern = QRegularExpression(R"([<=>&])");
		rule.format = m_symbolFormat;
		m_highlightingRules.append(rule);

		m_decodedFormat.setForeground(decodedColor);
		rule.pattern = QRegularExpression(R"(!.*?!)");
		rule.format = m_decodedFormat;
		m_highlightingRules.append(rule);
	}

	{
		if (m_settings->contains("Theme"))
			m_themeFileName = m_settings->value("Theme").toString();
	}

}

SettingsRegistry::~SettingsRegistry()
{
	SyncSettings();
}


void SettingsRegistry::SyncSettings() const
{
	if (!m_sfbgsDirectory.isEmpty())
	{
		m_dataStorage->setValue("SFBGS/Directory", m_sfbgsDirectory);
	}

	if (!m_lastDirectory.isEmpty())
	{
		m_dataStorage->setValue("Default/Directory", m_lastDirectory);
	}

	m_settings->setValue("Language", LanguageCodeToString(m_language));

	m_settings->setValue("Sidebar", static_cast<unsigned int>(m_sidebarVis));
	m_settings->setValue("Log/Startup", static_cast<unsigned int>(m_consoleVis));
		
	m_settings->setValue("ConvertFunctions", m_convertFunctions);
	m_settings->setValue("Log/UseSaveGeometry", m_rememberConsoleGeometry);
	m_settings->setValue("UseSaveGeometry", m_rememberMainGeometry);

	if(!m_categories_Default.isEmpty())
		m_settings->setValue("Categories", m_categories_Default.values());
	
	if(!m_categories_SFBGS.isEmpty())
		m_settings->setValue("SFBGS/Categories", m_categories_SFBGS.values());
	
	m_dataStorage->setValue("LastVersion", QCoreApplication::applicationVersion());

	
	m_settings->setValue("Colors/Symbol", GetSymbolColor().name());
	
	m_settings->setValue("Colors/Decoded", GetDecodedColor().name());

	m_settings->setValue("Theme", m_themeFileName);

	m_settings->sync();
	m_dataStorage->sync();
}

QByteArray SettingsRegistry::GetSavedWindowGeometry(const QString& key) const
{
	if (m_dataStorage->contains(key))
		return m_dataStorage->value(key).toByteArray();

	return {};
}

void SettingsRegistry::SaveWindowGeometry(const QString& key, const QByteArray& array) const
{
	m_dataStorage->setValue(key, array);
	m_dataStorage->sync();
}

bool SettingsRegistry::GetLastState(const QString& key, const bool defValue) const
{
	if (m_dataStorage->contains(key))
		return m_dataStorage->value(key).toBool();

	return defValue;
}

void SettingsRegistry::SaveLastState(const QString& key, const bool state) const
{
	m_dataStorage->setValue(key, state);
	m_dataStorage->sync();
}

QString SettingsRegistry::GetSavedVersion() const
{
	if (m_dataStorage->contains("LastVersion"))
		return m_dataStorage->value("LastVersion").toString();

	return {};
}

QString SettingsRegistry::IniPath() const
{
	return m_iniPath;
}

bool SettingsRegistry::CreateDirectoryIfNDef(const QString& fileString)
{
	const QString dirPath = QFileInfo(fileString).absolutePath();

	if (const QDir dir; !dir.mkpath(dirPath))
	{
		qCritical() << "Could not create directory path: " << dirPath;
		return false;
	}

	qInfo() << "Directory found: " << dirPath;
	return true;
}

void SettingsRegistry::SetSymbolColor(const QColor color)
{
	m_highlightingRules[0].format.setForeground(color);
}

const QColor& SettingsRegistry::GetSymbolColor() const
{
	return m_highlightingRules.at(0).format.foreground().color();
}

void SettingsRegistry::SetDecodedColor(const QColor color)
{
	m_highlightingRules[1].format.setForeground(color);
}

const QColor& SettingsRegistry::GetDecodedColor() const
{
	return m_highlightingRules.at(1).format.foreground().color();
}

QString SettingsRegistry::LastDirectory(const AgxGameType type) const
{
	switch (type)
	{
		case AgxGameType::SFBGS:
			return m_sfbgsDirectory;
		default:
			return m_lastDirectory;
	}
}

void SettingsRegistry::SetLastDirectory(const QString& dir, const AgxGameType type)
{
	const QFileInfo info(dir);
	
	if (!info.exists()) return;

	const QString path = info.isFile() ? info.path() : dir;

	switch (type)
	{
		case AgxGameType::SFBGS:
			m_sfbgsDirectory = path;
			break;
		default:
			break;
	}

	m_lastDirectory = path;
}

QString SettingsRegistry::GetRelativeDataPath(const AgxGameType type)
{
	QString path;
	if(!m_settings) return path;
	
	switch (type)
	{
		case AgxGameType::SFBGS: {	
			path = m_settings->contains("SFBGS/GraphRelativePath") ? m_settings->value("SFBGS/GraphRelativePath").toString() : m_defaultRelDataPath_SFBGS;
			break;
		default:
			break;
		}
	}

	return path;
}

void SettingsRegistry::SaveRelativeDataPath(const AgxGameType type, const QString& pathToSave) const
{
	if (!m_settings) return;

	switch (type)
	{
		case AgxGameType::SFBGS:
			if(pathToSave.compare(R"(Data\Meshes\AnimTextData\Tables\Graphs\)") != 0)
				m_settings->setValue("SFBGS/GraphRelativePath", pathToSave);
			break;
		default:
			break;
	}
}

void SettingsRegistry::SetLanguage(const LanguageCode code)
{
	if (code == m_language) return;

	const QString prefixPath = QApplication::applicationDirPath() +"/";
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
		case LanguageCode::Ukrainian:
			file = "motion_uk.qm";
			break;
		default:
			SetDefaultLanguage();
			return;
	}

	//_translator = new QTranslator();
	if (m_translator->load(prefixPath + file)) {
		//QCoreApplication::installTranslator(_translator);
		m_language = code;
		qInfo() << QObject::tr("Language Changed To: %1 (%2)").arg(LanguageCodeToString(code)).arg(file);
	} else {
		SetDefaultLanguage();
	}

}

QSet<QString> SettingsRegistry::GetCustomCategories(const AgxGameType game, const bool withDefault) const
{
	QSet<QString> customList;

	if (withDefault)
	{
		customList.unite(m_categories_Default);
	}

	switch (game)
	{
		case AgxGameType::SFBGS:
			customList.unite(m_categories_SFBGS);
			break;
		default:
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

void SettingsRegistry::AddCustomCategory(const QString& item, const AgxGameType game)
{
	switch (game)
	{
		case AgxGameType::SFBGS:
			m_categories_SFBGS.insert(item);
			break;
		default:
			m_categories_Default.insert(item);
			break;
	}
}

QString SettingsRegistry::GetThemeFilePath() const
{
	return m_themePath + m_themeFileName;
}

QString SettingsRegistry::GetThemeFileName() const
{
	return m_themeFileName;
}

void SettingsRegistry::SetThemeFileName(const QString& path)
{
	const QFileInfo file(path);

	if(const auto fileName = file.fileName(); m_themeFileName != fileName)
	{
		m_themeFileName = fileName;
		qInfo() << "Theme Change Requires Application Restart!";
	}
}

void SettingsRegistry::SetDefaultLanguage()
{
	m_language = LanguageCode::English;
	qInfo() << QObject::tr("Language Changed To: %1").arg(LanguageCodeToString(m_language));
	
	QCoreApplication::removeTranslator(m_translator);
	delete m_translator;

	m_translator = new QTranslator();
	QCoreApplication::installTranslator(m_translator);
}

static StartupVisibilityPreference Helper_GetVisibilitySafe(const int pref)
{
	switch (pref)
	{
		case 0:
			return StartupVisibilityPreference::Never;
		case 2:
			return StartupVisibilityPreference::Always;
		default:
			return StartupVisibilityPreference::Remember;
	}
}

void SettingsRegistry::SetPropertySidebarVisibilityPreference(const int pref)
{
	SetPropertySidebarVisibilityPreference(Helper_GetVisibilitySafe(pref));
}

void SettingsRegistry::SetConsoleVisibilityPreference(const int pref)
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
	auto index = LanguageStrings.indexOf(code, 0LL, Qt::CaseInsensitive);

	//if we don't find the string in the language string list we compare to some backups
	if (index < 0) {

		if (code.compare("Spanish", Qt::CaseInsensitive) == 0 || code.compare("ES", Qt::CaseInsensitive) == 0)
			return LanguageCode::Spanish;
		if (code.compare("French", Qt::CaseInsensitive) == 0 || code.compare("FR", Qt::CaseInsensitive) == 0)
			return LanguageCode::French;
		if (code.compare("German", Qt::CaseInsensitive) == 0 || code.compare("DE", Qt::CaseInsensitive) == 0 || code.compare("GER", Qt::CaseInsensitive) == 0 || code.compare("DEU", Qt::CaseInsensitive) == 0)
			return LanguageCode::German;
		if (code.compare("Ukrainian", Qt::CaseInsensitive) == 0 || code.compare("UK", Qt::CaseInsensitive) == 0 || code.compare("UKR", Qt::CaseInsensitive) == 0)
			return LanguageCode::Ukrainian;
		if (code.compare("Russian", Qt::CaseInsensitive) == 0 || code.compare("RU", Qt::CaseInsensitive) == 0 || code.compare("RUS", Qt::CaseInsensitive) == 0)
			return LanguageCode::Russian;

		return LanguageCode::English;
	}

	if (static_cast<unsigned int>(LanguageCode::Max) < index) {
		return LanguageCode::English;
	}

	return static_cast<LanguageCode>(index);
}
