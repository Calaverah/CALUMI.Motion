#pragma once

#pragma warning(push,0)
#include <QColor>
#include <QJsonObject>
#pragma warning(pop)

QJsonObject colorToJsonObject(const QColor& color);

QColor JsonObjectToColor(const QJsonObject& jColor);

QJsonObject FindJsonOverlaps(const QJsonObject& source, const QJsonObject& mask);

QJsonObject QStringListToQJsonObject(const QStringList& list, const QJsonValue& value = "");

QJsonObject JsonMerge(const QJsonObject& obj1, const QJsonObject& obj2);