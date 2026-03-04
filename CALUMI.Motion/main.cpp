#include "stdafx.h"
#include "CALUMIMotion.h"
#pragma warning(push,0)

//
#include <QtWidgets/QApplication>
#pragma warning(pop)
#include <AgxNodes/AgxNodes>
#include "Utilities/AgxJsonHelper.h"
#include "Utilities/AgxNodeRegistry.h"
#include "Utilities/DialogPool.h"
#include "Utilities/AgxGraphRegistry.h"
//#include <QOpenGLWidget>
//#include <QOpenGLWindow>
#include "Application/CALUMIMotionApplication.h"
#include "Widgets/TextEditDialog.h"
#include <Utilities/SettingsRegistry.h>
#include "Widgets/Logger/AgxLogger.h"


int main(int argc, char* argv[])
{
    CALUMIMotionApplication app(argc, argv);

    //app.setStyle("windows");

    QPixmap pixmap(":/Images/Resources/NASA_ISS_LongExposure.png");
    QSplashScreen splash = QSplashScreen(pixmap);
    splash.showMessage("CALUMI Motion:\nLoading modules...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
    splash.show();
    
    app.processEvents();

    {
        splash.showMessage("CALUMI Motion:\nLoading Node Registry...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
        AgxNodeRegistry::GetInstance();
        //QThread::msleep(500);
    }

    {
        splash.showMessage("CALUMI Motion:\nLoading Dialog Pool...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
        DialogPool_SFBGS::GetInstance();
        //QThread::msleep(100);
    }

    {
        splash.showMessage("CALUMI Motion:\nLoading Graph Registry...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
        AgxGraphRegistry::GetInstance();
        //QThread::msleep(400);
    }

    {
        splash.showMessage("CALUMI Motion:\nPre-Loading Fonts...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
        auto fonts = TextEditDialog();
        //QThread::msleep(100);
    }

    {
        splash.showMessage("CALUMI Motion:\nLoading User Settings...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
        
        //manage app wide settings here
        auto& setRef = SettingsRegistry::GetInstance();
        if (!setRef.GetSavedVersion().isEmpty())
        {
            if(setRef.GetSavedVersion() != app.applicationVersion())
            {
#ifdef RELEASEBUILD
                //what's new popup?
#endif // RELEASEBUILD
                qDebug() << "Version Mismatch!";
            }
        }
    }

    CALUMIMotion window;
    window.show();

    splash.finish(&window);

    return app.exec();
}
