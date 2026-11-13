#include "TagDictionary.h"
#include <qloggingcategory.h>

namespace CALUMI::Motion
{
    TagDictionary& TagDictionary::instance()
    {
        static auto t = new TagDictionary();
        return *t;
    }

    TagDictionary* TagDictionary::create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)

        TagDictionary* tagDictionary = &instance();
        QJSEngine::setObjectOwnership(tagDictionary, QJSEngine::CppOwnership);

        return tagDictionary;
    }

#if not __has_include("generated/g_tags.cpp")

#warning("generated tag source not found")

    static void s_MissingSourceLog()
    {
        qCCritical(TagDictionary::lcTag()) << "Tag files not generated";
    }

    TagId TagDictionary::fetchTag([[maybe_unused]] const QString& tag, [[maybe_unused]] const QString& label)
    {
        s_MissingSourceLog();
        // ReSharper disable once CppRedundantCastExpression
        return static_cast<TagId>(InvalidTagId);
    }

    QString TagDictionary::tag([[maybe_unused]] TagId id)
    {
        s_MissingSourceLog();
        return "error";
    }

    QString TagDictionary::label([[maybe_unused]] TagId id)
    {
        s_MissingSourceLog();
        return "ERROR";
    }

    TagId TagDictionary::id([[maybe_unused]] const QString& tag)
    {
        s_MissingSourceLog();
        // ReSharper disable once CppRedundantCastExpression
        return static_cast<TagId>(InvalidTagId);
    }
#endif
}
