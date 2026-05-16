// Copyright © 2025-2026 Calaverah. All rights reserved.
// License: https://www.gnu.org/licenses/lgpl-3.0.html
// Contact: Calaverahmedia@gmail.com

// ReSharper disable CppRedundantAccessSpecifier
#pragma once
#include "Widgets/ColorBox/AgxColorBox.h"

struct AgxIconPalette
{
    QColor IconColor = QColor(255,255,255);

    [[nodiscard]] QJsonObject toJson() const;
    void fromJson(const QJsonObject& input);
};

struct AgxGraphPalette
{
    QColor BackgroundColor = QColor(53, 53, 53);
    QColor FineGridColor = QColor(60, 60, 60);
    QColor CoarseGridColor = QColor(25, 25, 25);

    [[nodiscard]] QJsonObject toJson() const;
    void fromJson(const QJsonObject& input);

private:
    inline static auto backgroundColorTag = "background-color";
    inline static auto fineGridColorTag = "fine-grid-color";
    inline static auto coarseGridColorTag = "coarse-grid-color";
};

struct AgxNodePalette
{
    QColor NormalBoundaryColor = QColor(255, 255, 255);
    QColor SelectedBoundaryColor = QColor(255, 165, 0);
    QColor NormalStateColor = QColor(70, 70, 70);
    QColor CommentColor = QColor(60,60,60);
    QColor AlternateStateColor = QColor(Qt::cyan);

    QColor NormalFontColor = QColor(Qt::white);
    QColor AlternateFontColor = NormalFontColor;
    QColor FadedFontColor = QColor(Qt::gray);

    QColor ConnectionPointColor = QColor(169, 169, 169);
    QColor FilledConnectionPointColor = QColor(Qt::cyan);

    QColor WarningColor = QColor(255, 179, 0);
    QColor ErrorColor = QColor(211, 47, 47);
    QColor ToolTipIconColor = QColor(Qt::white);

    float PenWidth = 1.2f;
    float HoveredPenWidth = 2.25f;
    float ConnectionPointDiameter = 8.0f;
    float Opacity = 0.86f;

    void fromJson(const QJsonObject& input);
    [[nodiscard]] QJsonObject toJson() const;

private:
    inline static auto normalBoundaryColorTag ="normal-boundary-color";
    inline static auto selectedBoundaryColorTag ="selected-boundary-color";
    inline static auto normalStateColorTag ="normal-state-color";
    inline static auto commentColorTag ="comment-color";
    inline static auto alternateStateColorTag ="alternate-state-color";
    inline static auto normalFontColorTag ="normal-font-color";
    inline static auto alternateFontColorTag ="alternate-font-color";
    inline static auto fadedFontColorTag ="faded-font-color";
    inline static auto connectionPointColorTag ="connection-point-color";
    inline static auto filledConnectionPointColorTag ="filled-connection-point-color";
    inline static auto warningColorTag ="warning-color";
    inline static auto errorColorTag ="error-color";
    inline static auto tooltipIconColorTag ="tooltip-icon-color";
    inline static auto penWidthTag ="pen-width";
    inline static auto hoveredPenWidthTag ="hovered-pen-width";
    inline static auto connectionPointDiameterTag ="connection-point-diameter";
    inline static auto opacityTag ="opacity";
};

struct AgxConnectionPalette
{
    QColor ConstructionColor = QColor(Qt::gray);
    QColor NormalColor = QColor(Qt::darkCyan);
    QColor SelectedColor = QColor(100, 100, 100);
    QColor SelectedHaloColor = QColor(Qt::cyan);
    QColor HoveredColor = QColor(Qt::lightGray);

    float LineWidth = 3.0f;
    float ConstructionLineWidth = 2.0f;
    float PointDiameter = 10.0f;

    void fromJson(const QJsonObject& input);
    [[nodiscard]] QJsonObject toJson() const;

private:
    inline static auto constructionColorTag = "construction-color";
    inline static auto normalColorTag = "normal-color";
    inline static auto selectedColorTag = "selected-color";
    inline static auto selectedHaloColorTag = "selected-halo-color";
    inline static auto hoveredColorTag = "hovered-color";
    inline static auto lineWidthTag = "line-width";
    inline static auto constructionLineWidthTag = "construction-line-width";
    inline static auto pointDiameterTag = "point-diameter";
};

class AgxPalette
{
    AgxPalette() = default;
    ~AgxPalette() = default;

public:
    AgxPalette(const AgxPalette&) = delete;
    AgxPalette(AgxPalette&&) = delete;
    AgxPalette& operator=(const AgxPalette&) = delete;
    AgxPalette& operator=(AgxPalette&&) = delete;

    static AgxPalette& GetInstance()
    {
        static AgxPalette instance;
        return instance;
    }

    [[nodiscard]] QJsonObject toJson() const;
    void fromJson(const QJsonObject& input);
    void loadUserSettings();

public:
    [[nodiscard]] const AgxNodePalette& nodePalette() const;
    [[nodiscard]] const AgxConnectionPalette& connectionPalette() const;
    [[nodiscard]] const AgxGraphPalette& graphPalette() const;
    [[nodiscard]] const AgxIconPalette& iconPalette() const;

    [[nodiscard]] const QColor& panelColor() const;
    [[nodiscard]] const QColor& subPanelColor() const;

    static QPalette uiPalette();

private:
    AgxNodePalette m_nodePalette = AgxNodePalette();
    AgxConnectionPalette m_connectionPalette = AgxConnectionPalette();
    AgxGraphPalette m_graphPalette = AgxGraphPalette();
    AgxIconPalette m_iconPalette = AgxIconPalette();

    QColor m_panelColor = QColor();
    QColor m_subPanelColor = QColor();

    QColor m_iconColor = QColor();

    inline static auto nodePaletteTag = "node-palette";
    inline static auto connPaletteTag = "connection-palette";
    inline static auto uiPaletteTag = "ui-palette";
    inline static auto graphPaletteTag = "graph-palette";
};
