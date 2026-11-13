#pragma once
#include <qloggingcategory.h>
#include <QQmlEngine>

#if __has_include("generated/g_tags.h")
#include "generated/g_tags.h"
#else
namespace CALUMI::Motion::Tag {
    Q_NAMESPACE
    QML_ELEMENT
    enum TagId : quint8 {
        RuntimeTag
    };
    Q_ENUM_NS(TagId)
}
#warning("generated tag header not found")
#endif

namespace CALUMI::Motion
{
    using TagId = Tag::TagId;
    static constexpr auto InvalidTagId = static_cast<TagId>(std::numeric_limits<std::underlying_type_t<TagId>>::max());

    class TagDictionary : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        QML_SINGLETON
    public:
        ~TagDictionary() override = default;
        static TagDictionary& instance();
        static TagDictionary* create(const QQmlEngine* qmlEngine, const QJSEngine* jsEngine);

        Q_STATIC_LOGGING_CATEGORY(lcTag,"TagDictionary")

    private:
        using QObject::QObject;

    public:
        static TagId fetchTag(const QString& tag, const QString& label = {});
        static QString tag(TagId id);
        static QString label(TagId id);
        static TagId id(const QString& tag);
    };
}
