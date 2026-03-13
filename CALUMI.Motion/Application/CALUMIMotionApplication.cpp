#include "stdafx.h"
#include "CALUMIMotionApplication.h"
#include <Utilities/SettingsRegistry.h>
#include "version.h"
#include <CALUMIMotion.h>

CALUMIMotionApplication::CALUMIMotionApplication(int& argc, char** argv) : QApplication(argc, argv) 
{
	setApplicationVersion(VER_FILEVERSION_STR);
	setOrganizationName("CALUMI");
	setApplicationName("Motion");

	if (!g_log)
		g_log = new AgxLogger();

	ogHandler = qInstallMessageHandler(customMessageHandler);

	qInfo() << "Application: " << organizationName().toStdString().c_str() << applicationName().toStdString().c_str();
	qInfo() << "Application Version: " << applicationVersion().toStdString().c_str();

#ifdef DEBUGBUILD
	qDebug() << "Debug message";
	qInfo() << "Info message";
	qWarning() << "Warning message";
	qCritical() << "Critical message";
	//qFatal() << "Fatal message";
#endif // DEBUGBUILD

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

bool CALUMIMotionApplication::LoggerVisibile() const
{
	if (g_log)
		return g_log->isVisible();

	return false;
}

void CALUMIMotionApplication::UpdateApplicationTabWidgets()
{
	for (auto window : topLevelWidgets())
	{
		if (auto cWidget = dynamic_cast<CALUMIMotion*>(window))
		{
			cWidget->UpdateTabTitles();
		}
	}
}