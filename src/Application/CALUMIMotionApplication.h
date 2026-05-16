//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once

#include <qcoreevent.h>
#include <QApplication>
#include "Widgets/Logger/AgxLogger.h"
#include <QMutex>

static AgxLogger* g_log = nullptr;
static QMutex g_logMutex;
static QtMessageHandler g_ogHandler = nullptr;

class MouseEventFilter : public QObject
{
    Q_OBJECT
public:
    bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() == QEvent::MouseButtonPress) {
            //auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
            // Log the object name and class of the receiver
            qDebug() << "Mouse press event received by object:" << obj->objectName()
                << " Class:" << obj->metaObject()->className();

            // You can also get the widget type if it's a QWidget
            if (const auto widget = qobject_cast<QWidget*>(obj)) {
                qDebug() << "Receiver is a QWidget. Its window title is:" << widget->windowTitle();
            }
        }
        // Continue event propagation
        return QObject::eventFilter(obj, event);
    }
};



class CALUMIMotionApplication : public QApplication {
	Q_OBJECT
public:
	CALUMIMotionApplication(int& argc, char** argv);

    static bool ToggleLogger();
    static void HideLogger();
    static void ShowLogger();
    static bool LoggerVisible();

    static void UpdateApplicationTabWidgets();
    static void SaveLoggerExitState();

protected:
    bool event(QEvent* event) override;

private:
	QHash<QString, unsigned int> m_list;
};

// ReSharper disable once CppParameterMayBeConst
inline void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QMutexLocker locker(&g_logMutex); // Ensure thread safety

    //QString logMessage;
    //QTextStream stream(&logMessage);

    //stream << msg;

    // Output to the console (optional)
    fprintf(stderr, "%s\n", msg.toUtf8().constData());
    fflush(stderr); // Ensure real-time console output

    // Append to the log window via signal/slot connection from the main thread
    if (g_log) {
        // Use a signal to safely update the GUI from the handler thread
        QMetaObject::invokeMethod(g_log, "appendMessage",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, msg), Q_ARG(QtMsgType, type));
    }

    if (g_ogHandler)
        g_ogHandler(type, context, msg);
}
