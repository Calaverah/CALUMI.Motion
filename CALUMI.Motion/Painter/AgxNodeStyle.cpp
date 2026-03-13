#include "stdafx.h"
#include "AgxNodeStyle.h"
#include "AgxStyleCollection.h"
#include <Utilities/SettingsRegistry.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValueRef>

#include <QDebug>

inline void initResources() {
	Q_INIT_RESOURCE(CALUMIMotion);
}

AgxNodeStyle::AgxNodeStyle() {
    // Explicit resources inialization for preventing the static initialization
    // order fiasco: https://isocpp.org/wiki/faq/ctors#static-init-order
    initResources();

    QString filePath = SettingsRegistry::GetInstance().GetThemeFilePath();
    QFileInfo fileInfo(filePath);

    if (!fileInfo.fileName().isEmpty() && fileInfo.exists())
    {
        loadJsonFile(filePath);
    }
    else
    {
        qInfo() << "Loading Default Node Theme";
        // This configuration is stored inside the compiled unit and is loaded statically
        // However we have a backup style in the initialization if all else fails
        loadJsonFile(":/CALUMIMotion/Resources/DefaultStyle.json");
    }
}

AgxNodeStyle::AgxNodeStyle(QString jsonText) {
    loadJsonText(jsonText);
}

AgxNodeStyle::AgxNodeStyle(const QJsonObject& json) {
    loadJson(json);
}

void AgxNodeStyle::setNodeStyle(QString jsonText)
{
    AgxNodeStyle style(jsonText);

    AgxStyleCollection::setNodeStyle(style);
}

#define NODE_STYLE_CHECK_UNDEFINED_VALUE(v, variable)

#define NODE_STYLE_READ_COLOR(values, variable) \
    { \
        auto valueRef = values[#variable]; \
        NODE_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
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

#define NODE_STYLE_WRITE_COLOR(values, variable) \
    { \
        values[#variable] = variable.name(); \
    }

#define NODE_STYLE_READ_FLOAT(values, variable) \
    { \
        auto valueRef = values[#variable]; \
        NODE_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
        variable = valueRef.toDouble(); \
    }

#define NODE_STYLE_WRITE_FLOAT(values, variable) \
    { \
        values[#variable] = variable; \
    }

#define NODE_STYLE_READ_BOOL(values, variable) \
    { \
            auto valueRef = values[#variable]; \
            NODE_STYLE_CHECK_UNDEFINED_VALUE(valueRef, variable) \
            variable = valueRef.toBool(); \
    }

#define NODE_STYLE_WRITE_BOOL(values, variable) \
    { \
            values[#variable] = variable; \
    }

void AgxNodeStyle::loadJson(const QJsonObject& json) {
    QJsonValue nodeStyleValues = json["NodeStyle"];

    QJsonObject obj = nodeStyleValues.toObject();

    if(obj.isEmpty()) 
    {
        qWarning() << "Failed to parse node style Json file. Exiting node style creation and leaving existing values.";
        return;
    }

    NODE_STYLE_READ_COLOR(obj, NormalBoundaryColor);
    NODE_STYLE_READ_COLOR(obj, SelectedBoundaryColor);
    NODE_STYLE_READ_COLOR(obj, GradientColor0);
    NODE_STYLE_READ_COLOR(obj, GradientColor1);
    NODE_STYLE_READ_COLOR(obj, GradientColor2);
    NODE_STYLE_READ_COLOR(obj, GradientColor3);
    NODE_STYLE_READ_COLOR(obj, AlternateStateColor);
    //NODE_STYLE_READ_COLOR(obj, ShadowColor);
    //NODE_STYLE_READ_BOOL(obj, ShadowEnabled);
    NODE_STYLE_READ_COLOR(obj, FontColor);
    NODE_STYLE_READ_COLOR(obj, FontColorFaded);
    NODE_STYLE_READ_COLOR(obj, ConnectionPointColor);
    NODE_STYLE_READ_COLOR(obj, FilledConnectionPointColor);
    NODE_STYLE_READ_COLOR(obj, WarningColor);
    NODE_STYLE_READ_COLOR(obj, ErrorColor);

    NODE_STYLE_READ_FLOAT(obj, PenWidth);
    NODE_STYLE_READ_FLOAT(obj, HoveredPenWidth);
    NODE_STYLE_READ_FLOAT(obj, ConnectionPointDiameter);

    NODE_STYLE_READ_FLOAT(obj, Opacity);
}

QJsonObject AgxNodeStyle::toJson() const {
    QJsonObject obj;

    NODE_STYLE_WRITE_COLOR(obj, NormalBoundaryColor);
    NODE_STYLE_WRITE_COLOR(obj, SelectedBoundaryColor);
    NODE_STYLE_WRITE_COLOR(obj, GradientColor0);
    NODE_STYLE_WRITE_COLOR(obj, GradientColor1);
    NODE_STYLE_WRITE_COLOR(obj, GradientColor2);
    NODE_STYLE_WRITE_COLOR(obj, GradientColor3);
    NODE_STYLE_WRITE_COLOR(obj, AlternateStateColor);
    //NODE_STYLE_WRITE_COLOR(obj, ShadowColor);
    //NODE_STYLE_WRITE_BOOL(obj, ShadowEnabled);
    NODE_STYLE_WRITE_COLOR(obj, FontColor);
    NODE_STYLE_WRITE_COLOR(obj, FontColorFaded);
    NODE_STYLE_WRITE_COLOR(obj, ConnectionPointColor);
    NODE_STYLE_WRITE_COLOR(obj, FilledConnectionPointColor);
    NODE_STYLE_WRITE_COLOR(obj, WarningColor);
    NODE_STYLE_WRITE_COLOR(obj, ErrorColor);

    NODE_STYLE_WRITE_FLOAT(obj, PenWidth);
    NODE_STYLE_WRITE_FLOAT(obj, HoveredPenWidth);
    NODE_STYLE_WRITE_FLOAT(obj, ConnectionPointDiameter);

    NODE_STYLE_WRITE_FLOAT(obj, Opacity);

    QJsonObject root;
    root["NodeStyle"] = obj;

    return root;
}