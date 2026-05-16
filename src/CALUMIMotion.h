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

struct TabDataPair
{
    std::shared_ptr<AgxGraphModel> m_AgxGraphModel;
    std::shared_ptr<AgxGraphicsScene> m_AgxGraphicsScene;

    TabDataPair(const std::shared_ptr<AgxGraphModel>& agxGraphModel, const std::shared_ptr<AgxGraphicsScene>& agxGraphicsScene);
};

class CALUMIMotion : public QMainWindow
{
    Q_OBJECT

public:
    explicit CALUMIMotion(QWidget *parent = nullptr);
    ~CALUMIMotion() override;

    friend TabDataPair;
    //std::shared_ptr<QtNodes::NodeDelegateModelRegistry> GetRegistry() const;

    bool HasTab(const QWidget* widget) const;
    void CloseTab(const QWidget* widget) const;
    bool HasScene(const AgxGraphicsScene* scene) const;
    AgxGraphicsView* GetAgxViewFromTab(int idx) const;

void UpdateTabTitles() const;

signals:
    void LanguageChanged();

private slots:
    //void NewTab_SFBGS();
    void CloseTab(int i) const;

    void TogglePropertiesSidebar();
    static void ToggleApplicationConsole();
    
    void ShowNodeGroupMenu() const;
    void HandleNodeGroupMenuVisibility() const;

    void ShowAboutDialog();

    void GetEditMenu();
    void GetFileMenu() const;
    void GetViewMenu();
    void GetHelpMenu();

private:
    void BuildUndoViewMenu();
    void BuildCutCopyPasteMenu();
    void BuildCenterViewMenu();
    void BuildItemSelectionMenu();
    void BuildFileInOutMenu() const;
    void BuildSettingsMenu();
    void BuildGraphEditMenu();

public slots:
    void Create_SFBGSTab(std::shared_ptr<AgxGraphicsScene> scene = nullptr, std::shared_ptr<AgxGraphModel> model = nullptr);
    void ImportFile_Agx_SFBGS();
    void ExportFile_Agx_SFBGS();
    void OpenFile_Behavior_SFBGS(const QJsonObject& object);

public slots:
    void onSave();
    void onSaveAs();
    void onOpen();

protected:
    void closeEvent(QCloseEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    Ui::CALUMIMotionClass ui{};
    mutable std::map<AgxGraphicsView*, TabDataPair> tabMap;
    bool _showPropertiesSidebar = true;
};
