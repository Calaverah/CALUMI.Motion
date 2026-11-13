#include "EntryTypes.h"
#include <QDebug>
#include <QVariant>

namespace CALUMI::Motion
{
    EntryTypesEnum& EntryTypesEnum::instance()
    {
        static auto entryTypes = new EntryTypesEnum();
        return *entryTypes;
    }

    EntryTypesEnum* EntryTypesEnum::create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        EntryTypesEnum* entryEnum = &instance();
        QJSEngine::setObjectOwnership(entryEnum, QJSEngine::CppOwnership);

        return entryEnum;
    }

    QHash<int, QByteArray> PropertyList::roleNames() const
    {
        static QHash<int, QByteArray> output = {
            { static_cast<int>(Role::Present),          "present"},
            { static_cast<int>(Role::AllowedTypes),     "allowedTypes" },
            { static_cast<int>(Role::CustomDropdown),   "customDropdown" },
            { static_cast<int>(Role::Label),            "label" },
            { static_cast<int>(Role::Tag),              "tag" },
            { static_cast<int>(Role::Value),            "value" }
        };

        return output;
    }

    int PropertyList::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(m_list.size());
    }

    QVariantList PropertyList::values(const QString& tag, const bool caseSensitive) const
    {
        const auto cSense = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

        QVariantList output;
        output.reserve(static_cast<qsizetype>(m_list.size()));

        auto tagsMatchCheck = [&tag, &cSense](const Entry& e)
        {
            const auto t = TagDictionary::tag(e.tagId);
            return tag.compare(t, cSense) == 0;
        };

        for (const auto& entry : m_list | std::views::filter(tagsMatchCheck))
        {
            output.append(entry.value);
        }

        output.shrink_to_fit();
        return output;
    }

    QVariant PropertyList::data(const QModelIndex& index, const int role) const
    {
        if (!index.isValid() || index.row() < 0 || index.row() >= m_list.size())
            return {};

        // ReSharper disable once CppUseStructuredBinding
        const auto& entry = m_list.at(index.row());

        switch (role)
        {
        case Qt::DisplayRole:
            return QString("%1 : %2").arg(TagDictionary::label(entry.tagId)).arg(entry.value.toString());
        case static_cast<int>(Role::Value):
            return entry.value;
        case static_cast<int>(Role::AllowedTypes):
            return QVariant::fromValue(entry.allowedTypes);
        case static_cast<int>(Role::CustomDropdown):
            return customDropdown(entry.customDropdown);
        case static_cast<int>(Role::Tag):
            return TagDictionary::tag(entry.tagId);
        case static_cast<int>(Role::Label):
            return TagDictionary::label(entry.tagId);
        case static_cast<int>(Role::Present):
            return entry.present;
        default:
            break;
        }

        return {};
    }

    QStringList PropertyList::customDropdown(const quint16 index) const
    {
        if (m_parent)
            return m_parent->customDropdown(index);

        return {};
    }
}
