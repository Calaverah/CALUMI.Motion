#include "EntryTypes.h"
#include <QDebug>
#include <QVariant>
#include <utility>

namespace CALUMI::Motion
{


    BasicInt::BasicInt(const TermRef id, const qint64 value) : m_value(value), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Basic Int is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString BasicInt::label() const { return m_id().translation; }
    QString BasicInt::key() const { return m_id().tag; }
    QVariant BasicInt::value() const { return m_value; }

    bool BasicInt::setValue(const QVariant value)
    {
        if (value.canConvert(QMetaType::fromType<qint64>()))
        {
            m_value = value.value<qint64>();
            return true;
        }
        return false;
    }

    VariableInt::VariableInt(const TermRef id, std::variant<qint64, QString> value) : m_value(std::move(value)), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Variable Int is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString VariableInt::label() const { return m_id().translation; }
    QString VariableInt::key() const { return m_id().tag; }
    QVariant VariableInt::value() const { return QVariant::fromStdVariant(m_value); }

    bool VariableInt::setValue(const QVariant value)
    {
        if (value.canConvert<qint64>())
        {
            m_value = value.value<qint64>();
            return true;
        }
        if (value.canConvert<QString>())
        {
            m_value = value.value<QString>();
            return true;
        }
        return false;
    }

    BasicFloat::BasicFloat(const TermRef id, const double value) : m_value(value), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Basic Float is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString BasicFloat::label() const { return m_id().translation; }
    QString BasicFloat::key() const { return m_id().tag; }
    QVariant BasicFloat::value() const { return m_value; }

    bool BasicFloat::setValue(const QVariant value)
    {
        if (value.canConvert<double>())
        {
            m_value = value.value<double>();
            return true;
        }
        return false;
    }

    VariableFloat::VariableFloat(const TermRef id, std::variant<double, QString> value) : m_value(std::move(value)), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Variable Float is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString VariableFloat::label() const { return m_id().translation; }
    QString VariableFloat::key() const { return m_id().tag; }
    QVariant VariableFloat::value() const { return QVariant::fromStdVariant(m_value); }

    bool VariableFloat::setValue(const QVariant value)
    {
        if (value.canConvert<double>())
        {
            m_value = value.value<double>();
            return true;
        }
        if (value.canConvert<QString>())
        {
            m_value = value.value<QString>();
            return true;
        }
        return false;
    }

    BasicVector::BasicVector(const TermRef id, const QVector3D value) : m_value(value), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Basic Vector is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString BasicVector::label() const { return m_id().translation; }
    QString BasicVector::key() const { return m_id().tag; }
    QVariant BasicVector::value() const { return m_value; }

    bool BasicVector::setValue(const QVariant value)
    {
        if (value.canConvert<QVector3D>())
        {
            m_value = value.value<QVector3D>();
            return true;
        }
        return false;
    }


    BasicString::BasicString(const TermRef id, QString value) : m_value(std::move(value)), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Basic String is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString BasicString::label() const { return m_id().translation; }
    QString BasicString::key() const { return m_id().tag; }
    QVariant BasicString::value() const { return m_value; }

    bool BasicString::setValue(const QVariant value)
    {
        if (value.canConvert<QString>())
        {
            m_value = value.value<QString>();
            return true;
        }
        return false;
    }

    GameVariable::GameVariable(const TermRef id, QString value) : m_value(std::move(value)), m_id(id)
    {
        if (!m_id)
        {
            qWarning() << Q_FUNC_INFO << " Game Variable is given nullptr term ref";
            m_id = AgxDictionary::ErrorTerm;
        }
    }

    QString GameVariable::label() const { return m_id().translation; }
    QString GameVariable::key() const { return m_id().tag; }
    QVariant GameVariable::value() const { return m_value; }

    bool GameVariable::setValue(const QVariant value)
    {
        if (value.canConvert<QString>())
        {
            m_value = value.value<QString>();
            return true;
        }
        return false;
    }

}
