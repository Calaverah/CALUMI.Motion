// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#pragma once

#include "Utilities/AgxDefinitions.h"
#pragma warning(push,0)
#include <QTranslator>
#include <QSettings>
#include <QPointer>
#include <QTextCharFormat>
#pragma warning(pop)


#pragma region Definitions

enum class LanguageCode : unsigned int
{
	English,
	Spanish,
	French,
	German,
	Russian,
	Ukrainian,
	
	Max = Ukrainian
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

enum class StartupVisibilityPreference : uint8_t {
	Never,
	Remember,
	Always
};

#pragma endregion

class SettingsDialog;

class SettingsRegistry
{
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

	[[nodiscard]] QString LastDirectory(AgxGameType type = AgxGameType::None) const;
	void SetLastDirectory(const QString& dir, AgxGameType type = AgxGameType::None);

	QString GetRelativeDataPath(AgxGameType type);
	void SaveRelativeDataPath(AgxGameType type, const QString& pathToSave) const;

	void SetLanguage(LanguageCode code);
	[[nodiscard]] LanguageCode GetLanguage() const { return m_language; }

	[[nodiscard]] int GetGraphSidebarWidth() const { return m_graphSidebarWidth; }
	void SetGraphSidebarWidth(const int width) { m_graphSidebarWidth = qMax(width, 0); }

	[[nodiscard]] bool GetGraphSidebarAutoHide() const { return m_graphSidebarAutoHide; }
	void SetGraphSidebarAutoHide(const bool enable) { m_graphSidebarAutoHide = enable; }

	[[nodiscard]] bool GetGraphSelectedToTop() const { return m_graphSelectedToTop; }
	void SetGraphSelectedToTop(const bool setting) { m_graphSelectedToTop = setting; }

	[[nodiscard]] StartupVisibilityPreference GetConsoleVisibilityPreference() const { return m_consoleVis; }
	void SetConsoleVisibilityPreference(const StartupVisibilityPreference pref) { m_consoleVis = pref; }
	void SetConsoleVisibilityPreference(int pref);

	[[nodiscard]] bool GetConvertFunctionsSetting() const { return m_convertFunctions; }
	void SetConvertFunctionsSetting(const bool setting) { m_convertFunctions = setting; }

	void SyncSettings() const;

	[[nodiscard]] QByteArray GetSavedWindowGeometry(const QString& key) const;
	void SaveWindowGeometry(const QString& key, const QByteArray& array) const;

	[[nodiscard]] bool GetLastState(const QString& key, bool defValue = true) const;
	void SaveLastState(const QString& key, bool state) const;

	[[nodiscard]] QString GetSavedVersion() const;

	[[nodiscard]] bool UseSavedGeometry() const { return m_rememberMainGeometry; }
	void SetUseSavedGeometry(const bool setting) { m_rememberMainGeometry = setting; }

	[[nodiscard]] bool UseSavedConsoledGeometry() const { return m_rememberConsoleGeometry; }
	void SetUseSavedConsoledGeometry(const bool setting) { m_rememberConsoleGeometry = setting; }

	[[nodiscard]] QSet<QString> GetCustomCategories(AgxGameType game = AgxGameType::SFBGS, bool withDefault = true) const;
	void AddCustomCategory(const QString& item, AgxGameType game = AgxGameType::SFBGS);

	[[nodiscard]] QString GetThemeFilePath() const;
	[[nodiscard]] QString GetThemeFileName() const;
	void SetThemeFileName(const QString& path);

private:
	void SetDefaultLanguage();
	[[nodiscard]] QString IniPath() const;
	static bool CreateDirectoryIfNDef(const QString& fileString);

	friend class AgxStyle;
	friend class AgxNodeStyle;
	friend class AgxConnectionStyle;
	friend class AgxGraphicsViewStyle;

	//Generic
	QString m_iniPath;
	QString m_themePath;
	QSettings* m_settings = nullptr;
	QSettings* m_dataStorage = nullptr;
	QString m_lastDirectory;
	QString m_sfbgsDirectory;

	QPointer<QTranslator> m_translator = nullptr;
	LanguageCode m_language = LanguageCode::English;

	StartupVisibilityPreference m_consoleVis = StartupVisibilityPreference::Remember;
	bool m_consoleLastState = true;

	bool m_convertFunctions = true;
	bool m_rememberMainGeometry = true;
	bool m_rememberConsoleGeometry = true;

	QSet<QString> m_categories_Default;

	QString m_themeFileName;

	//Graph
	//bool m_graphSidebarEnabled = true;
	bool m_graphSidebarAutoHide = false;
	int m_graphSidebarWidth = 0;
	bool m_graphSelectedToTop = true;

	//SFBGS
	QString m_defaultRelDataPath_SFBGS = R"(Data\Meshes\AnimTextData\Tables\Graphs\)";
	QSet<QString> m_categories_SFBGS;


public:
	struct HighlightingRule {
		QRegularExpression pattern;
		QTextCharFormat format;
	};

	void SetSymbolColor(QColor color);
	[[nodiscard]] const QColor& GetSymbolColor() const;
	void SetDecodedColor(QColor color);
	[[nodiscard]] const QColor& GetDecodedColor() const;
	const QVector<HighlightingRule>& GetHighlightingRules() { return m_highlightingRules; }

private:
	QVector<HighlightingRule> m_highlightingRules;
	
	QColor m_symColorDefault = QColor(Qt::cyan);
	QTextCharFormat m_symbolFormat;
	QColor m_decodedColorDefault = QColor(Qt::red);
	QTextCharFormat m_decodedFormat;

};