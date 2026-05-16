//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppDFAMemoryLeak
#include "stdafx.h"
#include "AgxLogger.h"
#include "Utilities/SettingsRegistry.h"

AgxLogger::AgxLogger(QWidget* parent) : QWidget(parent), m_logTE(new QPlainTextEdit(this))
{
	setObjectName("CALUMIMotionLogger");

	const auto layout = new QVBoxLayout(this);
	layout->addWidget(m_logTE);
	setLayout(layout);
	m_logTE->setReadOnly(true);

	const auto& settingIns = SettingsRegistry::GetInstance();
	if (const auto bytes = settingIns.GetSavedWindowGeometry("Log/Geometry"); settingIns.UseSavedConsoledGeometry() && !bytes.isEmpty())
		restoreGeometry(bytes);
	else
		resize(600, 400);

	setWindowTitle("CALUMI Motion Log");

	const auto pref = settingIns.GetConsoleVisibilityPreference();

	bool show = true;

	switch (pref)
	{
		case StartupVisibilityPreference::Never:
			show = false;
			break;
		case StartupVisibilityPreference::Remember:
			show = settingIns.GetLastState("Log/State");
			break;
		case StartupVisibilityPreference::Always:
			show = true;
			break;
		default:
			break;
	}

	if(show)
		QTimer::singleShot(500, Qt::PreciseTimer, [this] { this->show();});
}

void AgxLogger::closeEvent(QCloseEvent * event)
{	
	hide();
	qDebug() << "Log Hidden";
	
	event->ignore();
}

void AgxLogger::hideEvent(QHideEvent* event)
{
	const auto& settingIns = SettingsRegistry::GetInstance();

	settingIns.SaveWindowGeometry("Log/Geometry", saveGeometry());
}

void AgxLogger::writeFormatted(const QString& text, const QColor& color, const bool bold) const
{
	auto cursorRef = m_logTE->textCursor();
	cursorRef.movePosition(QTextCursor::End);

	QTextCharFormat format;
	format.setForeground(QBrush(color));
	if (bold)
		format.setFontWeight(QFont::Bold);
	else
		format.setFontWeight(QFont::Normal);

	cursorRef.setCharFormat(format);
	cursorRef.insertText(text);

	cursorRef.setCharFormat(QTextCharFormat());
}

void AgxLogger::appendMessage(const QString& text, const QtMsgType& type) const
{
	const QString time = "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + "] ";
	writeFormatted(time, Qt::gray, false);

	switch (type) {
		case QtDebugMsg:
		{
#ifdef RELEASE_BUILD
			return;
#endif
			
			writeFormatted("DEBUG: ", QColor(180, 80, 255), true);
			break;
		}
		case QtInfoMsg:     writeFormatted("INFO: ", QColor(78, 160, 240), true); break;
		case QtWarningMsg:  writeFormatted("WARNING: ", QColor(255, 165, 0), true); break;
		case QtCriticalMsg: writeFormatted("CRITICAL: ", Qt::red, true); break;
		case QtFatalMsg:    writeFormatted("FATAL: ", Qt::darkRed, true); break;
	}


	//_logTE->appendPlainText(logMessage);
	writeFormatted(text + "\n", Qt::white, false);
	if (const auto scroller = m_logTE->verticalScrollBar())
		scroller->setValue(scroller->maximum());
}

void AgxLogger::saveExitState()
{
	if (m_stateSaved)
		return;

	m_stateSaved = true;

	const auto& settingIns = SettingsRegistry::GetInstance();
	settingIns.SaveLastState("Log/State", isVisible());
}
