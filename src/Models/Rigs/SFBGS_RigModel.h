// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#pragma once
#include <QObject>
#include <QString>
#include <QDebug>

#include <AnimStarfield>

class SFBGS_RigModel : public QObject {
    Q_OBJECT
public:
    explicit SFBGS_RigModel(QObject* parent = nullptr);
    ~SFBGS_RigModel() override = default;

    [[nodiscard]] const QString& sourceFilePath() const { return m_sourceFilePath; }
    void setSourceFilePath(const QString& sourceFilePath);

    [[nodiscard]] const CALUMI::SFBGS::SkeletonRig& rig() const { return m_rig; }
    void setRig(const CALUMI::SFBGS::SkeletonRig& rig);

//Internal Members - Get Only
     [[nodiscard]] int versionNumber() const { return m_rig.versionNumber(); }
     [[nodiscard]] uint32_t fileSize() const { return m_rig.fileSize(); }
     [[nodiscard]] uint64_t boneArrayOffset() const { return m_rig.headerSize(); }
     [[nodiscard]] uint64_t boneMapOffset() const { return m_rig.boneMapOffset(); }
     [[nodiscard]] QList<uint64_t> matchingThreeOffsets() const
     {
         auto three = m_rig.matchingThree();
         return {three.at(0), three.at(1), three.at(2)};
     }
    [[nodiscard]] uint16_t headerBoneCount() const { return m_rig.boneCount(); }
    [[nodiscard]] uint16_t headerBoneCountAnimated() const { return m_rig.boneCountAnimated(); }

//Internal Members
    [[nodiscard]] float lowPrecision() const { return m_rig.lowPrecision(); }
    void setLowPrecision(float precision);

    [[nodiscard]] float highPrecision() const { return m_rig.highPrecision(); }
    void setHighPrecision(float precision);

//Access Functions
    [[nodiscard]] int boneCount() const { return m_rig.boneCount(); }
    [[nodiscard]] const CALUMI::SFBGS::SkeletonBone& bone(const int idx) const { return m_rig.boneEntries().at(idx); }
    [[nodiscard]] QString boneName(const int idx) const { return m_rig.stringArray().c_str(idx); }



Q_SIGNALS:
    void sourceFilePathChanged(const QString& sourceFilePath);
    void rigChanged(const CALUMI::SFBGS::SkeletonRig& rig);
    void lowPrecisionChanged(float precision);
    void highPrecisionChanged(float precision);
    void boneEntriesChanged();
    void boneAdded(int idx);
    void boneRemoved(int idx);
    void boneChanged(int idx);

private:
    QString m_sourceFilePath;
    CALUMI::SFBGS::SkeletonRig m_rig;
};

