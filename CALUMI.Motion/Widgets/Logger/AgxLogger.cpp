#include "stdafx.h"
#include "AgxLogger.h"
#include "Utilities/SettingsRegistry.h"

AgxLogger::AgxLogger(QWidget* parent) : QWidget(parent), _logTE(new QPlainTextEdit(this))
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(_logTE);
	setLayout(layout);
	_logTE->setReadOnly(true);

	auto& settingIns = SettingsRegistry::GetInstance();
	auto bytes = settingIns.GetSavedWindowGeometry("Log/Geometry");
	if (settingIns.UseSavedConsoledGeometry() && !bytes.isEmpty())
		restoreGeometry(bytes);
	else
		resize(600, 400);

	setWindowTitle("CALUMI Motion Log");

	auto pref = settingIns.GetConsoleVisibilityPreference();

	bool show = true;

	switch (pref)
	{
		case StartupVisibiltyPreference::Never:
			show = false;
			break;
		case StartupVisibiltyPreference::Remember:
			show = settingIns.GetLastState("Log/State");
			break;
		case StartupVisibiltyPreference::Always:
			show = true;
			break;
		default:
			break;
	}

	if(show)
		QTimer::singleShot(500, Qt::PreciseTimer, [this]() { this->show();});
}

AgxLogger::~AgxLogger() {}

void AgxLogger::closeEvent(QCloseEvent * event)
{	
	hide();
	qDebug() << "Log Hidden";
	
	event->ignore();
}

void AgxLogger::hideEvent(QHideEvent* event)
{
	auto& settingIns = SettingsRegistry::GetInstance();
	//settingIns.beginGroup("Log");
	settingIns.SaveWindowGeometry("Log/Geometry", saveGeometry());
}

void AgxLogger::writeFormatted(const QString& text, const QColor& color, bool bold)
{
	auto cursorref = _logTE->textCursor();
	cursorref.movePosition(QTextCursor::End);

	QTextCharFormat format;
	format.setForeground(QBrush(color));
	if (bold)
		format.setFontWeight(QFont::Bold);
	else
		format.setFontWeight(QFont::Normal);

	cursorref.setCharFormat(format);
	cursorref.insertText(text);

	cursorref.setCharFormat(QTextCharFormat());
}

void AgxLogger::appendMessage(const QString& text, const QtMsgType& type)
{
	QString time = "[" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + "] ";
	writeFormatted(time, Qt::gray, false);

	switch (type) {
		case QtDebugMsg:
		{
#ifdef RELEASEBUILD
			return;
#endif // DEBUGBUILD
			
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
	QScrollBar* scroller = _logTE->verticalScrollBar();
	if (scroller)
		scroller->setValue(scroller->maximum());
}