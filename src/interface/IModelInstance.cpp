#include "IModelInstance.h"

namespace CALUMI::Motion
{
    IModelInstance::IModelInstance(QObject* parent) : IModelInstance(s_defaultName, parent)
    {
    }

    IModelInstance::IModelInstance(const QString& objectName, QObject* parent)
    : QObject(parent), m_instanceName(objectName.isEmpty() ? s_defaultName : objectName)
    {

    }

    QString IModelInstance::modelName() const
    {
        return m_instanceName;
    }

    void IModelInstance::setModelName(const QString& modelName)
    {
        const auto newName = modelName.isEmpty() ? s_defaultName : modelName;

        if (newName == m_instanceName)
            return;

        m_instanceName = newName;
        Q_EMIT modelNameChanged();
    }

    QString IModelInstance::sourcePath() const
    {
        return m_sourcePath;
    }

    void IModelInstance::setSourcePath(const QString& sourcePath)
    {
        if (sourcePath == m_sourcePath)
            return;

        m_sourcePath = sourcePath;
        Q_EMIT sourcePathChanged();
    }
}
