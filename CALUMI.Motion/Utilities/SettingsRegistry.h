#pragma once

#pragma warning(push,0)

#pragma warning(pop)

#include "Utilities/AgxDefinitions.h"
#include <QSettings>

#pragma region Definitions

enum class LanguageCode : unsigned int
{
	English,
	Spanish,
	French,
	German,
	Russian,
	Ukranian,
	
	Max = Ukranian
};

static inline QStringList LanguageStrings = {
	"English",
	"Español",
	"Français",
	"Deutsch",
	"Русский",
	"Yкраї́нська"
};

QString LanguageCodeToString(LanguageCode code = LanguageCode::English);
LanguageCode LanguageCodeFromString(const QString& code = "English");

enum class StartupVisibiltyPreference : uint8_t {
	Never,
	Remember,
	Always
};

#pragma endregion

class SettingsDialog;

class SettingsRegistry
{
private:
	SettingsRegistry();
	~SettingsRegistry();

public:
	SettingsRegistry(const SettingsRegistry&) = delete;
	SettingsRegistry& operator=(const SettingsRegistry&) = delete;

	SettingsRegistry(SettingsRegistry&&) = delete;
	SettingsRegistry& operator=(SettingsRegistry&&) = delete;

	static SettingsRegistry& GetInstance() {
		static SettingsRegistry instance;
		return instance;
	}

	QString LastDirectory(AgxGameType type = AgxGameType::None) const;
	void SetLastDirectory(const QString& dir, AgxGameType type = AgxGameType::None);

	QString GetRelativeDataPath(AgxGameType type);
	void SaveRelativeDataPath(AgxGameType type, const QString& pathToSave);

	void SetLanguage(LanguageCode code);
	inline LanguageCode GetLanguage() const { return _language; }

	inline StartupVisibiltyPreference GetPropertySidebarVisibilityPreference() const { return _sidebarVis; }
	inline void SetPropertySidebarVisibilityPreference(StartupVisibiltyPreference pref) { _sidebarVis = pref; }
	void SetPropertySidebarVisibilityPreference(int pref);

	inline StartupVisibiltyPreference GetConsoleVisibilityPreference() const { return _consoleVis; }
	inline void SetConsoleVisibilityPreference(StartupVisibiltyPreference pref) { _consoleVis = pref; }
	void SetConsoleVisibilityPreference(int pref);

	inline bool GetConvertFunctionsSetting() const { return _convertFunctions; }
	inline void SetConvertFunctionsSetting(bool setting) { _convertFunctions = setting; }

	void SyncSettings() const;

	QByteArray GetSavedWindowGeometry(const QString& key) const;
	void SaveWindowGeometry(const QString& key, const QByteArray& array);

	bool GetLastState(const QString& key, bool defValue = true) const;
	void SaveLastState(const QString& key, bool state);

	QString GetSavedVersion();

	inline bool UseSavedGeometry() const { return _rememberMainGeometry; }
	inline void SetUseSavedGeometry(bool setting) { _rememberMainGeometry = setting; }

	inline bool UseSavedConsoledGeometry() const { return _rememberConsoleGeometry; }
	inline void SetUseSavedConsoledGeometry(bool setting) { _rememberConsoleGeometry = setting; }

	QSet<QString> GetCustomCategories(AgxGameType game = AgxGameType::SFBGS, bool withDefault = true) const;
	void AddCustomCatgeory(const QString& item, AgxGameType game = AgxGameType::SFBGS);

private:
	void SetDefaultLanguage();
	QString IniPath() const;

	//Generic
private:
	QString _iniPath;
	QSettings* _settings = nullptr;
	QSettings* _dataStorage = nullptr;
	QString _lastDirectory;
	QString _sfbgsDirectory;

	QPointer<QTranslator> _translator = nullptr;
	LanguageCode _language = LanguageCode::English;

	StartupVisibiltyPreference _sidebarVis = StartupVisibiltyPreference::Remember;
	bool _sidebarLastState = true;

	StartupVisibiltyPreference _consoleVis = StartupVisibiltyPreference::Remember;
	bool _consoleLastState = true;

	bool _convertFunctions = true;
	bool _rememberMainGeometry = true;
	bool _rememberConsoleGeometry = true;

	QSet<QString> _categories_Default;

	//SFBGS
private:
	QString _defaultRelDataPath_SFBGS = R"(Data\Meshes\AnimTextData\Tables\Graphs\)";
	QSet<QString> _categories_SFBGS;

};