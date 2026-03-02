#pragma once

#include <QApplication>
#include "Widgets/Logger/AgxLogger.h"
#include <QMutex>

static AgxLogger* g_log = nullptr;
static QMutex logMutex;
static QtMessageHandler ogHandler = nullptr;

class MouseEventFilter : public QObject
{
    Q_OBJECT
protected:
    inline bool eventFilter(QObject* obj, QEvent* event) override
    {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            // Log the object name and class of the receiver
            qDebug() << "Mouse press event received by object:" << obj->objectName()
                << " Class:" << obj->metaObject()->className();

            // You can also get the widget type if it's a QWidget
            if (QWidget* widget = qobject_cast<QWidget*>(obj)) {
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

    bool ToggleLogger();
    void HideLogger();
    void ShowLogger();
    bool LoggerVisibile() const;

    void UpdateApplicationTabWidgets();

private:
	QHash<QString, unsigned int> _list;
};

inline void customMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QMutexLocker locker(&logMutex); // Ensure thread safety

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

    if (ogHandler)
        ogHandler(type, context, msg);
}