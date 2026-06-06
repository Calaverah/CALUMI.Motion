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

    CALUMIMotionApplication::TrackWindow(this);

#ifdef __APPLE__
    ui.menuBar->setNativeMenuBar(false);
#endif
    
    setWindowTitle(windowTitle() + " [" + QCoreApplication::applicationVersion() + "]");

    connect(ui.tabWidget, &QTabWidget::currentChanged, this, &CALUMIMotion::onTabChanged);

    connect(ui.actionOpen, &QAction::triggered, this, [this]
    {
        this->onLoadFile("");
    });
    connect(ui.actionSave, &QAction::triggered, this, &CALUMIMotion::onSave);
    connect(ui.actionSave_As, &QAction::triggered, this, &CALUMIMotion::onSaveAs);

    connect(ui.actionSFBGS_Behavior_Graph_export, &QAction::triggered, this, &CALUMIMotion::ExportFile_Agx_SFBGS);

    connect(ui.menuEdit, &QMenu::aboutToShow, this, &CALUMIMotion::BuildSettingsMenu);
    connect(ui.menuFile, &QMenu::aboutToShow, this, &CALUMIMotion::GetFileMenu);

    connect(ui.actionAbout, &QAction::triggered, this, &CALUMIMotion::ShowAboutDialog);
    connect(ui.actionToggle_Log_Console, &QAction::triggered, this, &CALUMIMotionApplication::ToggleLogger);

    const auto& settings = SettingsRegistry::GetInstance();

    const QByteArray& bytes = settings.GetSavedWindowGeometry("Geometry");
    
    if (settings.UseSavedGeometry() && !bytes.isEmpty())
        restoreGeometry(settings.GetSavedWindowGeometry("Geometry"));
    else
        resize(1280, 800);

    //Drop behavior
    setAcceptDrops(true);
}

CALUMIMotion::~CALUMIMotion() {
    SettingsRegistry::GetInstance().SaveWindowGeometry("Geometry", saveGeometry());
}

void CALUMIMotion::closeEvent(QCloseEvent* event)
{
    CALUMIMotionApplication::UntrackWindow(this);
    QMainWindow::closeEvent(event);
    CALUMIMotionApplication::RequestShutdown();
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

void CALUMIMotion::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }

    QMainWindow::dragEnterEvent(event);
}

void CALUMIMotion::dragLeaveEvent(QDragLeaveEvent* event)
{
    QMainWindow::dragLeaveEvent(event);
}

void CALUMIMotion::dropEvent(QDropEvent* event)
{
    const auto mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        auto urlList = mimeData->urls();
        for (const auto& url : urlList)
        {
            if (url.isLocalFile())
            {
                qDebug() << "Dropped file: " << url.toLocalFile();
                onLoadFile(url.toLocalFile());
            }
        }
        event->acceptProposedAction();
    }

    QMainWindow::dropEvent(event);
}

bool CALUMIMotion::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::Drop)
    {
        // goal is to inform sub widgets that drop is already handled
        qDebug() << "Intercepted Drop by MOTION";
        return true;
    }

    return QMainWindow::eventFilter(object, event);
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
        if (&pair == scene)
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
            if (const auto tab = dynamic_cast<ITabWidget*>(ui.tabWidget->widget(i)))
            {
                ui.tabWidget->setTabText(i, tab->tabTitle());
                ui.tabWidget->tabBar()->setTabTextColor(i, tab->tabTitleColor());
            }
        }
    }
}

void CALUMIMotion::ShowNodeGroupMenu()
{
    if (ui.tabWidget->count() <= 0)
        return;

    const auto gWindow = new QDialog(this);

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
            const auto agxScene = &tabMap.at(agxView);
            const auto newTabView = new NodeGroupMenuPopup(nullptr, *agxScene, agxView, ui.tabWidget);
            tabWidget->addTab(newTabView, ui.tabWidget->tabText(i));

            if (tabWidget->count() == ui.tabWidget->count())
                tabWidget->setCurrentIndex(ui.tabWidget->currentIndex());
        }
    }

    const auto buttonBox = new QDialogButtonBox();
    buttonBox->addButton(QDialogButtonBox::Close);
    pgrid->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::rejected, gWindow, &QDialog::reject);

    gWindow->exec();
    gWindow->deleteLater();
}

void CALUMIMotion::ShowAboutDialog()
{
    QMessageBox::about(this, tr("About"),
                       QString(tr("<b>%1</b><br/>Version %2<br/>Copyright &copy; 2026 %3<br/>All rights reserved")).
                       arg(QCoreApplication::applicationName()).
                       arg(QCoreApplication::applicationVersion()).
                       arg(QCoreApplication::organizationName()));
}

void CALUMIMotion::GetFileMenu() const
{
    BuildFileInOutMenu();
}

void CALUMIMotion::onTabChanged(const int i) const
{
    for (int idx = 0; idx < ui.tabWidget->count(); idx++)
    {
        if (const auto item = static_cast<ITabWidget*>(ui.tabWidget->widget(idx))) // NOLINT(*-pro-type-static-cast-downcast)
        {
            if (i == idx)
                item->onShowMenus();
            else
                QTimer::singleShot(0, item, [item]
                {
                    item->onHideMenus();
                });
        }
    }
}

void CALUMIMotion::BuildSettingsMenu()
{
    //For now, we build on show to keep this action at the bottom of the list. Will add seperator insert item later to optimize this procedure
    const auto settingsA = ui.menuEdit->addAction(tr("Settings"));

    const auto iconPath = oclero::qlementine::icons::iconPath(oclero::qlementine::icons::Icons16::Navigation_Settings);
    const auto ico = GetColoredIconFromSVG(iconPath);
    settingsA->setIcon(ico);

    connect(ui.menuEdit, &QMenu::aboutToHide, settingsA, &QAction::deleteLater);
    connect(settingsA, &QAction::triggered, this, [this] {
            SettingsDialog dialog(this);
            dialog.exec();
        });
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

void CALUMIMotion::BuildFileInOutMenu() const
{
    const auto view = GetAgxViewFromTab(ui.tabWidget->currentIndex());

    ui.actionSave->setEnabled(view != nullptr);
    ui.actionSave_As->setEnabled(view != nullptr);

}

void CALUMIMotion::ExportFile_Agx_SFBGS()
{
    if (ui.tabWidget->count() <= 0) 
    {
        return;
    }

    const AgxGraphicsView* agxView = GetAgxViewFromTab(ui.tabWidget->currentIndex());

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

void CALUMIMotion::onLoadFile(QString filePath)
{
    if (filePath.isEmpty())
    {
        const QString dir = SettingsRegistry::GetInstance().LastDirectory();
        filePath = QFileDialog::getOpenFileName(this, tr("Open File"), dir, tr("Agx Files (*.agx);;Motion Files (*.jagx);;All Files (*.*)"));
    }

    if (filePath.isEmpty())
        return;

    SettingsRegistry::GetInstance().SetLastDirectory(filePath);

    const QFileInfo fileInfo(filePath);
    auto fileType = AgxFileType::UNKNOWN;

    if (!fileInfo.isFile())
    {
        QMessageBox::critical(this, tr("Path Error"), tr("Input Path Is Not A File Type!"));
        return;
    }

    if (fileInfo.suffix().compare("jagx", Qt::CaseInsensitive) == 0)
    {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file: ") + fileInfo.fileName());
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

        if (!doc.isObject())
        {
            return;
        }

        const QJsonObject obj = doc.object();
        fileType = AgxFileTypeFromString(obj.value("file-type").toString());
    }
    else if (fileInfo.suffix().compare("agx", Qt::CaseInsensitive) == 0)
    {
        fileType = AgxFileType::BehaviorFile;
    }

    int idx = -1;

    switch (fileType)
    {
        case AgxFileType::UNKNOWN:
        {
            QMessageBox::critical(this, tr("Unknown File Type"), tr("Unable to open and deserialize file type. Please try a different file!"));
            return;
        }
        case AgxFileType::BehaviorFile:
        {
            const auto graphTab = new GraphTabWidget();
            if (!graphTab->onLoadGraphFile(fileInfo))
            {
                return graphTab->deleteLater();
            }
            idx = ui.tabWidget->addTab(graphTab, graphTab->tabTitle());
            ui.tabWidget->tabBar()->setTabTextColor(idx, graphTab->tabTitleColor());
            tabMap.insert({graphTab->graph(), *graphTab->graph()->agxNodeScene()});
            break;
        }
        case AgxFileType::AnimationFile:
        case AgxFileType::AnimationComponent:
        case AgxFileType::RigFile:
            break;
    }

    if (idx >= 0)
        ui.tabWidget->setCurrentIndex(idx);
}

void CALUMIMotion::Create_SFBGSTab(std::shared_ptr<AgxGraphicsScene> scene, std::shared_ptr<AgxGraphModel> model)
{
    if(!model)
        model = std::make_shared<AgxGraphModel>(AgxGameType::SFBGS);
    if(!scene)
        scene = std::make_shared<AgxGraphicsScene>(model);

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

    tabMap.insert({ newTabView, *scene});

    auto module = new GraphTabWidget();
    module->setGraph(newTabView);

    const auto tabIdx = ui.tabWidget->addTab(module, module->tabTitle());
    ui.tabWidget->tabBar()->setTabTextColor(tabIdx, module->tabTitleColor());
    ui.tabWidget->setCurrentWidget(module);

    module->buildMenus(this);

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
