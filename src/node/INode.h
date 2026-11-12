#pragma once
#include <QPointF>
#include <QSizeF>
#include "NodeType.h"

namespace CALUMI::Motion
{
    /**
     * @brief Abstract node model with basic data methods
     * @details
     */
    class INode : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool hidden READ hidden WRITE setHidden NOTIFY hiddenChanged)
        Q_PROPERTY(bool collapsed READ collapsed WRITE setCollapsed NOTIFY collapsedChanged)
        Q_PROPERTY(bool hasWarning READ hasWarning NOTIFY diagnosticsChanged)
        Q_PROPERTY(bool hasError READ hasError NOTIFY diagnosticsChanged)
        Q_PROPERTY(bool hasFatal READ hasFatal NOTIFY diagnosticsChanged)
        Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
        Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle NOTIFY subtitleChanged)
        Q_PROPERTY(QString typeName READ typeName NOTIFY typeChanged)
        Q_PROPERTY(NodeType type READ type NOTIFY typeChanged)
        Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
        Q_PROPERTY(QPointF expandedPosition READ expandedPosition NOTIFY positionChanged)
        Q_PROPERTY(QPointF collapsedPosition READ collapsedPosition NOTIFY positionChanged)

    public:
        using QObject::QObject;
        ~INode() override = default;

///< @name State
///< {
        [[nodiscard]] virtual bool hidden() const { return false; }
        [[nodiscard]] virtual bool collapsed() const { return false; }
        [[nodiscard]] virtual bool hasWarning() const = 0;
        [[nodiscard]] virtual bool hasError() const = 0;
        [[nodiscard]] virtual bool hasFatal() const = 0;


        virtual void setHidden(bool hidden) {}
        virtual void setCollapsed(bool collapsed) {}
    signals:
        void hiddenChanged(bool hidden);
        void collapsedChanged(bool collapsed);
        void diagnosticsChanged();
///< }
///< @name NodeType
///< {
    public:
        [[nodiscard]] virtual QString title() const = 0;
        [[nodiscard]] virtual QString subtitle() const = 0;
        [[nodiscard]] virtual QString typeName() const = 0;
        [[nodiscard]] virtual NodeType type() const = 0;

        virtual void setTitle(const QString& newTitle) {}
        virtual void setSubtitle(const QString& newSubtitle) {}
    signals:
        void titleChanged();
        void subtitleChanged();
        void typeChanged();
///< }
///< @name Transform
///< {
    public:
        [[nodiscard]] virtual QPointF position() const { return collapsedPosition(); }
        [[nodiscard]] virtual QPointF expandedPosition() const { return collapsedPosition(); }
        [[nodiscard]] virtual QPointF collapsedPosition() const = 0;

        virtual void setPosition(const QPointF& position) = 0;
    signals:
        void positionChanged();
///< }
    };
}
