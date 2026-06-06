//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "CALUMIMotionApplication.h"
#include "version.h"
#include <CALUMIMotion.h>

#include "../Utilities/Settings/SettingsRegistry.h"

CALUMIMotionApplication::CALUMIMotionApplication(int& argc, char** argv) : QApplication(argc, argv) 
{
	setApplicationVersion(VER_FILEVERSION_STR);
	setOrganizationName("CALUMI");
	setApplicationName("Motion");

	if (!g_log)
		g_log = new AgxLogger();

	g_ogHandler = qInstallMessageHandler(customMessageHandler);

	qInfo() << "Application: " << organizationName().toStdString().c_str() << applicationName().toStdString().c_str();
	qInfo() << "Application Version: " << applicationVersion().toStdString().c_str();

#ifdef DEBUG_BUILD
	qDebug() << "Debug message";
	qInfo() << "Info message";
	qWarning() << "Warning message";
	qCritical() << "Critical message";
	//qFatal() << "Fatal message";
#endif // DEBUG_BUILD

}

bool CALUMIMotionApplication::ToggleLogger()
{
	g_log->setVisible(!g_log->isVisible());

	return g_log->isVisible();
}

void CALUMIMotionApplication::HideLogger()
{
	g_log->setVisible(false);
}

void CALUMIMotionApplication::ShowLogger()
{
	g_log->setVisible(true);
}

bool CALUMIMotionApplication::LoggerVisible()
{
	if (g_log)
		return g_log->isVisible();

	return false;
}

bool CALUMIMotionApplication::event(QEvent* event)
{
	if (event->type() == QEvent::Type::Quit)
	{
		SaveLoggerExitState();
		HideLogger();
	}

	return QApplication::event(event);
}

void CALUMIMotionApplication::SaveLoggerExitState()
{
	g_log->saveExitState();
}

void CALUMIMotionApplication::TrackWindow(QWidget* widget)
{
	calApp->m_mainWindows.append(widget);
}

void CALUMIMotionApplication::UntrackWindow(QWidget* widget)
{
	auto& mainWindows = calApp->m_mainWindows;
	mainWindows.erase(std::ranges::remove_if(mainWindows,[widget](const QPointer<QWidget>& m_widget) {
		return widget == m_widget.get();
	}).begin(), mainWindows.end());
}

void CALUMIMotionApplication::RequestShutdown()
{
	if (calApp->m_mainWindows.isEmpty())
#ifndef __APPLE__
		quit();
#else
		SaveLoggerExitState();
		HideLogger();
#endif
}

void CALUMIMotionApplication::LoadFiles(const QStringList& filePaths)
{
	const auto app = calApp; // NOLINT(*-pro-type-static-cast-downcast)
	const auto activeWindow = CALUMIMotionApplication::activeWindow();

	if (const auto desired = static_cast<CALUMIMotion*>(activeWindow)) // NOLINT(*-pro-type-static-cast-downcast)
	{
		for (const auto& filePath : filePaths)
			desired->onLoadFile(filePath);

		return;
	}

	for (const auto& entry : app->m_mainWindows)
	{
		if (const auto desired = static_cast<CALUMIMotion*>(entry.get())) // NOLINT(*-pro-type-static-cast-downcast)
		{
			for (const auto& filePath : filePaths)
				desired->onLoadFile(filePath);

			return;
		}
	}

}

void CALUMIMotionApplication::UpdateApplicationTabWidgets()
{
	for (const auto window : topLevelWidgets())
	{
		if (const auto cWidget = dynamic_cast<CALUMIMotion*>(window))
		{
			cWidget->UpdateTabTitles();
		}
	}
}
