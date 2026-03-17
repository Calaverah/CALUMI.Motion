#include "stdafx.h"
#include "CALUMIMotion.h"
#include <windows.h>
#include "Widgets/AgxGraphicsView.h"
#include "Widgets/NodeGroupMenuPopup.h"
#include <format>
#include <string>
#include <Utilities/AgxDefinitions.h>
#include "Utilities/DialogPool.h"
#include "Widgets/MultiVariableDialog.h"
#include "Widgets/CALUMITabModule.h"
#include "Widgets/SFBGS/SFBGS_GraphPropertiesDialogWidget.h"

#include <Widgets/Dialog/AgxProgressDialog.h>
#include <Utilities/SettingsRegistry.h>
#include "Application/CALUMIMotionApplication.h"
#include "Widgets/Settings/SettingsDialog.h"
#include "Utilities/UndoRedoCommands.h"

CALUMIMotion::CALUMIMotion(QWidget *parent) : QMainWindow(parent)
{
    setObjectName("CALUMIMotionObject");
    ui.setupUi(this);

    setWindowTitle(windowTitle() + " [" + QCoreApplication::applicationVersion() + "]");

    connect(ui.actionOpen, &QAction::triggered, this, &CALUMIMotion::onOpen);
    connect(ui.actionSave, &QAction::triggered, this, &CALUMIMotion::onSave);
    connect(ui.actionSave_As, &QAction::triggered, this, &CALUMIMotion::onSaveAs);

    connect(ui.actionSFBGS_Behavior_Graph_import, &QAction::triggered, this, &CALUMIMotion::ImportFile_Agx_SFBGS);
    connect(ui.actionSFBGS_Behavior_Graph_export, &QAction::triggered, this, &CALUMIMotion::ExportFile_Agx_SFBGS);
    
    connect(ui.actionNode_Groups, &QAction::triggered, this, &CALUMIMotion::ShowNodeGroupMenu);

    connect(ui.menuView, &QMenu::aboutToShow, this, &CALUMIMotion::GetViewMenu);
    connect(ui.menuEdit, &QMenu::aboutToShow, this, &CALUMIMotion::GetEditMenu);
    connect(ui.menuFile, &QMenu::aboutToShow, this, &CALUMIMotion::GetFileMenu);
    connect(ui.menuHelp, &QMenu::aboutToShow, this, &CALUMIMotion::GetHelpMenu);

    connect(ui.actionAbout, &QAction::triggered, this, &CALUMIMotion::ShowAboutDialog);
    
    auto& settings = SettingsRegistry::GetInstance();

    const QByteArray& bytes = settings.GetSavedWindowGeometry("Geometry");
    
    if (settings.UseSavedGeometry() && !bytes.isEmpty())
        restoreGeometry(settings.GetSavedWindowGeometry("Geometry"));
    else
        resize(1280, 800);
    StartupVisibiltyPreference propPref = settings.GetPropertySidebarVisibilityPreference();
    switch (propPref)
    {
        case StartupVisibiltyPreference::Never:
            _showPropertiesSidebar = false;
            break;
        case StartupVisibiltyPreference::Remember:
            _showPropertiesSidebar = settings.GetLastState("Sidebar/State", _showPropertiesSidebar);
            break;
        case StartupVisibiltyPreference::Always:
            _showPropertiesSidebar = true;
            break;
    }

}

CALUMIMotion::~CALUMIMotion() {
    SettingsRegistry::GetInstance().SaveWindowGeometry("Geometry", saveGeometry());
    
}

void CALUMIMotion::closeEvent(QCloseEvent* event)
{
    if (auto calumiApp = dynamic_cast<CALUMIMotionApplication*>(QCoreApplication::instance()))
    {
        size_t topLevelCount = 0;
        
        for (auto window : calumiApp->topLevelWindows())
        {
            //TBD: there has to be a better solution for this but whatever.
            if (window->objectName() == "CALUMIMotionObjectWindow" && window->isVisible())
            {
                topLevelCount++;
            }
        }

        auto& settingIns = SettingsRegistry::GetInstance();
        
        settingIns.SaveLastState("Log/State", calumiApp->LoggerVisibile());

        if(topLevelCount <= 1)
        {
            calumiApp->HideLogger();
        }
    }

    QMainWindow::closeEvent(event);
}

void CALUMIMotion::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        ui.retranslateUi(this);
        setWindowTitle(windowTitle() + " [" + QCoreApplication::applicationVersion() + "]");

        Q_EMIT LanguageChanged();
    }
    QMainWindow::changeEvent(event);
}

void CALUMIMotion::CloseTab(int i)
{
    if (ui.tabWidget && ui.tabWidget->count()>i && i >= 0)
    {
        //ui.tabWidget->removeTab(i);
        if (auto agxView = GetAgxViewFromTab(i))
        {
            if (agxView->pagxNodeScene())
            {
                agxView->pagxNodeScene()->agxGraphModel().HandleEmbeddedClosures();
            }
            if (tabMap.contains(agxView))
            {
                tabMap.erase(agxView);
            }
            
        }
        
        ui.tabWidget->widget(i)->deleteLater();
        HandleNodeGroupMenuVisibility();
    }
}

void CALUMIMotion::TogglePropertiesSidebar()
{
    _showPropertiesSidebar = !_showPropertiesSidebar;

    if (ui.tabWidget)
    {
        for (int i = 0; i < ui.tabWidget->count(); i++)
        {
            if (auto tab = dynamic_cast<CALUMITabModule*>(ui.tabWidget->widget(i)))
            {
                tab->SetSidebarVisibility_Right(_showPropertiesSidebar);
            }
        }
    }

    SettingsRegistry::GetInstance().SaveLastState("Sidebar/State", _showPropertiesSidebar);
}

void CALUMIMotion::ToggleApplicationConsole() const
{
    if (auto calumiApp = dynamic_cast<CALUMIMotionApplication*>(QCoreApplication::instance()))
    {
        calumiApp->ToggleLogger();
    }
}

void CALUMIMotion::CloseTab(QWidget* widget)
{
    if (!widget) return;

    if (ui.tabWidget)
    {
        for (int i = 0; i < ui.tabWidget->count(); i++)
        {
            if (widget == ui.tabWidget->widget(i) || widget == GetAgxViewFromTab(i))
            {
                CloseTab(i);
            }
        }
    }
}

bool CALUMIMotion::HasScene(AgxGraphicsScene* scene)
{
    for (auto& entry : tabMap) {
        if (entry.second._AgxGraphicsScene.get() == scene) return true;
    }

    return false;
}

AgxGraphicsView* CALUMIMotion::GetAgxViewFromTab(int idx)
{
    if (idx >= ui.tabWidget->count() || idx < 0) return nullptr;

    if (auto agxView = dynamic_cast<AgxGraphicsView*>(ui.tabWidget->widget(idx)))
    {
        return agxView;
    }

    if (auto module = dynamic_cast<CALUMITabModule*>(ui.tabWidget->widget(idx)))
    {
        if (auto agxView = dynamic_cast<AgxGraphicsView*>(module->GetMainWidget()))
        {
            return agxView;
        }
    }

    return nullptr;
}

void CALUMIMotion::UpdateTabTitles()
{
    if (ui.tabWidget)
    {
        for (int i = 0; i < ui.tabWidget->count(); i++)
        {
            if (auto tab = dynamic_cast<CALUMITabModule*>(ui.tabWidget->widget(i)))
            {
                if (auto view = dynamic_cast<AgxGraphicsView*>(tab->GetMainWidget()))
                {
                    ui.tabWidget->setTabText(i, view->pagxNodeScene()->agxGraphModel().GetGraphTitle(false));
                    if (view->pagxNodeScene()->agxGraphModel().rootGraphReference() != &view->pagxNodeScene()->agxGraphModel())
                        ui.tabWidget->tabBar()->setTabTextColor(i, QColor(Qt::gray));
                }
            }
        }
    }
}

void CALUMIMotion::ShowNodeGroupMenu()
{
    if (ui.tabWidget->count() <= 0) return;
    QDialog* gWindow = new QDialog();
    
    QGridLayout* pgrid = new QGridLayout();
    QTabWidget* tabWidget = new QTabWidget(gWindow);
    pgrid->addWidget(tabWidget);
    tabWidget->clear();
    QGridLayout* grid = new QGridLayout();
    gWindow->setLayout(pgrid);
    tabWidget->setLayout(grid);
    gWindow->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    gWindow->setMinimumSize(440, 200);
    for (int i = 0; i < ui.tabWidget->count();i++)
    {
        auto agxView = GetAgxViewFromTab(i);

        if (agxView) {
            auto agxScene = tabMap.at(agxView)._AgxGraphicsScene.get();
            auto newTabView = new NodeGroupMenuPopup(nullptr, *agxScene, agxView, ui.tabWidget);
            tabWidget->addTab(newTabView, ui.tabWidget->tabText(i));

            if (tabWidget->count() == ui.tabWidget->count())
                tabWidget->setCurrentIndex(ui.tabWidget->currentIndex());
        }
    }
    gWindow->exec();

    if (gWindow)
        gWindow->deleteLater();
}

void CALUMIMotion::HandleNodeGroupMenuVisibility() const
{
    if (ui.tabWidget->count() <= 0)
        ui.actionNode_Groups->setEnabled(false);
    else
        ui.actionNode_Groups->setEnabled(true);
}

void CALUMIMotion::ShowAboutDialog()
{
    QMessageBox::about(this, tr("About"),
                       QString(tr("<b>%1</b><br/>Version %2<br/>Copyright &copy; 2026 %3<br/>All rights reserved")).
                       arg(QCoreApplication::applicationName()).
                       arg(QCoreApplication::applicationVersion()).
                       arg(QCoreApplication::organizationName()));
}

void CALUMIMotion::GetEditMenu()
{
    BuildUndoViewMenu();
    BuildCutCopyPasteMenu();
    BuildItemSelectionMenu();
    BuildGraphEditMenu();
    BuildSettingsMenu();
}

void CALUMIMotion::GetFileMenu()
{
    BuildFileInOutMenu();
}

void CALUMIMotion::GetViewMenu()
{
    HandleNodeGroupMenuVisibility();
    BuildCenterViewMenu();

    if (_showPropertiesSidebar)
        ui.actionHide_Properties->setText(tr("Hide Property Sidebar "));
    else
        ui.actionHide_Properties->setText(tr("Show Property Sidebar "));
}

void CALUMIMotion::GetHelpMenu()
{

}

void CALUMIMotion::BuildUndoViewMenu()
{
    if (ui.tabWidget->count() <= 0) { 
        //set dummy actions. Probably a cleaner way to do this but it's fine for now
        QAction* undoAction = ui.menuEdit->addAction(tr("Undo"));
        undoAction->setShortcut(QKeySequence::Undo); 
        undoAction->setEnabled(false);

        QAction* redoAction = ui.menuEdit->addAction(tr("Redo"));
        redoAction->setShortcut(QKeySequence::Redo); 
        redoAction->setEnabled(false);

        QMenu* undoStackAction = ui.menuEdit->addMenu(tr("Undo History"));
        undoStackAction->setEnabled(false);
        QMenu* redoStackAction = ui.menuEdit->addMenu(tr("Redo History"));
        redoStackAction->setEnabled(false);
        QAction* sep = ui.menuEdit->addSeparator();
        
        connect(ui.menuEdit, &QMenu::aboutToHide, undoStackAction, &QMenu::deleteLater);
        connect(ui.menuEdit, &QMenu::aboutToHide, undoAction, &QAction::deleteLater);
        connect(ui.menuEdit, &QMenu::aboutToHide, redoStackAction, &QMenu::deleteLater);
        connect(ui.menuEdit, &QMenu::aboutToHide, redoAction, &QAction::deleteLater);
        connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
        return; }

    if(const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex()))
    {
        view->undoActionRef()->setText(tr("Undo"));
        view->redoActionRef()->setText(tr("Redo"));
        ui.menuEdit->addAction(view->undoActionRef());
        ui.menuEdit->addAction(view->redoActionRef());

        auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view]() {
            ui.menuEdit->removeAction(view->undoActionRef());
            ui.menuEdit->removeAction(view->redoActionRef()); }, Qt::SingleShotConnection);

        QMenu* undoStackMenu = ui.menuEdit->addMenu(tr("Undo Stack"));

        QMenu* redoStackMenu = ui.menuEdit->addMenu(tr("Redo Stack"));

        QString text;
        for (int i = 0; i < view->undoStackRef().index(); i++)
        {
            text = view->undoStackRef().command(i)->text();
            QAction* menuItem = undoStackMenu->addAction(text);
            connect(menuItem, &QAction::triggered, view, [view, i]() {view->undoStackRef().setIndex(i); });
            if (i == view->undoStackRef().index() - 1) menuItem->setIcon(QIcon::fromTheme("edit-undo"));
        }
        for (int i = view->undoStackRef().index(); i < view->undoStackRef().count(); i++)
        {
            text = view->undoStackRef().command(i)->text();
            QAction* menuItem = redoStackMenu->addAction(text);
            connect(menuItem, &QAction::triggered, view, [view, i]() {view->undoStackRef().setIndex(i + 1); });
            if (i == view->undoStackRef().index()) menuItem->setIcon(QIcon::fromTheme("edit-redo"));
        }
            
        undoStackMenu->setEnabled(!undoStackMenu->actions().isEmpty());
        connect(ui.menuEdit, &QMenu::aboutToHide, undoStackMenu, &QMenu::deleteLater);
            
        redoStackMenu->setEnabled(!redoStackMenu->actions().isEmpty());
        connect(ui.menuEdit, &QMenu::aboutToHide, redoStackMenu, &QMenu::deleteLater);

        QAction* sep = ui.menuEdit->addSeparator();
        connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
    }
    
}

void CALUMIMotion::BuildCutCopyPasteMenu()
{
    if (ui.tabWidget->count() == 0)
    {
        QAction* dummyCut = ui.menuEdit->addAction(tr("Cut"),QKeySequence::Cut);
        dummyCut->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyCut, &QAction::deleteLater);
        QAction* dummyCopy = ui.menuEdit->addAction(tr("Copy"), QKeySequence::Copy);
        dummyCopy->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyCopy, &QAction::deleteLater);
        QAction* dummyPaste = ui.menuEdit->addAction(tr("Paste"), QKeySequence::Paste);
        dummyPaste->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyPaste, &QAction::deleteLater);
        QAction* dummyDup = ui.menuEdit->addAction(tr("Duplicate"), QKeySequence(Qt::CTRL | Qt::Key_D));
        dummyDup->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyDup, &QAction::deleteLater);
        QAction* dummyDel = ui.menuEdit->addAction(tr("Delete"), QKeySequence::Delete);
        dummyDel->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyDel, &QAction::deleteLater);
        QAction* sep = ui.menuEdit->addSeparator();
        connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
        return;
    }

    const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    if (!view) return;

    if (tabMap.at(view)._AgxGraphicsScene->selectedItems().size() != 0) {
        ui.menuEdit->addAction(view->cutActionRef());
        ui.menuEdit->addAction(view->copyActionRef());
        auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view]() {
            ui.menuEdit->removeAction(view->cutActionRef());
            ui.menuEdit->removeAction(view->copyActionRef()); }, Qt::SingleShotConnection);
    }
    else {
        QAction* dummyCut = ui.menuEdit->addAction(tr("Cut"), QKeySequence::Cut);
        dummyCut->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyCut, &QAction::deleteLater);
        QAction* dummyCopy = ui.menuEdit->addAction(tr("Copy"), QKeySequence::Copy);
        dummyCopy->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyCopy, &QAction::deleteLater);
    }
    
    ui.menuEdit->addAction(view->pasteActionRef());
    auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view]() {
    ui.menuEdit->removeAction(view->pasteActionRef()); }, Qt::SingleShotConnection);
    
    if (tabMap.at(view)._AgxGraphicsScene->selectedItems().size() != 0) {
        ui.menuEdit->addAction(view->duplicateActionRef());
        ui.menuEdit->addAction(view->deleteActionRef());
        auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view]() {
            ui.menuEdit->removeAction(view->duplicateActionRef());
            ui.menuEdit->removeAction(view->deleteActionRef()); }, Qt::SingleShotConnection);
    }
    else {
        QAction* dummyDup = ui.menuEdit->addAction(tr("Duplicate"), QKeySequence(Qt::CTRL | Qt::Key_D));
        dummyDup->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyDup, &QAction::deleteLater);
        QAction* dummyDel = ui.menuEdit->addAction(tr("Delete"), QKeySequence::Delete);
        dummyDel->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyDel, &QAction::deleteLater);
    }
    QAction* sep = ui.menuEdit->addSeparator();
    connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
}

void CALUMIMotion::BuildCenterViewMenu()
{
    if (!ui.tabWidget) return;

    if(ui.tabWidget->count()==0){
        QAction* centerView = ui.menuView->addAction(tr("Center View"));
        centerView->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Space));
        centerView->setEnabled(false);
        connect(ui.menuView, &QMenu::aboutToHide, centerView, &QAction::deleteLater);
    }
    else if(const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {        
        QAction* actionRef = view->centerActionRef();
        ui.menuView->addAction(actionRef);
        connect(ui.menuView, &QMenu::aboutToHide, this, [this, view]() {
                ui.menuView->removeAction(view->centerActionRef());
            }, Qt::SingleShotConnection);
    }
}

void CALUMIMotion::BuildSettingsMenu()
{

    QAction* settingsA = ui.menuEdit->addAction(QIcon::fromTheme("document-properties"), tr("Settings"));
    connect(ui.menuEdit, &QMenu::aboutToHide, settingsA, &QAction::deleteLater);
    connect(settingsA, &QAction::triggered, this, [this]() {
                                                                SettingsDialog dialog;
                                                                dialog.exec();
                                                            });

}

void CALUMIMotion::BuildGraphEditMenu()
{
    if (ui.tabWidget->count() <= 0) return;

    if (const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {
        QAction* newTitleAction = ui.menuEdit->addAction(tr("Edit Graph Title"));
        connect(ui.menuEdit, &QMenu::aboutToHide, newTitleAction, &QAction::deleteLater);
        connect(newTitleAction, &QAction::triggered, this, [this, view]() {
            bool ok = false;
            QString result = QInputDialog::getText(this, tr("Input New Graph Title"),
                                  tr("Graph Title:"), QLineEdit::Normal,
                                  view->pagxNodeScene()->agxGraphModel().GetGraphTitle(), &ok);

            if (result.isEmpty() || !ok || view->pagxNodeScene()->agxGraphModel().GetGraphTitle().compare(result, Qt::CaseInsensitive) == 0) return;

            result = cleanFileName(result, false, true);

            view->pagxNodeScene()->undoStack().push(new AgxSetGraphTitleCommand(&view->pagxNodeScene()->agxGraphModel(), result));

                });
        QAction* sep = ui.menuEdit->addSeparator();
        connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
    }
}

void CALUMIMotion::BuildItemSelectionMenu()
{
    if (ui.tabWidget->count() == 0)
    {
        QAction* dummySelectAll = ui.menuEdit->addAction(tr("Select All"), QKeySequence(Qt::CTRL | Qt::Key_A));
        dummySelectAll->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummySelectAll, &QAction::deleteLater);

        QAction* dummySelectAllN = ui.menuEdit->addAction(tr("Select All Nodes"), QKeySequence(Qt::CTRL | Qt::Key_N));
        dummySelectAllN->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummySelectAllN, &QAction::deleteLater);

        QAction* dummySelectAllL = ui.menuEdit->addAction(tr("Select All Connections"), QKeySequence(Qt::CTRL | Qt::Key_L));
        dummySelectAllL->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummySelectAllL, &QAction::deleteLater);

        QMenu* dummyMenu = ui.menuEdit->addMenu(tr("Selection Filter"));
        dummyMenu->setEnabled(false);
        connect(ui.menuEdit, &QMenu::aboutToHide, dummyMenu, &QMenu::deleteLater);

        return;
    }

    const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    if (!view) return;

    ui.menuEdit->addAction(view->selectAllActionRef());
    ui.menuEdit->addAction(view->selectAllNodesActionRef());
    ui.menuEdit->addAction(view->selectAllConnectionsActionRef());
    ui.menuEdit->addMenu(view->selectionFilterMenu());

    auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view]() {
        ui.menuEdit->removeAction(view->selectAllActionRef());
        ui.menuEdit->removeAction(view->selectAllNodesActionRef());          
        ui.menuEdit->removeAction(view->selectAllConnectionsActionRef());      
        ui.menuEdit->removeAction(view->selectionFilterMenu()->menuAction());
                                                                                   }, Qt::SingleShotConnection);


    QAction* sep = ui.menuEdit->addSeparator();
    connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
}


void CALUMIMotion::onSave()
{
    if (!ui.tabWidget || ui.tabWidget->count() == 0) return;

    if (const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {
        QString fileStr = view->pagxNodeScene()->agxGraphModel().GetModelFilePath();
        QSaveFile sFile(fileStr);
        if (!fileStr.isEmpty() && QFileInfo(fileStr).exists() && sFile.open(QIODevice::WriteOnly)) {
            auto doc = view->pagxNodeScene()->agxGraphModel().rootGraphReference()->save();
            QJsonDocument document(doc);

            QTextStream out(&sFile);
            out << document.toJson();

            if (sFile.commit()) {
                qInfo() << tr("Saved .jagx File: ") << fileStr;
                return;
            }
        }

        onSaveAs();
        
    }
}

void CALUMIMotion::onSaveAs()
{
    if (!ui.tabWidget || ui.tabWidget->count() == 0) return;

    if (const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {
        QString initialFilePath = view->pagxNodeScene()->agxGraphModel().GetModelFilePath();
        QFileInfo initialFileName(initialFilePath);
        
        QString graphTitle = view->pagxNodeScene()->agxGraphModel().GetGraphTitle();
        QString fileNameString = cleanFileName(graphTitle + ".agx");

        //QString fileNameString = initialFileName.fileName().isEmpty() ? "untitled.jagx" : initialFileName.fileName();
        QString fileDirString = initialFileName.path().isEmpty() && initialFileName.dir().exists() ? QDir::homePath() : initialFileName.path();
        QString fileStemString = initialFileName.baseName().isEmpty() ? "untitled" : initialFileName.baseName();

        QString filePath = QFileDialog::getSaveFileName(this, tr("Save Graph As"), fileDirString + "/" + fileNameString);
        if (filePath.isEmpty()) return;
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { QMessageBox::critical(this, tr("Error"), tr("Could not save to file")); return; }
        //view->pagxNodeScene()->agxGraphModel().SetGraphFilePath(QFileInfo(file).fileName());
        auto doc = view->pagxNodeScene()->agxGraphModel().rootGraphReference()->save();
        QJsonDocument document(doc);
        file.write(document.toJson());
        qInfo() << tr("Saved .jagx File As: ") << filePath;
        view->pagxNodeScene()->agxGraphModel().SetModelFilePath(filePath);
        view->pagxNodeScene()->agxGraphModel().SetGraphTitle(QFileInfo(filePath).baseName());
        view->pagxNodeScene()->agxGraphModel().SetRelativeDataPath(QFileInfo(filePath).baseName());
        SettingsRegistry::GetInstance().SetLastDirectory(filePath);
        file.close();
    }
}

void CALUMIMotion::onOpen()
{
    QString fileString = QFileDialog::getOpenFileName(this, tr("Open File"), SettingsRegistry::GetInstance().LastDirectory(), tr("Motion Files (*.jagx);;All Files (*.*)"));

    if (fileString.isEmpty()) return;

    QFile file(fileString);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file: ") + QFileInfo(fileString).fileName());
        return;
    }

    QByteArray byteArray = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, tr("Error"), tr("Json Parsing Failed: ") + parseError.errorString());
        return;
    }

    if (doc.isObject()) {

        QJsonObject obj = doc.object();
        AgxFileType type = AgxFileTypeFromString(obj.value("file-type").toString());
        switch (type)
        {
            case AgxFileType::BehaviorFile:
                OpenFile_Behavior_SFBGS(obj);
                break;
            case AgxFileType::UNKNOWN:
            case AgxFileType::AnimationFile:
            case AgxFileType::AnimationComponent:
            default:
                QMessageBox::critical(this, tr("Error"), tr("Unable To Open File Type: ") + AgxFileTypeToString(type));
                break;
        }
    }

}

void CALUMIMotion::BuildFileInOutMenu()
{
    const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    ui.actionSave->setEnabled(view != nullptr);
    ui.actionSave_As->setEnabled(view != nullptr);

}

void CALUMIMotion::ImportFile_Agx_SFBGS() {

    QString dir = SettingsRegistry::GetInstance().LastDirectory(AgxGameType::SFBGS).isEmpty() ? SettingsRegistry::GetInstance().LastDirectory() : SettingsRegistry::GetInstance().LastDirectory(AgxGameType::SFBGS);
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Starfield Agx File"), dir, tr("Agx Files (*.agx);;All files (*.*)"));

    if (filePath.isEmpty()) return;

    SettingsRegistry::GetInstance().SetLastDirectory(filePath, AgxGameType::SFBGS);

    pugi::xml_document doc;
    doc.load_file(filePath.toStdString().c_str());
    if (!doc) return;

    if (!doc.child("root"))
    {
        QMessageBox::StandardButton reply = QMessageBox::critical(nullptr, tr("File Content Warning"), tr("File is missing root and may not import properly..."), QMessageBox::Ok | QMessageBox::Abort);

        if (reply == QMessageBox::Abort) return;
    }

    if (ui.tabWidget)
    {
        std::shared_ptr<AgxGraphModel> agxGraphModel = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
        std::shared_ptr<AgxGraphicsScene> scene = std::make_shared<AgxGraphicsScene>(*agxGraphModel);

        AgxProgressDialog* progBar = new AgxProgressDialog(tr("Loading Agx File..."), "", 0, 1000, this);
        QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
        connect(watcher, &QFutureWatcher<void>::progressValueChanged, progBar, &QProgressDialog::setValue);
        connect(watcher, &QFutureWatcher<void>::progressTextChanged, progBar, &QProgressDialog::setLabelText);
        connect(watcher, &QFutureWatcher<void>::finished, progBar, &QProgressDialog::deleteLater);
        connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
        connect(agxGraphModel.get(), &AgxGraphModel::statusUpdate, watcher, [this, watcher](float loadPercentage, const QString& message) {
            Q_EMIT watcher->progressValueChanged(static_cast<int>(0.49 * loadPercentage * 1000));
            if (!message.isEmpty()) Q_EMIT watcher->progressTextChanged(message);
                });
        progBar->show();

        auto graphNode = doc.child("root");
        agxGraphModel.get()->load(graphNode);
        scene.get()->update();

        auto newTabView = new AgxGraphicsView(scene.get());

        Q_EMIT watcher->progressValueChanged(495);
        Q_EMIT watcher->progressTextChanged(tr("Processing Scene"));

        //newTabView->setUpdatesEnabled(false);
        //auto tempUpdateMode = newTabView->viewportUpdateMode();
        //newTabView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

        TabDataPair pairCopy(agxGraphModel, scene);
        tabMap.insert({ newTabView,pairCopy });

        Q_EMIT watcher->progressValueChanged(500);
        Q_EMIT watcher->progressTextChanged(tr("Loading View"));

        CALUMITabModule* module = new CALUMITabModule(newTabView, watcher, 500, 450);

        QFileInfo pathInfo(filePath);

        Q_EMIT watcher->progressValueChanged(960);

        ui.tabWidget->addTab(module, "");

        //scene->agxGraphModel().SetGraphTitle(pathInfo.baseName());
        if (auto cApp = dynamic_cast<CALUMIMotionApplication*>(QCoreApplication::instance()))
        {
            cApp->UpdateApplicationTabWidgets();
        }

        Q_EMIT watcher->progressValueChanged(970);

        ui.tabWidget->setCurrentWidget(module);

        Q_EMIT watcher->progressValueChanged(980);

        module->SetSidebarVisibility_Right(_showPropertiesSidebar);

        Q_EMIT watcher->progressValueChanged(990);

        QObject::connect(scene.get(), &AgxGraphicsScene::nodeContextMenu, newTabView, &AgxGraphicsView::ShowContextMenu);
        QObject::connect(scene.get(), &AgxGraphicsScene::nodeDoubleClicked, newTabView, &AgxGraphicsView::ToggleNodeCollapse);
        QObject::connect(scene.get(), &AgxGraphicsScene::nodePreClicked, newTabView, &AgxGraphicsView::OnNodePreClicked);

        HandleNodeGroupMenuVisibility();

        Q_EMIT watcher->progressValueChanged(995);
        Q_EMIT watcher->progressTextChanged(tr("Finalizing View"));

        if (auto toolbar = newTabView->getToolBarLayout()) {
            QPushButton* butt = new QPushButton("P");
            //QPushButton* buttr = new QPushButton("R");
            butt->setFixedSize(QSize(48, 48));
            //buttr->setFixedSize(QSize(48,48));
            toolbar->addWidget(butt);
            //toolbar->addWidget(buttr);

            connect(butt, &QPushButton::pressed, this, [this, scene, module]() {
                if (module->GetSideBarVisibility_Left())
                    module->CloseSideBarItem_Left();
                else {
                    SFBGS_GraphPropertiesDialogWidget* rootProperties = new SFBGS_GraphPropertiesDialogWidget(*scene);
                    module->SetSideBarItem_Left(rootProperties, true);
                }
                    });

            connect(agxGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [this, scene, module]() {
                SFBGS_GraphPropertiesDialogWidget* rootProperties = new SFBGS_GraphPropertiesDialogWidget(*scene);
                module->SetSideBarItem_Left(rootProperties, module->GetSideBarVisibility_Left());
                    });

        }

        Q_EMIT watcher->progressValueChanged(1000);
        Q_EMIT watcher->progressTextChanged(tr("Finished!"));

        newTabView->setFocus();
    }
}

void CALUMIMotion::ExportFile_Agx_SFBGS()
{
    if (ui.tabWidget->count() <= 0) 
    {
        return;
    }

    AgxGraphicsView* agxView = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    if (!agxView) 
    {
        qWarning() << "Export Agx File Called On Non AgxView Tab. Exiting Function...";
        return;
    }

    const AgxGraphModel* agxModel = agxView->pagxNodeScene()->agxGraphModel().rootGraphReference();

    if (!agxModel)
    {
        qCritical() << "Root Graph Is Null";
        return;
    }

        QString graphTitle = agxModel->GetGraphTitle();
        QString fileNameString = cleanFileName(graphTitle + ".agx");
        QString filePathToOpen = SettingsRegistry::GetInstance().LastDirectory(AgxGameType::SFBGS) + "/" + fileNameString;

    QString filePath = QFileDialog::getSaveFileName(this, tr("Export As Animation Behavior Graph"), filePathToOpen, tr("Agx Files (*.agx);;All files (*.*)"));
    if (filePath.isEmpty()) return;
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { QMessageBox::critical(this, tr("Error"), tr("Could not save to file")); return; }

    QString fileName = QFileInfo(file).baseName() + ".agx";

    pugi::xml_document doc;
    auto root = doc.append_child("root");

    {
        AgxAppendValue(root, "Name", SettingsRegistry::GetInstance().GetRelativeDataPath(AgxGameType::SFBGS) + fileName, AgxFormat::NewLine);

        QString category = agxModel->getGraphCategory();
        if (!category.isEmpty() && category.compare("None", Qt::CaseInsensitive) != 0 && category.compare("<none>", Qt::CaseInsensitive) != 0)
            AgxAppendValue(root, "Category", category, AgxFormat::NewLine, 0);
    }    

    agxModel->save(root);

    AgxCloseNode(root, false, false, 0);

    std::stringstream buffer;
    doc.save(buffer, "\t", pugi::format_no_declaration | pugi::format_raw);
    buffer << char(0x0A);
    std::string bufferData = buffer.str();
    file.write(bufferData.c_str(), bufferData.size());

    if (!file.commit())
    {
        QString msg = "File Could Not Be Exported...";
        qCritical() << msg;
        QMessageBox::critical(this, "Error", msg, QMessageBox::StandardButton::Ok);
    }
    else
    {
        qInfo() << "File Saved To: " << file.fileName();
    }
}

void CALUMIMotion::OpenFile_Behavior_SFBGS(QJsonObject& object)
{
    if (ui.tabWidget)
    {
        std::shared_ptr<AgxGraphModel> agxGraphModel = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
        std::shared_ptr<AgxGraphicsScene> scene = std::make_shared<AgxGraphicsScene>(*agxGraphModel);

        AgxProgressDialog* progBar = new AgxProgressDialog(tr("Loading jagx Behavior File..."), "", 0, 1000, this);
        QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
        connect(watcher, &QFutureWatcher<void>::progressValueChanged, progBar, &QProgressDialog::setValue);
        connect(watcher, &QFutureWatcher<void>::progressTextChanged, progBar, &QProgressDialog::setLabelText);
        connect(watcher, &QFutureWatcher<void>::finished, progBar, &QProgressDialog::deleteLater);
        connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
        connect(agxGraphModel.get(), &AgxGraphModel::statusUpdate, watcher, [this, watcher](float loadPercentage, const QString& message) {
            Q_EMIT watcher->progressValueChanged(static_cast<int>(0.49 * loadPercentage * 1000));
            if (!message.isEmpty()) Q_EMIT watcher->progressTextChanged(message);
                });
        progBar->show();

        
        agxGraphModel.get()->load(object);
        scene.get()->update();

        auto newTabView = new AgxGraphicsView(scene.get());

        Q_EMIT watcher->progressValueChanged(495);
        Q_EMIT watcher->progressTextChanged(tr("Processing Scene"));

        //newTabView->setUpdatesEnabled(false);
        //auto tempUpdateMode = newTabView->viewportUpdateMode();
        //newTabView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);

        TabDataPair pairCopy(agxGraphModel, scene);
        tabMap.insert({ newTabView,pairCopy });

        Q_EMIT watcher->progressValueChanged(500);
        Q_EMIT watcher->progressTextChanged(tr("Loading View"));

        CALUMITabModule* module = new CALUMITabModule(newTabView, watcher, 500, 450);

        //QFileInfo pathInfo(filePath);

        Q_EMIT watcher->progressValueChanged(960);

        ui.tabWidget->addTab(module, "");

        //scene->SetGraphTitle(pathInfo.baseName());

        Q_EMIT watcher->progressValueChanged(970);

        ui.tabWidget->setCurrentWidget(module);

        Q_EMIT watcher->progressValueChanged(980);

        module->SetSidebarVisibility_Right(_showPropertiesSidebar);

        Q_EMIT watcher->progressValueChanged(990);

        QObject::connect(scene.get(), &AgxGraphicsScene::nodeContextMenu, newTabView, &AgxGraphicsView::ShowContextMenu);
        QObject::connect(scene.get(), &AgxGraphicsScene::nodeDoubleClicked, newTabView, &AgxGraphicsView::ToggleNodeCollapse);
        QObject::connect(scene.get(), &AgxGraphicsScene::nodePreClicked, newTabView, &AgxGraphicsView::OnNodePreClicked);

        HandleNodeGroupMenuVisibility();

        Q_EMIT watcher->progressValueChanged(995);
        Q_EMIT watcher->progressTextChanged(tr("Finalizing View"));

        if (auto toolbar = newTabView->getToolBarLayout()) {
            QPushButton* butt = new QPushButton("P");
            //QPushButton* buttr = new QPushButton("R");
            butt->setFixedSize(QSize(48, 48));
            //buttr->setFixedSize(QSize(48,48));
            toolbar->addWidget(butt);
            //toolbar->addWidget(buttr);

            connect(butt, &QPushButton::pressed, this, [this, scene, module]() {
                if (module->GetSideBarVisibility_Left())
                    module->CloseSideBarItem_Left();
                else {
                    SFBGS_GraphPropertiesDialogWidget* rootProperties = new SFBGS_GraphPropertiesDialogWidget(*scene);
                    module->SetSideBarItem_Left(rootProperties, true);
                }
                    });

            connect(agxGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [this, scene, module]() {
                SFBGS_GraphPropertiesDialogWidget* rootProperties = new SFBGS_GraphPropertiesDialogWidget(*scene);
                module->SetSideBarItem_Left(rootProperties, module->GetSideBarVisibility_Left());
                    });

        }

        Q_EMIT watcher->progressValueChanged(1000);
        Q_EMIT watcher->progressTextChanged(tr("Finished!"));

        newTabView->setFocus();
    }
}

void CALUMIMotion::Create_SFBGSTab(std::shared_ptr<AgxGraphicsScene> scene, std::shared_ptr<AgxGraphModel> model)
{
    if(!model.get())
        model = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
    if(!scene.get())
        scene = std::make_shared<AgxGraphicsScene>(*model);

    for (int i = 0; i < ui.tabWidget->count(); i++)
    {
        if (auto view = GetAgxViewFromTab(i))
        {
            if (view->pagxNodeScene() == scene.get())
            {
                ui.tabWidget->setCurrentIndex(i);
                return;
            }
        }
    }

    auto newTabView = new AgxGraphicsView(scene.get());
    TabDataPair pairCopy(model, scene);
    tabMap.insert({ newTabView,pairCopy });

    CALUMITabModule* module = new CALUMITabModule(newTabView);
    ui.tabWidget->addTab(module, scene->agxGraphModel().GetGraphTitle(false));
    ui.tabWidget->setCurrentWidget(module);

    module->SetSidebarVisibility_Right(_showPropertiesSidebar);

    //ui.tabWidget->addTab(newTabView, "EmbeddedUntitledAnimationGraph");
    //ui.tabWidget->setCurrentWidget(newTabView);

    QObject::connect(scene.get(), &AgxGraphicsScene::nodeContextMenu, newTabView, &AgxGraphicsView::ShowContextMenu);
    QObject::connect(scene.get(), &AgxGraphicsScene::nodeDoubleClicked, newTabView, &AgxGraphicsView::ToggleNodeCollapse);
    QObject::connect(scene.get(), &AgxGraphicsScene::nodePreClicked, newTabView, &AgxGraphicsView::OnNodePreClicked);

    if (auto toolbar = newTabView->getToolBarLayout()) {
        
        if(model->rootGraphReference() != model.get())
        {
            QLabel* embLabel = new QLabel(tr("EMBEDDED GRAPH"));
            connect(this, &CALUMIMotion::LanguageChanged, embLabel, [this, embLabel]() { embLabel->setText(tr("EMBEDDED GRAPH")); });

            embLabel->setFixedHeight(48);
            auto font = embLabel->font();
            font.setPointSize(32);
            font.setBold(true);
            embLabel->setFont(font);
            embLabel->setEnabled(false);
            toolbar->addWidget(embLabel);
            toolbar->addItem(new QSpacerItem(12, 10));
        }
        
        QPushButton* butt = new QPushButton("P");
        //QPushButton* buttr = new QPushButton("R");
        butt->setFixedSize(QSize(48, 48));
        //buttr->setFixedSize(QSize(48,48));
        toolbar->addWidget(butt);
        //toolbar->addWidget(buttr);

        connect(butt, &QPushButton::pressed, this, [this, scene, module]() {

            if (module->GetSideBarVisibility_Left())
                module->CloseSideBarItem_Left();
            else {
                SFBGS_GraphPropertiesDialogWidget* rootProperties = new SFBGS_GraphPropertiesDialogWidget(*scene);
                module->SetSideBarItem_Left(rootProperties);
            }
                });

        connect(model.get(), &AgxGraphModel::GraphTypeUpdated, this, [this, scene, module]() {
            SFBGS_GraphPropertiesDialogWidget* rootProperties = new SFBGS_GraphPropertiesDialogWidget(*scene);
            module->SetSideBarItem_Left(rootProperties, module->GetSideBarVisibility_Left());
                });

    }

    newTabView->setFocus();
}

bool CALUMIMotion::HasTab(QWidget* widget)
{
    if (!widget) return false;

    if (ui.tabWidget)
    {
        for (int i = 0; i < ui.tabWidget->count(); i++)
        {
            if (widget == ui.tabWidget->widget(i) || widget == GetAgxViewFromTab(i))
            {
                return true;
            }
        }
    }

    return false;
}

TabDataPair::TabDataPair(const std::shared_ptr<AgxGraphModel>& _AgxGraphModel, const std::shared_ptr<AgxGraphicsScene>& _AgxGraphicsScene) : _AgxGraphModel(_AgxGraphModel), _AgxGraphicsScene(_AgxGraphicsScene) {}
