//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QtWidgets/QMainWindow>
// ReSharper disable once CppUnusedIncludeDirective
#include <QFileInfo>
#include "ui_CALUMIMotion.h"
#pragma warning(pop)

#include "Models/AgxGraphModel.h"
#include "Models/AgxGraphicsScene.h"
#include "Widgets/AgxGraphicsView.h"

class CALUMIMotion : public QMainWindow
{
    Q_OBJECT

public:
    explicit CALUMIMotion(QWidget *parent = nullptr);
    ~CALUMIMotion() override;

    bool HasTab(const QWidget* widget) const;
    void CloseTab(const QWidget* widget) const;
    bool HasScene(const AgxGraphicsScene* scene) const;
    [[nodiscard]] AgxGraphicsView* GetAgxViewFromTab(int idx) const;

    void UpdateTabTitles() const;

signals:
    void LanguageChanged();

public slots:
    void ShowNodeGroupMenu();

private slots:
    //void NewTab_SFBGS();
    void CloseTab(int i) const;

    void ShowAboutDialog();
    void BuildSettingsMenu();
    void GetFileMenu() const;

    void onTabChanged(int i) const;

private:
    void BuildFileInOutMenu() const;

public slots:
    void Create_SFBGSTab(std::shared_ptr<AgxGraphicsScene> scene = nullptr, std::shared_ptr<AgxGraphModel> model = nullptr);
    void ImportFile_Agx_SFBGS();
    void ExportFile_Agx_SFBGS();
    void OpenFile_Behavior_SFBGS(const QJsonObject& object);

    void onLoadFile(QString filePath = {});
    //void onSaveFile();

public slots:
    void onSave();
    void onSaveAs();
    void onOpen();

protected:
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    Ui::CALUMIMotionClass ui{};
    mutable std::map<AgxGraphicsView*, AgxGraphicsScene&> tabMap;

    friend class ITabWidget;
};
