// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#include "SFBGS_RigModel.h"

SFBGS_RigModel::SFBGS_RigModel(QObject* parent) : QObject(parent)
{
}

void SFBGS_RigModel::setSourceFilePath(const QString& sourceFilePath)
{
    if (m_sourceFilePath != sourceFilePath)
    {
        m_sourceFilePath = sourceFilePath;
        Q_EMIT sourceFilePathChanged(m_sourceFilePath);
    }
}

void SFBGS_RigModel::setRig(const CALUMI::SFBGS::SkeletonRig& rig)
{
    m_rig = rig;
    Q_EMIT rigChanged(m_rig);
}

void SFBGS_RigModel::setLowPrecision(const float precision)
{
    if (precision > 0.0f && m_rig.lowPrecision() != precision)
    {
        m_rig.setLowPrecision(precision);
        Q_EMIT lowPrecisionChanged(m_rig.lowPrecision());
    }
}

void SFBGS_RigModel::setHighPrecision(const float precision)
{
    if (precision > 0.0f && m_rig.highPrecision() != precision)
    {
        m_rig.setHighPrecision(precision);
        Q_EMIT highPrecisionChanged(m_rig.highPrecision());
    }
}
