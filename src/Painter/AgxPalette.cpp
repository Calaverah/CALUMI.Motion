// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

#include "AgxPalette.h"

#include <qfileinfo.h>
#include <QJsonObject>
#include "Application/CALUMIMotionApplication.h"
#include "../Utilities/Settings/SettingsRegistry.h"

void AgxGraphPalette::fromJson(const QJsonObject& input)
{
    *this = AgxGraphPalette();

    if (input.contains(backgroundColorTag))
    {
        if (const auto color = QColor::fromString(input[backgroundColorTag].toString()); color.isValid())
            BackgroundColor = color;
    }

    if (input.contains(fineGridColorTag))
    {
        if (const auto color = QColor::fromString(input[fineGridColorTag].toString()); color.isValid())
            FineGridColor = color;
    }

    if (input.contains(coarseGridColorTag))
    {
        if (const auto color = QColor::fromString(input[coarseGridColorTag].toString()); color.isValid())
            CoarseGridColor = color;
    }
}

QJsonObject AgxIconPalette::toJson() const
{
    QJsonObject output;

    output["icon-color"] = IconColor.name(QColor::HexRgb);

    return output;
}

void AgxIconPalette::fromJson(const QJsonObject& input)
{
    if (input.contains("icon-color"))
    {
        if (QColor color = input["icon-color"].toString(); color.isValid())
        {
            color.setAlpha(255);
            IconColor = color;
        }
    }
}

QJsonObject AgxGraphPalette::toJson() const
{
    QJsonObject output;

    output[backgroundColorTag] = BackgroundColor.name(QColor::HexArgb);
    output[fineGridColorTag] = FineGridColor.name(QColor::HexArgb);
    output[coarseGridColorTag] = CoarseGridColor.name(QColor::HexArgb);

    return output;
}

void AgxNodePalette::fromJson(const QJsonObject& input)
{
    *this = AgxNodePalette();

    if (input.contains(normalBoundaryColorTag))
        if (const auto color = QColor::fromString(input[normalBoundaryColorTag].toString()); color.isValid())
            NormalBoundaryColor = color;

    if (input.contains(selectedBoundaryColorTag))
        if (const auto color = QColor::fromString(input[selectedBoundaryColorTag].toString()); color.isValid())
            SelectedBoundaryColor = color;

    if (input.contains(normalStateColorTag))
        if (const auto color = QColor::fromString(input[normalStateColorTag].toString()); color.isValid())
            NormalStateColor = color;

    if (input.contains(commentColorTag))
        if (const auto color = QColor::fromString(input[commentColorTag].toString()); color.isValid())
            CommentColor = color;

    if (input.contains(alternateStateColorTag))
        if (const auto color = QColor::fromString(input[alternateStateColorTag].toString()); color.isValid())
            AlternateStateColor = color;

    if (input.contains(normalFontColorTag))
        if (const auto color = QColor::fromString(input[normalFontColorTag].toString()); color.isValid())
            NormalFontColor = color;

    if (input.contains(alternateFontColorTag))
        if (const auto color = QColor::fromString(input[alternateFontColorTag].toString()); color.isValid())
            AlternateFontColor = color;

    if (input.contains(fadedFontColorTag))
        if (const auto color = QColor::fromString(input[fadedFontColorTag].toString()); color.isValid())
            FadedFontColor = color;

    if (input.contains(connectionPointColorTag))
        if (const auto color = QColor::fromString(input[connectionPointColorTag].toString()); color.isValid())
            ConnectionPointColor = color;

    if (input.contains(filledConnectionPointColorTag))
        if (const auto color = QColor::fromString(input[filledConnectionPointColorTag].toString()); color.isValid())
            FilledConnectionPointColor = color;

    if (input.contains(warningColorTag))
        if (const auto color = QColor::fromString(input[warningColorTag].toString()); color.isValid())
            WarningColor = color;

    if (input.contains(errorColorTag))
        if (const auto color = QColor::fromString(input[errorColorTag].toString()); color.isValid())
            ErrorColor = color;

    if (input.contains(tooltipIconColorTag))
        if (const auto color = QColor::fromString(input[tooltipIconColorTag].toString()); color.isValid())
            ToolTipIconColor = color;

    if (input.contains(penWidthTag))
        if (const auto width = input[penWidthTag].toDouble(); !qIsNaN(width))
            PenWidth = static_cast<float>(width);

    if (input.contains(hoveredPenWidthTag))
        if (const auto width = input[hoveredPenWidthTag].toDouble(); !qIsNaN(width))
            HoveredPenWidth = static_cast<float>(width);

    if (input.contains(connectionPointDiameterTag))
        if (const auto width = input[connectionPointDiameterTag].toDouble(); !qIsNaN(width))
            ConnectionPointDiameter = static_cast<float>(width);

    if (input.contains(opacityTag))
        if (const auto width = input[opacityTag].toDouble(); !qIsNaN(width))
            Opacity = static_cast<float>(width);
}

QJsonObject AgxNodePalette::toJson() const
{
    QJsonObject output;

    output[normalBoundaryColorTag] = NormalBoundaryColor.name(QColor::HexArgb);
    output[selectedBoundaryColorTag] = SelectedBoundaryColor.name(QColor::HexArgb);
    output[normalStateColorTag] = NormalStateColor.name(QColor::HexArgb);
    output[commentColorTag] = CommentColor.name(QColor::HexArgb);
    output[alternateStateColorTag] = AlternateStateColor.name(QColor::HexArgb);
    output[normalFontColorTag] = NormalFontColor.name(QColor::HexArgb);
    output[alternateFontColorTag] = AlternateFontColor.name(QColor::HexArgb);
    output[fadedFontColorTag] = FadedFontColor.name(QColor::HexArgb);
    output[connectionPointColorTag] = ConnectionPointColor.name(QColor::HexArgb);
    output[filledConnectionPointColorTag] = FilledConnectionPointColor.name(QColor::HexArgb);
    output[warningColorTag] = WarningColor.name(QColor::HexArgb);
    output[errorColorTag] = ErrorColor.name(QColor::HexArgb);
    output[tooltipIconColorTag] = ToolTipIconColor.name(QColor::HexArgb);
    output[penWidthTag] = PenWidth;
    output[hoveredPenWidthTag] = HoveredPenWidth;
    output[connectionPointDiameterTag] = ConnectionPointDiameter;
    output[opacityTag] = Opacity;

    return output;
}

void AgxConnectionPalette::fromJson(const QJsonObject& input)
{
    *this = AgxConnectionPalette();

    if (input.contains(constructionColorTag))
        if (const auto color = QColor::fromString(input[constructionColorTag].toString()); color.isValid())
            ConstructionColor = color;

    if (input.contains(normalColorTag))
        if (const auto color = QColor::fromString(input[normalColorTag].toString()); color.isValid())
            NormalColor = color;

    if (input.contains(selectedColorTag))
        if (const auto color = QColor::fromString(input[selectedColorTag].toString()); color.isValid())
            SelectedColor = color;

    if (input.contains(selectedHaloColorTag))
        if (const auto color = QColor::fromString(input[selectedHaloColorTag].toString()); color.isValid())
            SelectedHaloColor = color;

    if (input.contains(hoveredColorTag))
        if (const auto color = QColor::fromString(input[hoveredColorTag].toString()); color.isValid())
            HoveredColor = color;

    if (input.contains(lineWidthTag))
        if (const auto width = input[lineWidthTag].toDouble(); !qIsNaN(width))
            LineWidth = static_cast<float>(width);

    if (input.contains(constructionLineWidthTag))
        if (const auto width = input[constructionLineWidthTag].toDouble(); !qIsNaN(width))
            ConstructionLineWidth = static_cast<float>(width);

    if (input.contains(pointDiameterTag))
        if (const auto width = input[pointDiameterTag].toDouble(); !qIsNaN(width))
            PointDiameter = static_cast<float>(width);
}

QJsonObject AgxConnectionPalette::toJson() const
{
    QJsonObject output;

    output[constructionColorTag] = ConstructionColor.name(QColor::HexArgb);
    output[normalColorTag] = NormalColor.name(QColor::HexArgb);
    output[selectedColorTag] = SelectedColor.name(QColor::HexArgb);
    output[selectedHaloColorTag] = SelectedHaloColor.name(QColor::HexArgb);
    output[hoveredColorTag] = HoveredColor.name(QColor::HexArgb);
    output[lineWidthTag] = LineWidth;
    output[constructionLineWidthTag] = ConstructionLineWidth;
    output[pointDiameterTag] = PointDiameter;

    return output;
}

QJsonObject AgxPalette::toJson() const
{
    QJsonObject output;

    if (const auto nodePalette = m_nodePalette.toJson(); !nodePalette.empty())
        output[nodePaletteTag] = nodePalette;

    if (const auto connPalette = m_connectionPalette.toJson(); !connPalette.empty())
        output[connPaletteTag] = connPalette;

    if (const auto graphPalette = m_graphPalette.toJson(); !graphPalette.empty())
        output[graphPaletteTag] = graphPalette;

    QJsonObject uiPalette;
    QJsonObject panel;
    QJsonObject subPanel;

    panel["color"] = m_panelColor.name(QColor::HexArgb);
    subPanel["color"] = m_subPanelColor.name(QColor::HexArgb);

    const auto palette = AgxPalette::uiPalette();

    uiPalette["panel"] = panel;
    uiPalette["sub-panel"] = subPanel;
    uiPalette["window"] = palette.color(QPalette::Window).name(QColor::HexArgb);
    uiPalette["window-text"] = palette.color(QPalette::WindowText).name(QColor::HexArgb);
    uiPalette["base"] = palette.color(QPalette::Base).name(QColor::HexArgb);
    uiPalette["alt-base"] = palette.color(QPalette::AlternateBase).name(QColor::HexArgb);
    uiPalette["highlight"] = palette.color(QPalette::Highlight).name(QColor::HexArgb);
    uiPalette["highlight-text"] = palette.color(QPalette::HighlightedText).name(QColor::HexArgb);
    uiPalette["button"] = palette.color(QPalette::Button).name(QColor::HexArgb);
    uiPalette["button-text"] = palette.color(QPalette::ButtonText).name(QColor::HexArgb);


    const QJsonObject iconPalette = m_iconPalette.toJson();
    output["icon-palette"] = iconPalette;

    output[uiPaletteTag] = uiPalette;

    return output;
}

void AgxPalette::fromJson(const QJsonObject& input)
{
    const QJsonObject nodePalette = input[nodePaletteTag].toObject();
    const QJsonObject connPalette = input[connPaletteTag].toObject();
    const QJsonObject uiPalette = input[uiPaletteTag].toObject();
    const QJsonObject iconPalette = input["icon-palette"].toObject();

    m_connectionPalette.fromJson(connPalette);
    m_nodePalette.fromJson(nodePalette);
    m_iconPalette.fromJson(iconPalette);

    if (uiPalette.contains("panel-color"))
    {
        if (const auto panelColor = QColor::fromString(uiPalette["panel-color"].toString()); panelColor.isValid())
            m_panelColor = panelColor;
    }

    if (uiPalette.contains("subpanel-color"))
    {
        if (const auto subPanelColor = QColor::fromString(uiPalette["subpanel-color"].toString()); subPanelColor.isValid())
            m_subPanelColor = subPanelColor;
    }

    auto newPalette = qApp->palette();

    if (uiPalette.contains("window"))
    {
        if (const auto windowColor = QColor::fromString(uiPalette["window"].toString()); windowColor.isValid())
            newPalette.setColor(QPalette::Window, windowColor);
    }

    if (uiPalette.contains("window-text"))
    {
        if (const auto windowTextColor = QColor::fromString(uiPalette["window-text"].toString()); windowTextColor.isValid())
            newPalette.setColor(QPalette::WindowText, windowTextColor);
    }

    if (uiPalette.contains("base"))
    {
        if (const auto baseColor = QColor::fromString(uiPalette["base"].toString()); baseColor.isValid())
            newPalette.setColor(QPalette::Base, baseColor);
    }

    if (uiPalette.contains("alt-base"))
    {
        if (const auto altBaseColor = QColor::fromString(uiPalette["alt-base"].toString()); altBaseColor.isValid())
            newPalette.setColor(QPalette::AlternateBase, altBaseColor);
    }

    if (uiPalette.contains("highlight"))
    {
        if (const auto highlightColor = QColor::fromString(uiPalette["highlight"].toString()); highlightColor.isValid())
            newPalette.setColor(QPalette::Highlight, highlightColor);
    }

    if (uiPalette.contains("highlight-text"))
    {
        if (const auto highlightTextColor = QColor::fromString(uiPalette["highlight-text"].toString()); highlightTextColor.isValid())
            newPalette.setColor(QPalette::HighlightedText, highlightTextColor);
    }

    if (uiPalette.contains("button"))
    {
        if (const auto buttonColor = QColor::fromString(uiPalette["button"].toString()); buttonColor.isValid())
            newPalette.setColor(QPalette::Button, buttonColor);
    }

    if (uiPalette.contains("button-text"))
    {
        if (const auto buttonTextColor = QColor::fromString(uiPalette["button-text"].toString()); buttonTextColor.isValid())
            newPalette.setColor(QPalette::ButtonText, buttonTextColor);
    }

    qApp->setPalette(newPalette);
}

void AgxPalette::loadUserSettings()
{
    QString filePath = SettingsRegistry::GetInstance().GetThemeFilePath();

    if (const QFileInfo fileInfo(filePath); !fileInfo.exists() || fileInfo.fileName().isEmpty())
    {
        qInfo() << "Loading Default Node Theme";
        // This configuration is stored inside the compiled unit and is loaded statically
        // However we have a backup style in the initialization if all else fails
        filePath = ":/CALUMIMotion/Resources/DefaultStyle.json";
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Couldn't open file at: " << filePath;

        return;
    }

    const auto json = QJsonDocument::fromJson(file.readAll()).object();
    fromJson(json);
}

const AgxNodePalette& AgxPalette::nodePalette() const
{
    return m_nodePalette;
}

const AgxConnectionPalette& AgxPalette::connectionPalette() const
{
    return m_connectionPalette;
}

const AgxGraphPalette& AgxPalette::graphPalette() const
{
    return m_graphPalette;
}

const AgxIconPalette& AgxPalette::iconPalette() const
{
    return m_iconPalette;
}

const QColor& AgxPalette::panelColor() const
{
    return m_panelColor;
}
const QColor& AgxPalette::subPanelColor() const
{
    return m_subPanelColor;
}

QPalette AgxPalette::uiPalette()
{
    return CALUMIMotionApplication::palette();
}
