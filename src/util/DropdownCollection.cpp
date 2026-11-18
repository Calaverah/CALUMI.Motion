#include "DropdownCollection.h"

namespace CALUMI::Motion
{
    DropdownCollection& DropdownCollection::instance()
    {
        static auto collection = new DropdownCollection();
        return *collection;
    }

    DropdownCollection* DropdownCollection::create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        DropdownCollection* dropdownCollection = &instance();
        QJSEngine::setObjectOwnership(dropdownCollection, QJSEngine::CppOwnership);

        return dropdownCollection;
    }

    static const std::array<std::pair<std::string_view, QString>, DropId::DropIdCount> s_dropdowns = {
        {
#if __has_include("generated/g_dropdown.inc")
#include "generated/g_dropdown.inc"
#else
            {"Error\0error\0err", ""},
            {"",""},
#endif
            }};

    QStringList DropdownCollection::GetUniqueDropdown(const DropId id, const bool labels)
    {
        if (id < DropId::DropIdCount)
        {
            const auto& view = s_dropdowns.at(id).first;
            if (labels)
                return s_dropdowns.at(id).second.split(QChar('\0'));

            return QString::fromUtf8(view.data(), static_cast<qsizetype>(view.size())).split(QChar{'\0'});
        }

        const auto& inst = instance();

        if (id >= DropId::DropIdCount + inst.m_runtimeDropdowns.size())
            return {};

        if (const auto aIdx = id - DropId::DropIdCount; aIdx >= inst.m_runtimeDropdowns.size())
            return {};
        else
            return inst.m_runtimeDropdowns.at(aIdx);
    }

    DropId DropdownCollection::RegisterUniqueDropDown(const QStringList& entry)
    {
        auto& inst = instance();
        if (inst.m_runtimeHash.contains(entry))
            return inst.m_runtimeHash.value(entry);

        const auto idx = inst.m_runtimeDropdowns.size();
        inst.m_runtimeHash.insert(entry, static_cast<DropId>(idx));
        inst.m_runtimeDropdowns.append(entry);
        return inst.m_runtimeHash.value(entry);
    }

    DropId DropdownValue::dropId() const
    {
        return m_dropId;
    }

    quint32 DropdownValue::index() const
    {
        return m_index;
    }

    QString DropdownValue::label() const
    {
        const auto labels = DropdownCollection::GetUniqueDropdown(m_dropId, true);

        if (m_index >= labels.size())
            return {};

        return labels.at(m_index);
    }

    QString DropdownValue::tag() const
    {
        const auto labels = DropdownCollection::GetUniqueDropdown(m_dropId, false);

        if (m_index >= labels.size())
            return {};

        return labels.at(m_index);
    }

    void DropdownValue::setIndex(const quint32 index)
    {
        m_index = index;
    }
}
