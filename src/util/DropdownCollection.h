#pragma once
#include <QLoggingCategory>

#if __has_include("generated/g_dropdown.h")
#include "generated/g_dropdown.h"
#else
#include <QQmlEngine>
namespace CALUMI::Motion::Dropdown {
    Q_NAMESPACE
    QML_ELEMENT

    enum DropId : quint32 {
        gen_error, ///< ['error']
        gen_empty, ///< []
        DropIdCount
    };
    Q_ENUM_NS(DropId)
}
#warning("generated dropdown header not found")
#endif


namespace CALUMI::Motion
{
    using DropId = Dropdown::DropId;
    static constexpr auto InvalidDropId = static_cast<DropId>(std::numeric_limits<std::underlying_type_t<DropId>>::max());

    class DropdownCollection : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        ~DropdownCollection() override = default;
        static DropdownCollection& instance();
        static DropdownCollection* create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine);

        Q_STATIC_LOGGING_CATEGORY(lcDrop, "DropdownCollection")
    private:
        using QObject::QObject;

    public:
        static QStringList GetUniqueDropdown(DropId id, bool labels);
        static DropId RegisterUniqueDropDown(const QStringList& entry);

    private:
        QList<QStringList> m_runtimeDropdowns;
        QHash<QStringList, DropId> m_runtimeHash;
    };

    class DropdownValue
    {
        Q_GADGET
        QML_VALUE_TYPE(dropdownValue)

        Q_PROPERTY(DropId dropId READ dropId CONSTANT);
        Q_PROPERTY(quint32 index READ index WRITE setIndex);
        Q_PROPERTY(QString label READ label CONSTANT);
        Q_PROPERTY(QString tag READ tag CONSTANT);
    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        DropdownValue(const DropId dropId = InvalidDropId, const quint32 index = std::numeric_limits<quint32>::max()) : m_dropId(dropId), m_index(index) {}

        [[nodiscard]] DropId dropId() const;
        [[nodiscard]] quint32 index() const;
        [[nodiscard]] QString label() const;
        [[nodiscard]] QString tag() const;

        void setIndex(quint32 index);

    private:
        DropId m_dropId;
        quint32 m_index;
    };
}
