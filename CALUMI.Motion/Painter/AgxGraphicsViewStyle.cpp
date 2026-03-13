#include "stdafx.h"
#include "AgxGraphicsViewStyle.h"

#include "AgxStyleCollection.h"
#include <Utilities/SettingsRegistry.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>

inline void initResources()
{
	Q_INIT_RESOURCE(CALUMIMotion);
}

AgxGraphicsViewStyle::AgxGraphicsViewStyle() {
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
        qInfo() << "Loading Default Graph Theme";
        // This configuration is stored inside the compiled unit and is loaded statically
        // However we have a backup style in the initialization if all else fails
        loadJsonFile(":/CALUMIMotion/Resources/DefaultStyle.json");
    }
}

AgxGraphicsViewStyle::AgxGraphicsViewStyle(QString jsonText) {
    loadJsonText(jsonText);
}

void AgxGraphicsViewStyle::setStyle(QString jsonText) {
    AgxGraphicsViewStyle style(jsonText);

    AgxStyleCollection::setGraphicsViewStyle(style);
}

#define FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(v, variable)

#define FLOW_VIEW_STYLE_READ_COLOR(values, variable) \
    { \
        auto valueRef = values[#variable]; \
        FLOW_VIEW_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
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
    }

#define FLOW_VIEW_STYLE_WRITE_COLOR(values, variable) \
    { \
        values[#variable] = variable.name(); \
    }

void AgxGraphicsViewStyle::loadJson(QJsonObject const& json)
{
    QJsonValue nodeStyleValues = json["GraphicsViewStyle"];

    QJsonObject obj = nodeStyleValues.toObject();

    if (obj.isEmpty())
    {
        qWarning() << "Failed to parse view style Json file. Exiting view style creation and leaving existing values.";
        return;
    }

    FLOW_VIEW_STYLE_READ_COLOR(obj, BackgroundColor);
    FLOW_VIEW_STYLE_READ_COLOR(obj, FineGridColor);
    FLOW_VIEW_STYLE_READ_COLOR(obj, CoarseGridColor);
}

QJsonObject AgxGraphicsViewStyle::toJson() const
{
    QJsonObject obj;

    FLOW_VIEW_STYLE_WRITE_COLOR(obj, BackgroundColor);
    FLOW_VIEW_STYLE_WRITE_COLOR(obj, FineGridColor);
    FLOW_VIEW_STYLE_WRITE_COLOR(obj, CoarseGridColor);

    QJsonObject root;
    root["GraphicsViewStyle"] = obj;

    return root;
}
