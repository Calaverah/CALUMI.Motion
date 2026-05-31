//Copyright © 2025-2026 Calaverah. All rights reserved.
//License: https://www.gnu.org/licenses/lgpl-3.0.html
//Contact: Calaverahmedia@gmail.com

// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppTooWideScope
// ReSharper disable CppDFAMemoryLeak
#include "stdafx.h"
#include "CALUMIMotion.h"
#include "Widgets/AgxGraphicsView.h"
#include "Widgets/NodeGroupMenuPopup.h"
#include <string>
#include <Utilities/AgxDefinitions.h>
#include "Utilities/DialogPool.h"
#include "Widgets/TabModule/GraphTabWidget.h"
#include "Widgets/SFBGS/SFBGS_GraphPropertiesDialogWidget.h"

#include <Widgets/Dialog/AgxProgressDialog.h>
#include <Utilities/Settings/SettingsRegistry.h>
#include "Application/CALUMIMotionApplication.h"
#include "oclero/qlementine/icons/Icons16.hpp"
#include <QtSvg/QSvgRenderer>
#include "Utilities/AgxFormat.h"
#include "Utilities/QWidgetFactories.h"
#include "Widgets/Settings/SettingsDialog.h"
#include "Utilities/UndoRedoCommands.h"

CALUMIMotion::CALUMIMotion(QWidget *parent) : QMainWindow(parent)
{
    setObjectName("CALUMIMotionObject");
    ui.setupUi(this);

#ifdef __APPLE__
    ui.menuBar->setNativeMenuBar(false);
#endif
    
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
    connect(ui.actionToggle_Log_Console, &QAction::triggered, this, &CALUMIMotionApplication::ToggleLogger);

    const auto& settings = SettingsRegistry::GetInstance();

    const QByteArray& bytes = settings.GetSavedWindowGeometry("Geometry");
    
    if (settings.UseSavedGeometry() && !bytes.isEmpty())
        restoreGeometry(settings.GetSavedWindowGeometry("Geometry"));
    else
        resize(1280, 800);
    const StartupVisibilityPreference propPref = settings.GetPropertySidebarVisibilityPreference();
    switch (propPref)
    {
        case StartupVisibilityPreference::Never:
            _showPropertiesSidebar = false;
            break;
        case StartupVisibilityPreference::Remember:
            _showPropertiesSidebar = settings.GetLastState("Sidebar/State", _showPropertiesSidebar);
            break;
        case StartupVisibilityPreference::Always:
            _showPropertiesSidebar = true;
            break;
    }

}

CALUMIMotion::~CALUMIMotion() {
    SettingsRegistry::GetInstance().SaveWindowGeometry("Geometry", saveGeometry());
    
}

void CALUMIMotion::closeEvent(QCloseEvent* event)
{
    size_t topLevelCount = 0;

    for (const auto window : CALUMIMotionApplication::topLevelWindows())
    {
        //TBD: there has to be a better solution for this but whatever.
        if (window->objectName() != "CALUMIMotionObjectLogger" && window->isVisible())
        {
            topLevelCount++;
        }
    }

    if(topLevelCount <= 1)
    {
        CALUMIMotionApplication::SaveLoggerExitState();
        CALUMIMotionApplication::HideLogger();
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

void CALUMIMotion::CloseTab(const int i) const
{
    if (ui.tabWidget && ui.tabWidget->count()>i && i >= 0)
    {
        //ui.tabWidget->removeTab(i);
        if (const auto agxView = GetAgxViewFromTab(i))
        {
            if (agxView->agxNodeScene())
            {
                agxView->agxNodeScene()->agxGraphModel().HandleEmbeddedClosures();
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

void CALUMIMotion::CloseTab(const QWidget* widget) const
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

bool CALUMIMotion::HasScene(const AgxGraphicsScene* scene) const
{
    for (auto& pair : tabMap | std::views::values) // NOLINT(*-use-anyofallof)
    {
        if (pair.m_AgxGraphicsScene.get() == scene)
            return true;
    }

    return false;
}

AgxGraphicsView* CALUMIMotion::GetAgxViewFromTab(const int idx) const
{
    if (idx >= ui.tabWidget->count() || idx < 0)
        return nullptr;

    if (const auto agxView = dynamic_cast<AgxGraphicsView*>(ui.tabWidget->widget(idx)))
    {
        return agxView;
    }

    if (const auto module = dynamic_cast<GraphTabWidget*>(ui.tabWidget->widget(idx)))
    {
        if (const auto agxView = module->graph())
        {
            return agxView;
        }
    }

    return nullptr;
}

void CALUMIMotion::UpdateTabTitles() const
{
    if (ui.tabWidget)
    {
        for (int i = 0; i < ui.tabWidget->count(); i++)
        {
            if (const auto tab = dynamic_cast<GraphTabWidget*>(ui.tabWidget->widget(i)))
            {
                if (const auto view = tab->graph())
                {
                    ui.tabWidget->setTabText(i, view->agxNodeScene()->agxGraphModel().GetGraphTitle(false));
                    if (view->agxNodeScene()->agxGraphModel().rootGraphReference() != &view->agxNodeScene()->agxGraphModel())
                        ui.tabWidget->tabBar()->setTabTextColor(i, QColor(Qt::gray));
                }
            }
        }
    }
}

void CALUMIMotion::ShowNodeGroupMenu() const
{
    if (ui.tabWidget->count() <= 0) return;
    const auto gWindow = new QDialog();

    const auto pgrid = new QGridLayout();
    const auto tabWidget = new QTabWidget(gWindow);
    pgrid->addWidget(tabWidget);
    tabWidget->clear();
    const auto grid = new QGridLayout();
    gWindow->setLayout(pgrid);
    tabWidget->setLayout(grid);
    gWindow->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    gWindow->setMinimumSize(440, 200);
    for (int i = 0; i < ui.tabWidget->count();i++)
    {
        auto agxView = GetAgxViewFromTab(i);

        if (agxView)
        {
            const auto agxScene = tabMap.at(agxView).m_AgxGraphicsScene.get();
            const auto newTabView = new NodeGroupMenuPopup(nullptr, *agxScene, agxView, ui.tabWidget);
            tabWidget->addTab(newTabView, ui.tabWidget->tabText(i));

            if (tabWidget->count() == ui.tabWidget->count())
                tabWidget->setCurrentIndex(ui.tabWidget->currentIndex());
        }
    }
    gWindow->exec();
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

void CALUMIMotion::GetFileMenu() const
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

// ReSharper disable once CppMemberFunctionMayBeStatic
void CALUMIMotion::GetHelpMenu()
{

}

void CALUMIMotion::BuildUndoViewMenu()
{
    if (ui.tabWidget->count() <= 0) { 
        //set dummy actions. Probably a cleaner way to do this, but it's fine for now
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
        const QAction* sep = ui.menuEdit->addSeparator();
        
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

        auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view] {
            ui.menuEdit->removeAction(view->undoActionRef());
            ui.menuEdit->removeAction(view->redoActionRef()); }, Qt::SingleShotConnection);

        QMenu* undoStackMenu = ui.menuEdit->addMenu(tr("Undo Stack"));

        QMenu* redoStackMenu = ui.menuEdit->addMenu(tr("Redo Stack"));

        QString text;
        for (int i = 0; i < view->undoStackRef().index(); i++)
        {
            text = view->undoStackRef().command(i)->text();
            QAction* menuItem = undoStackMenu->addAction(text);
            connect(menuItem, &QAction::triggered, view, [view, i] {view->undoStackRef().setIndex(i); });
            if (i == view->undoStackRef().index() - 1) menuItem->setIcon(QIcon::fromTheme("edit-undo"));
        }
        for (int i = view->undoStackRef().index(); i < view->undoStackRef().count(); i++)
        {
            text = view->undoStackRef().command(i)->text();
            QAction* menuItem = redoStackMenu->addAction(text);
            connect(menuItem, &QAction::triggered, view, [view, i] {view->undoStackRef().setIndex(i + 1); });
            if (i == view->undoStackRef().index()) menuItem->setIcon(QIcon::fromTheme("edit-redo"));
        }
            
        undoStackMenu->setEnabled(!undoStackMenu->actions().isEmpty());
        connect(ui.menuEdit, &QMenu::aboutToHide, undoStackMenu, &QMenu::deleteLater);
            
        redoStackMenu->setEnabled(!redoStackMenu->actions().isEmpty());
        connect(ui.menuEdit, &QMenu::aboutToHide, redoStackMenu, &QMenu::deleteLater);

        const QAction* sep = ui.menuEdit->addSeparator();
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
        const QAction* sep = ui.menuEdit->addSeparator();
        connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
        return;
    }

    const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    if (!view) return;

    if (!tabMap.at(view).m_AgxGraphicsScene->selectedItems().empty()) {
        ui.menuEdit->addAction(view->cutActionRef());
        ui.menuEdit->addAction(view->copyActionRef());
        auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view] {
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
    auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view] {
    ui.menuEdit->removeAction(view->pasteActionRef()); }, Qt::SingleShotConnection);
    
    if (!tabMap.at(view).m_AgxGraphicsScene->selectedItems().empty()) {
        ui.menuEdit->addAction(view->duplicateActionRef());
        ui.menuEdit->addAction(view->deleteActionRef());
        auto dRemovals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view] {
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
    const QAction* sep = ui.menuEdit->addSeparator();
    connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
}

void CALUMIMotion::BuildCenterViewMenu()
{
    if (!ui.tabWidget)
        return;

    if(ui.tabWidget->count()==0){
        QAction* centerView = ui.menuView->addAction(tr("Center View"));
        centerView->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Space));
        centerView->setEnabled(false);
        connect(ui.menuView, &QMenu::aboutToHide, centerView, &QAction::deleteLater);
    }
    else if(const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {        
        QAction* actionRef = view->centerActionRef();
        ui.menuView->addAction(actionRef);
        connect(ui.menuView, &QMenu::aboutToHide, this, [this, view] {
                ui.menuView->removeAction(view->centerActionRef());
            }, Qt::SingleShotConnection);
    }
}

void CALUMIMotion::BuildSettingsMenu()
{

    const QAction* settingsA = ui.menuEdit->addAction(QIcon::fromTheme("document-properties"), tr("Settings"));
    connect(ui.menuEdit, &QMenu::aboutToHide, settingsA, &QAction::deleteLater);
    connect(settingsA, &QAction::triggered, this, [] {
            SettingsDialog dialog;
            dialog.exec();
        });

}

void CALUMIMotion::BuildGraphEditMenu()
{
    if (ui.tabWidget->count() <= 0) return;

    if (const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {
        const QAction* newTitleAction = ui.menuEdit->addAction(tr("Edit Graph Title"));
        connect(ui.menuEdit, &QMenu::aboutToHide, newTitleAction, &QAction::deleteLater);
        connect(newTitleAction, &QAction::triggered, this, [this, view] {
            bool ok = false;
            QString result = QInputDialog::getText(this, tr("Input New Graph Title"),
                                  tr("Graph Title:"), QLineEdit::Normal,
                                  view->agxNodeScene()->agxGraphModel().GetGraphTitle(), &ok);

            if (result.isEmpty() || !ok || view->agxNodeScene()->agxGraphModel().GetGraphTitle().compare(result, Qt::CaseInsensitive) == 0) return;

            result = cleanFileName(result, false, true);

            view->agxNodeScene()->undoStack().push(new AgxSetGraphTitleCommand(&view->agxNodeScene()->agxGraphModel(), result));

                });
        const QAction* sep = ui.menuEdit->addSeparator();
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

    if (!view)
        return;

    ui.menuEdit->addAction(view->selectAllActionRef());
    ui.menuEdit->addAction(view->selectAllNodesActionRef());
    ui.menuEdit->addAction(view->selectAllConnectionsActionRef());
    ui.menuEdit->addMenu(view->selectionFilterMenu());

    auto removals = connect(ui.menuEdit, &QMenu::aboutToHide, this, [this, view] {
        ui.menuEdit->removeAction(view->selectAllActionRef());
        ui.menuEdit->removeAction(view->selectAllNodesActionRef());          
        ui.menuEdit->removeAction(view->selectAllConnectionsActionRef());      
        ui.menuEdit->removeAction(view->selectionFilterMenu()->menuAction());
                                                                                   }, Qt::SingleShotConnection);


    const QAction* sep = ui.menuEdit->addSeparator();
    connect(ui.menuEdit, &QMenu::aboutToHide, sep, &QAction::deleteLater);
}


void CALUMIMotion::onSave()
{
    if (!ui.tabWidget || ui.tabWidget->count() == 0) return;

    if (const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex())) {
        const QString fileStr = view->agxNodeScene()->agxGraphModel().GetModelFilePath();
        QSaveFile sFile(fileStr);
        if (!fileStr.isEmpty() && QFileInfo(fileStr).exists() && sFile.open(QIODevice::WriteOnly)) {
            const auto doc = view->agxNodeScene()->agxGraphModel().rootGraphReference()->save();
            const QJsonDocument document(doc);

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
        const QString initialFilePath = view->agxNodeScene()->agxGraphModel().GetModelFilePath();
        const QFileInfo initialFileName(initialFilePath);

        const QString graphTitle = view->agxNodeScene()->agxGraphModel().GetGraphTitle();
        const QString fileNameString = cleanFileName(graphTitle + ".jagx");

        //QString fileNameString = initialFileName.fileName().isEmpty() ? "untitled.jagx" : initialFileName.fileName();
        const QString fileDirString = initialFileName.path().isEmpty() && initialFileName.dir().exists() ? QDir::homePath() : initialFileName.path();
        QString fileStemString = initialFileName.baseName().isEmpty() ? "untitled" : initialFileName.baseName();

        const QString filePath = QFileDialog::getSaveFileName(this, tr("Save Graph As"), fileDirString + "/" + fileNameString);
        if (filePath.isEmpty()) return;
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) { QMessageBox::critical(this, tr("Error"), tr("Could not save to file")); return; }
        const auto doc = view->agxNodeScene()->agxGraphModel().rootGraphReference()->save();
        const QJsonDocument document(doc);
        file.write(document.toJson());
        qInfo() << tr("Saved .jagx File As: ") << filePath;
        view->agxNodeScene()->agxGraphModel().SetModelFilePath(filePath);
        view->agxNodeScene()->agxGraphModel().SetGraphTitle(QFileInfo(filePath).baseName());
        view->agxNodeScene()->agxGraphModel().SetRelativeDataPath(QFileInfo(filePath).baseName());
        SettingsRegistry::GetInstance().SetLastDirectory(filePath);
        file.close();
    }
}

void CALUMIMotion::onOpen()
{
    const QString fileString = QFileDialog::getOpenFileName(this, tr("Open File"), SettingsRegistry::GetInstance().LastDirectory(), tr("Motion Files (*.jagx);;All Files (*.*)"));

    if (fileString.isEmpty())
        return;

    QFile file(fileString);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file: ") + QFileInfo(fileString).fileName());
        return;
    }

    const QByteArray byteArray = file.readAll();
    file.close();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::critical(this, tr("Error"), tr("Json Parsing Failed: ") + parseError.errorString());
        return;
    }

    if (doc.isObject()) {

        const QJsonObject obj = doc.object();
        const AgxFileType type = AgxFileTypeFromString(obj.value("file-type").toString());
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

void CALUMIMotion::BuildFileInOutMenu() const
{
    const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    ui.actionSave->setEnabled(view != nullptr);
    ui.actionSave_As->setEnabled(view != nullptr);

}

void CALUMIMotion::ImportFile_Agx_SFBGS() {

    const QString dir = SettingsRegistry::GetInstance().LastDirectory(AgxGameType::SFBGS).isEmpty() ? SettingsRegistry::GetInstance().LastDirectory() : SettingsRegistry::GetInstance().LastDirectory(AgxGameType::SFBGS);
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Starfield Agx File"), dir, tr("Agx Files (*.agx);;All files (*.*)"));

    if (filePath.isEmpty())
        return;

    SettingsRegistry::GetInstance().SetLastDirectory(filePath, AgxGameType::SFBGS);

    pugi::xml_document doc;
    doc.load_file(filePath.toStdString().c_str());

    if (!doc)
        return;

    if (!doc.child("root"))
    {
        const QMessageBox::StandardButton reply = QMessageBox::critical(nullptr, tr("File Content Warning"), tr("File is missing root and may not import properly..."), QMessageBox::Ok | QMessageBox::Abort);

        if (reply == QMessageBox::Abort)
            return;
    }

    if (ui.tabWidget)
    {
        const auto agxGraphModel = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
        auto scene = std::make_shared<AgxGraphicsScene>(*agxGraphModel);

        const auto progBar = new AgxProgressDialog(tr("Loading Agx File..."), "", 0, 1000, this);
        auto watcher = new QFutureWatcher<void>(this);
        connect(watcher, &QFutureWatcher<void>::progressValueChanged, progBar, &QProgressDialog::setValue);
        connect(watcher, &QFutureWatcher<void>::progressTextChanged, progBar, &QProgressDialog::setLabelText);
        connect(watcher, &QFutureWatcher<void>::finished, progBar, &QProgressDialog::deleteLater);
        connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
        connect(agxGraphModel.get(), &AgxGraphModel::statusUpdate, watcher, [watcher](const float loadPercentage, const QString& message) {
                Q_EMIT watcher->progressValueChanged(static_cast<int>(0.49 * loadPercentage * 1000));
                if (!message.isEmpty())
                    Q_EMIT watcher->progressTextChanged(message);
            });
        progBar->show();

        auto graphNode = doc.child("root");
        agxGraphModel->load(graphNode);
        scene->update();

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

        auto module = new GraphTabWidget(newTabView); //, watcher, 500, 450);

        QFileInfo pathInfo(filePath);

        Q_EMIT watcher->progressValueChanged(960);

        ui.tabWidget->addTab(module, "");

        //scene->agxGraphModel().SetGraphTitle(pathInfo.baseName());
        CALUMIMotionApplication::UpdateApplicationTabWidgets();


        Q_EMIT watcher->progressValueChanged(970);

        ui.tabWidget->setCurrentWidget(module);

        Q_EMIT watcher->progressValueChanged(980);

        // module->onSetRightPanelVisible(_showPropertiesSidebar);

        Q_EMIT watcher->progressValueChanged(990);

        HandleNodeGroupMenuVisibility();

        Q_EMIT watcher->progressValueChanged(995);
        Q_EMIT watcher->progressTextChanged(tr("Finalizing View"));

        if (const auto toolbar = newTabView->getToolBarLayout())
        {
            const auto propIcon = QIcon::fromTheme("navigation/menu-burger");
            const auto propButton = new QPushButton(propIcon, "Graph Properties");
            propButton->setFixedSize(QSize(48, 48));
            toolbar->addWidget(propButton);

            connect(propButton, &QPushButton::pressed, this, [module]
                {
                    module->onSetLeftPanelVisible(!module->leftPanelVisible());
                });

            connect(agxGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [scene, module]
                {
                    module->setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));
                });

            module->setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));

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

    const AgxGraphModel* agxModel = agxView->agxNodeScene()->agxGraphModel().rootGraphReference();

    if (!agxModel)
    {
        qCritical() << "Root Graph Is Null";
        return;
    }

        const QString graphTitle = agxModel->GetGraphTitle();
        const QString fileNameString = cleanFileName(graphTitle + ".agx");
        const QString filePathToOpen = SettingsRegistry::GetInstance().LastDirectory(AgxGameType::SFBGS) + "/" + fileNameString;

    const QString filePath = QFileDialog::getSaveFileName(this, tr("Export As Animation Behavior Graph"), filePathToOpen, tr("Agx Files (*.agx);;All files (*.*)"));

    if (filePath.isEmpty())
        return;

    QSaveFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Error"), tr("Could not save to file")); return;
    }

    const QString fileName = QFileInfo(file).baseName() + ".agx";

    pugi::xml_document doc;
    auto root = doc.append_child("root");

    {
        AgxAppendValue(root, "Name", SettingsRegistry::GetInstance().GetRelativeDataPath(AgxGameType::SFBGS) + fileName, AgxFormat::NewLine);

        if (const QString category = agxModel->getGraphCategory(); !category.isEmpty() && category.compare("None", Qt::CaseInsensitive) != 0 && category.compare("<none>", Qt::CaseInsensitive) != 0)
            AgxAppendValue(root, "Category", category, AgxFormat::NewLine, 0);
    }    

    agxModel->save(root);

    AgxCloseNode(root, false, false, 0);

    std::stringstream buffer;
    doc.save(buffer, "\t", pugi::format_no_declaration | pugi::format_raw);
    buffer << static_cast<char>(0x0A);
    const std::string bufferData = buffer.str();
    file.write(bufferData.c_str(), static_cast<qint64>(bufferData.size()));

    if (!file.commit())
    {
        const QString msg = "File Could Not Be Exported...";
        qCritical() << msg;
        QMessageBox::critical(this, "Error", msg, QMessageBox::StandardButton::Ok);
    }
    else
    {
        qInfo() << "File Saved To: " << file.fileName();
    }
}

void CALUMIMotion::OpenFile_Behavior_SFBGS(const QJsonObject& object)
{
    if (ui.tabWidget)
    {
        const auto agxGraphModel = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
        auto scene = std::make_shared<AgxGraphicsScene>(*agxGraphModel);

        const auto progBar = new AgxProgressDialog(tr("Loading jagx Behavior File..."), "", 0, 1000, this);
        auto watcher = new QFutureWatcher<void>(this);
        connect(watcher, &QFutureWatcher<void>::progressValueChanged, progBar, &QProgressDialog::setValue);
        connect(watcher, &QFutureWatcher<void>::progressTextChanged, progBar, &QProgressDialog::setLabelText);
        connect(watcher, &QFutureWatcher<void>::finished, progBar, &QProgressDialog::deleteLater);
        connect(watcher, &QFutureWatcher<void>::finished, watcher, &QFutureWatcher<void>::deleteLater);
        connect(agxGraphModel.get(), &AgxGraphModel::statusUpdate, watcher, [watcher](const float loadPercentage, const QString& message) {
                Q_EMIT watcher->progressValueChanged(static_cast<int>(0.49 * loadPercentage * 1000));

                if (!message.isEmpty())
                        Q_EMIT watcher->progressTextChanged(message);
            });
        progBar->show();

        
        agxGraphModel->load(object);
        scene->update();

        auto newTabView = new AgxGraphicsView(scene.get());

        Q_EMIT watcher->progressValueChanged(495);
        Q_EMIT watcher->progressTextChanged(tr("Processing Scene"));

        TabDataPair pairCopy(agxGraphModel, scene);
        tabMap.insert({ newTabView,pairCopy });

        Q_EMIT watcher->progressValueChanged(500);
        Q_EMIT watcher->progressTextChanged(tr("Loading View"));

        auto module = new GraphTabWidget(newTabView); //, watcher, 500, 450);

        Q_EMIT watcher->progressValueChanged(960);

        ui.tabWidget->addTab(module, "");

        Q_EMIT watcher->progressValueChanged(970);

        ui.tabWidget->setCurrentWidget(module);

        Q_EMIT watcher->progressValueChanged(980);

        // module->onSetRightPanelVisible(_showPropertiesSidebar);

        Q_EMIT watcher->progressValueChanged(990);

        HandleNodeGroupMenuVisibility();

        Q_EMIT watcher->progressValueChanged(995);
        Q_EMIT watcher->progressTextChanged(tr("Finalizing View"));

        if (const auto toolbar = newTabView->getToolBarLayout())
        {
            const auto propIcon = QIcon::fromTheme("navigation/menu-burger");
            const auto propButton = new QPushButton(propIcon, "Graph Properties");
            propButton->setFixedSize(QSize(48, 48));
            toolbar->addWidget(propButton);

            connect(propButton, &QPushButton::pressed, this, [module]
                {
                    module->onSetLeftPanelVisible(!module->leftPanelVisible());
                });

            connect(agxGraphModel.get(), &AgxGraphModel::GraphTypeUpdated, this, [scene, module]
                {
                    module->setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));
                });

            module->setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));

        }

        Q_EMIT watcher->progressValueChanged(1000);
        Q_EMIT watcher->progressTextChanged(tr("Finished!"));

        newTabView->setFocus();
    }
}

void CALUMIMotion::Create_SFBGSTab(std::shared_ptr<AgxGraphicsScene> scene, std::shared_ptr<AgxGraphModel> model)
{
    if(!model)
        model = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
    if(!scene)
        scene = std::make_shared<AgxGraphicsScene>(*model);

    for (int i = 0; i < ui.tabWidget->count(); i++)
    {
        if (const auto view = GetAgxViewFromTab(i))
        {
            if (view->agxNodeScene() == scene.get())
            {
                ui.tabWidget->setCurrentIndex(i);
                return;
            }
        }
    }

    auto newTabView = new AgxGraphicsView(scene.get());
    TabDataPair pairCopy(model, scene);
    tabMap.insert({ newTabView,pairCopy });

    auto module = new GraphTabWidget(newTabView);
    ui.tabWidget->addTab(module, scene->agxGraphModel().GetGraphTitle(false));
    ui.tabWidget->setCurrentWidget(module);

    if (const auto toolbar = newTabView->getToolBarLayout()) {
        
        if(model->rootGraphReference() != model.get())
        {
            auto embLabel = new QLabel(tr("EMBEDDED GRAPH"));
            connect(this, &CALUMIMotion::LanguageChanged, embLabel, [embLabel] { embLabel->setText(tr("EMBEDDED GRAPH")); });

            embLabel->setFixedHeight(48);
            auto font = embLabel->font();
            font.setPointSize(32);
            font.setBold(true);
            embLabel->setFont(font);
            embLabel->setEnabled(false);
            toolbar->addWidget(embLabel);
            toolbar->addItem(new QSpacerItem(12, 10));
        }

        const auto propButton = new QPushButton();
        {
            const auto iconPath = oclero::qlementine::icons::iconPath(oclero::qlementine::icons::Icons16::Navigation_MenuBurger);
            const auto ico = GetColoredIconFromSVG(iconPath);
            propButton->setIcon(ico);
        }

        propButton->setFixedSize(QSize(48, 48));
        toolbar->addWidget(propButton);

        connect(propButton, &QPushButton::pressed, this, [module]
            {
                    module->onSetLeftPanelVisible(!module->leftPanelVisible());
            });

        connect(model.get(), &AgxGraphModel::GraphTypeUpdated, this, [scene, module]
            {
                module->setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));
            });

        module->setLeftItem(new SFBGS_GraphPropertiesDialogWidget(*scene));

    }

    newTabView->setFocus();
}

bool CALUMIMotion::HasTab(const QWidget* widget) const
{
    if (!widget)
        return false;

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

TabDataPair::TabDataPair(const std::shared_ptr<AgxGraphModel>& agxGraphModel, const std::shared_ptr<AgxGraphicsScene>& agxGraphicsScene) : m_AgxGraphModel(agxGraphModel), m_AgxGraphicsScene(agxGraphicsScene) {}
