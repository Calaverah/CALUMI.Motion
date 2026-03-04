#pragma once

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>


class AgxStyle // : public QObject
{
    //Q_OBJECT

public:
    virtual ~AgxStyle() = default;

public:
    virtual void loadJson(const QJsonObject& json) = 0;

    virtual QJsonObject toJson() const = 0;

    /// Loads from utf-8 byte array.
    virtual void loadJsonFromByteArray(const QByteArray& byteArray)
    {
        auto json = QJsonDocument::fromJson(byteArray).object();
        loadJson(json);
    }

    virtual void loadJsonText(QString jsonText) { loadJsonFromByteArray(jsonText.toUtf8()); }

    virtual void loadJsonFile(QString fileName)
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Couldn't open file " << fileName;

            return;
        }

        loadJsonFromByteArray(file.readAll());
    }
};