#pragma once
#include <QPointF>
#include <QSizeF>
#include <QUuid>

#include "NodeType.h"
#include "PortList.h"

namespace CALUMI::Motion
{
    /**
     * @brief Abstract node model with basic data methods
     * @details
     */
    class INode : public QObject
    {
        Q_OBJECT
        QML_UNCREATABLE("Cannot create an abstract node element")

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

        Q_PROPERTY(PortList* inPorts READ inPortListPtr CONSTANT)
        Q_PROPERTY(PortList* outPorts READ outPortListPtr CONSTANT)
        Q_PROPERTY(PropertyList* propertyList READ propertyListPtr CONSTANT)
        Q_PROPERTY(PropertyTree* propertyTables READ propertyTablesPtr CONSTANT)

    public:
        using QObject::QObject;
        ~INode() override = default;

        [[nodiscard]] virtual QUuid nodeId() const = 0;

///< @name Node State
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
///< @name Node Type
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
///< @name Node Transform
///< {
    public:
        [[nodiscard]] virtual QPointF position() const { return collapsedPosition(); }
        [[nodiscard]] virtual QPointF expandedPosition() const { return collapsedPosition(); }
        [[nodiscard]] virtual QPointF collapsedPosition() const = 0;

        virtual void setPosition(const QPointF& position) = 0;
    signals:
        void positionChanged();
///< }
///< @name Port Data
///< {
    public:
        [[nodiscard]] virtual PortId nextPortId() const = 0;
        [[nodiscard]] virtual PortId peekNextPortId() const = 0;

        [[nodiscard]] virtual QPointer<PortList> portList(PortType type) = 0;
        [[nodiscard]] virtual QPointer<const PortList> portList(PortType type) const = 0;

    private:
        [[nodiscard]] PortList* inPortListPtr() { return portList(PortType::IN); }
        [[nodiscard]] PortList* outPortListPtr() { return portList(PortType::OUT); }

    signals:
        void portListPtrChanged(PortTypes types);

///< }
///< @name Properties
///< {
    public:
        [[nodiscard]] virtual QPointer<PropertyList> propertyList() = 0;
        [[nodiscard]] virtual QPointer<const PropertyList> propertyList() const = 0;
        [[nodiscard]] virtual QPointer<PropertyTree> propertyTables() = 0;
        [[nodiscard]] virtual QPointer<const PropertyTree> propertyTables() const = 0;
    private:
        [[nodiscard]] PropertyList* propertyListPtr() { return propertyList(); }
        [[nodiscard]] PropertyTree* propertyTablesPtr() { return propertyTables(); }
    signals:
        void propertyListPtrChanged();
        void propertyTablesPtrChanged();
///< }
///< @name Embedded Nodes
///< {
    public:
        [[nodiscard]] QList<QUuid> childNodeIds(bool recursive) const;
///< }
    };
}
