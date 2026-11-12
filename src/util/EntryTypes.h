// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once
#include <qqmlintegration.h>
#include <QString>
#include <QVariant>
#include <QVector3D>
#include "Utilities/AgxDictionary.h"

namespace CALUMI::Motion
{
    class IProperty
    {
    public:
        virtual ~IProperty() = default;

        [[nodiscard]] virtual QString label() const = 0;
        [[nodiscard]] virtual QString key() const = 0;
        [[nodiscard]] virtual QVariant value() const = 0;

        virtual bool setValue(QVariant value) = 0;
    };

    class BasicInt : public IProperty
    {
    public:
        BasicInt(TermRef id, qint64 value = 0);

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;

    private:
        qint64 m_value = 0;
        TermRef m_id = nullptr;
    };

    class VariableInt : public IProperty
    {
    public:
        VariableInt(TermRef id, std::variant<qint64, QString> value = 0);

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;
    private:
        std::variant<qint64, QString> m_value;
        TermRef m_id = nullptr;
    };

    class BasicFloat : public IProperty
    {
    public:
        BasicFloat(TermRef id, double value = 0);

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;

    private:
        double m_value = 0.0;
        TermRef m_id = nullptr;
    };

    class VariableFloat : public IProperty
    {
    public:
        VariableFloat(TermRef id, std::variant<double, QString> value = 0.0);

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;

    private:
        std::variant<double, QString> m_value;
        TermRef m_id = nullptr;
    };

    class BasicVector : public IProperty
    {
    public:
        BasicVector(TermRef id, QVector3D value = QVector3D());

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;

    private:
        QVector3D m_value;
        TermRef m_id = nullptr;
    };

    class BasicString : public IProperty
    {
    public:
        BasicString(TermRef id, QString value = QString());

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;
    private:
        QString m_value;
        TermRef m_id = nullptr;
    };

    class GameVariable : public IProperty
    {
    public:
        GameVariable(TermRef id, QString value = QString());

        [[nodiscard]] QString label() const override;
        [[nodiscard]] QString key() const override;
        [[nodiscard]] QVariant value() const override;

        bool setValue(QVariant value) override;
    private:
        QString m_value;
        TermRef m_id = nullptr;
        quint32 m_type = 0;
    };




}
