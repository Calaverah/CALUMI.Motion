//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

#pragma once
#pragma warning(push,0)
#include <QtWidgets/QMainWindow>
#include "ui_CALUMIMotion.h"
#pragma warning(pop)

#include "Models/AgxGraphModel.h"
#include "Models/AgxGraphicsScene.h"
#include "Widgets/AgxGraphicsView.h"
#include "Widgets/FilteredDropDownDialog.h"
#include "Widgets/MultiVariableDialog.h"

struct TabDataPair
{
    std::shared_ptr<AgxGraphModel> _AgxGraphModel;
    std::shared_ptr<AgxGraphicsScene> _AgxGraphicsScene;

    TabDataPair(const std::shared_ptr<AgxGraphModel>& _AgxGraphModel, const std::shared_ptr<AgxGraphicsScene>& _AgxGraphicsScene);
};

class CALUMIMotion : public QMainWindow
{
    Q_OBJECT

public:
    CALUMIMotion(QWidget *parent = nullptr);
    ~CALUMIMotion();

    friend TabDataPair;
    //std::shared_ptr<QtNodes::NodeDelegateModelRegistry> GetRegistry() const;

    bool HasTab(QWidget* widget);
    void CloseTab(QWidget* widget);
    bool HasScene(AgxGraphicsScene* scene);
    AgxGraphicsView* GetAgxViewFromTab(int idx);

public:
    void UpdateTabTitles();

signals:
    void LanguageChanged();

private slots:
    //void NewTab_SFBGS();
    void CloseTab(int i);

    void TogglePropertiesSidebar();
    void ToggleApplicationConsole() const;
    
    void ShowNodeGroupMenu();
    void HandleNodeGroupMenuVisibility() const;

    void ShowAboutDialog();

    void GetEditMenu();
    void GetFileMenu();
    void GetViewMenu();
    void GetHelpMenu();

private:
    void BuildUndoViewMenu();
    void BuildCutCopyPasteMenu();
    void BuildCenterViewMenu();
    void BuildItemSelectionMenu();
    void BuildFileInOutMenu();
    void BuildSettingsMenu();
    void BuildGraphEditMenu();

public slots:
    void Create_SFBGSTab(std::shared_ptr<AgxGraphicsScene> scene = nullptr, std::shared_ptr<AgxGraphModel> model = nullptr);
    void ImportFile_Agx_SFBGS();
    void ExportFile_Agx_SFBGS();
    void OpenFile_Behavior_SFBGS(QJsonObject& object);

public slots:
    void onSave();
    void onSaveAs();
    void onOpen();

protected:
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    Ui::CALUMIMotionClass ui;
    mutable std::map<AgxGraphicsView*, TabDataPair> tabMap;
    bool _showPropertiesSidebar = true;
};
