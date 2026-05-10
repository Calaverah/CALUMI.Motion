//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#include "stdafx.h"
#include "AgxConnectionStyle.h"
#include "AgxStyleCollection.h"
#include <Utilities/SettingsRegistry.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>

#include <QDebug>
#include <random>

inline void initResources()
{
    Q_INIT_RESOURCE(CALUMIMotion);
}

AgxConnectionStyle::AgxConnectionStyle() {
    // Explicit resources inialization for preventing the static initialization
    // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
    initResources();

    QString filePath = SettingsRegistry::GetInstance().GetThemeFilePath();
    QFileInfo fileInfo(filePath);

    if (!fileInfo.fileName().isEmpty() && fileInfo.exists())
    {
        loadJsonFile(filePath);
    } else
    {
        qInfo() << "Loading Default Connections Theme";
        // This configuration is stored inside the compiled unit and is loaded statically
        // However we have a backup style in the initialization if all else fails
        loadJsonFile(":/CALUMIMotion/Resources/DefaultStyle.json");
    }
}

AgxConnectionStyle::AgxConnectionStyle(QString jsonText)
{
    loadJsonFile(":/CALUMIMotion/Resources/DefaultStyle.json");
    loadJsonText(jsonText);
}

void AgxConnectionStyle::setConnectionStyle(QString jsonText)
{
    AgxConnectionStyle style(jsonText);

    AgxStyleCollection::setConnectionStyle(style);
}

#define CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(v, variable)

#define CONNECTION_VALUE_EXISTS(v) \
    (v.type() != QJsonValue::Undefined && v.type() != QJsonValue::Null)

#define CONNECTION_STYLE_READ_COLOR(values, variable) \
    { \
        auto valueRef = values[#variable]; \
        CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
        if (CONNECTION_VALUE_EXISTS(valueRef)) { \
            if (valueRef.isArray()) { \
                auto colorArray = valueRef.toArray(); \
                std::vector<int> rgb; \
                rgb.reserve(3); \
                for (auto it = colorArray.begin(); it != colorArray.end(); ++it) { \
                    rgb.push_back((*it).toInt()); \
                } \
                variable = QColor(rgb[0], rgb[1], rgb[2]); \
            } else { \
                variable = QColor(valueRef.toString()); \
            } \
        } \
    }

#define CONNECTION_STYLE_WRITE_COLOR(values, variable) \
    { \
        values[#variable] = variable.name(); \
    }

#define CONNECTION_STYLE_READ_FLOAT(values, variable) \
    { \
        auto valueRef = values[#variable]; \
        CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
        if (CONNECTION_VALUE_EXISTS(valueRef)) \
            variable = valueRef.toDouble(); \
    }

#define CONNECTION_STYLE_WRITE_FLOAT(values, variable) \
    { \
        values[#variable] = variable; \
    }

#define CONNECTION_STYLE_READ_BOOL(values, variable) \
    { \
        auto valueRef = values[#variable]; \
        CONNECTION_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
        if (CONNECTION_VALUE_EXISTS(valueRef)) \
            variable = valueRef.toBool(); \
    }

#define CONNECTION_STYLE_WRITE_BOOL(values, variable) \
    { \
        values[#variable] = variable; \
    }

void AgxConnectionStyle::loadJson(QJsonObject const& json)
{
    QJsonValue nodeStyleValues = json["ConnectionStyle"];

    QJsonObject obj = nodeStyleValues.toObject();

    if (obj.isEmpty())
    {
        qWarning() << "Failed to parse view connection Json file. Exiting connection style creation and leaving existing values.";
        return;
    }

    CONNECTION_STYLE_READ_COLOR(obj, ConstructionColor);
    CONNECTION_STYLE_READ_COLOR(obj, NormalColor);
    CONNECTION_STYLE_READ_COLOR(obj, SelectedColor);
    CONNECTION_STYLE_READ_COLOR(obj, SelectedHaloColor);
    CONNECTION_STYLE_READ_COLOR(obj, HoveredColor);

    CONNECTION_STYLE_READ_FLOAT(obj, LineWidth);
    CONNECTION_STYLE_READ_FLOAT(obj, ConstructionLineWidth);
    CONNECTION_STYLE_READ_FLOAT(obj, PointDiameter);

    CONNECTION_STYLE_READ_BOOL(obj, UseDataDefinedColors);
}

QJsonObject AgxConnectionStyle::toJson() const
{
    QJsonObject obj;

    CONNECTION_STYLE_WRITE_COLOR(obj, ConstructionColor);
    CONNECTION_STYLE_WRITE_COLOR(obj, NormalColor);
    CONNECTION_STYLE_WRITE_COLOR(obj, SelectedColor);
    CONNECTION_STYLE_WRITE_COLOR(obj, SelectedHaloColor);
    CONNECTION_STYLE_WRITE_COLOR(obj, HoveredColor);

    CONNECTION_STYLE_WRITE_FLOAT(obj, LineWidth);
    CONNECTION_STYLE_WRITE_FLOAT(obj, ConstructionLineWidth);
    CONNECTION_STYLE_WRITE_FLOAT(obj, PointDiameter);

    CONNECTION_STYLE_WRITE_BOOL(obj, UseDataDefinedColors);

    QJsonObject root;
    root["ConnectionStyle"] = obj;

    return root;
}

QColor AgxConnectionStyle::constructionColor() const
{
    return ConstructionColor;
}

QColor AgxConnectionStyle::normalColor() const
{
    return NormalColor;
}

QColor AgxConnectionStyle::normalColor(QString typeId) const
{
    size_t hash = qHash(typeId);

    size_t const hue_range = 0xFF;

    std::mt19937 gen(static_cast<unsigned int>(hash));
    std::uniform_int_distribution<int> distrib(0, hue_range);

    int hue = distrib(gen);
    int sat = 120 + hash % 129;

    return QColor::fromHsl(hue, sat, 160);
}

QColor AgxConnectionStyle::selectedColor() const
{
    return SelectedColor;
}

QColor AgxConnectionStyle::selectedHaloColor() const
{
    return SelectedHaloColor;
}

QColor AgxConnectionStyle::hoveredColor() const
{
    return HoveredColor;
}

float AgxConnectionStyle::lineWidth() const
{
    return LineWidth;
}

float AgxConnectionStyle::constructionLineWidth() const
{
    return ConstructionLineWidth;
}

float AgxConnectionStyle::pointDiameter() const
{
    return PointDiameter;
}

bool AgxConnectionStyle::useDataDefinedColors() const
{
    return UseDataDefinedColors;
}