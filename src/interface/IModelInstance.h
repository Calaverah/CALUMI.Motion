#pragma once
#include <QQmlEngine>

namespace CALUMI::Motion
{
    class IModelInstance : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        QML_UNCREATABLE("Cannot create abstract model instance")

        Q_PROPERTY(QString modelName READ modelName WRITE setModelName NOTIFY modelNameChanged)
        Q_PROPERTY(QString sourcePath READ sourcePath NOTIFY sourcePathChanged)
    public:
        explicit IModelInstance(QObject* parent = Q_NULLPTR);
        explicit IModelInstance(const QString& objectName, QObject* parent = Q_NULLPTR );
        ~IModelInstance() override = default;

        [[nodiscard]] QString modelName() const;
        void setModelName(const QString& modelName);

    signals:
        void modelNameChanged();

    public:
        [[nodiscard]] QString sourcePath() const;
        void setSourcePath(const QString& sourcePath);

    signals:
        void sourcePathChanged();

    public:
        Q_INVOKABLE [[nodiscard]] virtual QString saveToFile(const QString& filePath) const = 0;
        Q_INVOKABLE virtual bool loadFromFile(const QString& filePath) = 0;

        [[nodiscard]] virtual QJsonObject save() const = 0;
        virtual bool load(const QJsonObject& object) = 0;

    private:
        static constexpr auto s_defaultName  = "Unnamed Model Instance";
        QString m_instanceName = s_defaultName;
        QString m_sourcePath = "";
    };
}
