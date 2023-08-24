#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include "setting_defines.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <datatextview.h>
#include <QDockWidget>
#include <QSplitter>
#include <form.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , portControl(&serialPort)
    , demoReader(&serialPort)
    , dataTextView(&sink)
    , aboutDialog(this)
{
    ui->setupUi(this);
    setWindowTitle(PROGRAM_NAME);
    setWindowIcon(QIcon(":/toolboxes_icon.png"));
    addToolBar(portControl.toolBar());

    setupAboutDialog();

    //init dock widget
    QDockWidget *dockWidget = new QDockWidget(tr("Text View"),this);
    dockWidget->setWidget(&dataTextView);
    dockWidget->setObjectName("textView");
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);


    //init view menu
    QMenu* tbMenu = new QMenu(tr("Tool Bar"));
    tbMenu->addAction(portControl.toolBar()->toggleViewAction());

    ui->menuview->addAction(dockWidget->toggleViewAction());
    ui->menuview->addMenu(tbMenu);

    // load default settings
    QSettings settings(PROGRAM_NAME, PROGRAM_NAME);
    loadAllSettings(&settings);

    //init status bar
    statusBar()->showMessage(tr("Ready"));

    demoReader.enable(true);
    demoReader.connectSink(&sink);

    // Help menu signals
    QObject::connect(ui->actionAbout, &QAction::triggered,
              &aboutDialog, &QWidget::show);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent * event)
{
//    // save snapshots
//    if (!snapshotMan.isAllSaved())
//    {
//        auto clickedButton = QMessageBox::warning(
//            this, "Closing SerialPlot",
//            "There are un-saved snapshots. If you close you will loose the data.",
//            QMessageBox::Discard, QMessageBox::Cancel);
//        if (clickedButton == QMessageBox::Cancel)
//        {
//            event->ignore();
//            return;
//        }
//    }
    // save settings
    QSettings settings(PROGRAM_NAME, PROGRAM_NAME);
    saveAllSettings(&settings);
    settings.sync();

    if (settings.status() != QSettings::NoError)
    {
        QString errorText;

        if (settings.status() == QSettings::AccessError)
        {
            QString file = settings.fileName();
            errorText = QString("Serialplot cannot save settings due to access error. \
This happens if you have run serialplot as root (with sudo for ex.) previously. \
Try fixing the permissions of file: %1, or just delete it.").arg(file);
        }
        else
        {
            errorText = QString("Serialplot cannot save settings due to unknown error: %1").\
                arg(settings.status());
        }

        auto button = QMessageBox::critical(
            NULL,
            "Failed to save settings!", errorText,
            QMessageBox::Cancel | QMessageBox::Ok);

        if (button == QMessageBox::Cancel)
        {
            event->ignore();
            return;
        }
    }

    QMainWindow::closeEvent(event);
}


void MainWindow::saveAllSettings(QSettings* settings)
{
    portControl.saveSettings(settings);
    saveMWSettings(settings);
}

void MainWindow::loadAllSettings(QSettings* settings)
{
    portControl.loadSettings(settings);
    loadMWSettings(settings);
}

void MainWindow::saveMWSettings(QSettings* settings)
{
    // save window geometry
    settings->beginGroup(SettingGroup_MainWindow);
    settings->setValue(SG_MainWindow_Size, size());
    settings->setValue(SG_MainWindow_Pos, pos());
//    // save active panel
//    settings->setValue(SG_MainWindow_ActivePanel,
//                       panelSettingMap.value(ui->tabWidget->currentIndex()));
//    // save panel minimization
//    settings->setValue(SG_MainWindow_HidePanels,
//                       ui->tabWidget->hideAction.isChecked());
    // save window maximized state
    settings->setValue(SG_MainWindow_Maximized,
                       bool(windowState() & Qt::WindowMaximized));
    // save toolbar/dockwidgets state
    settings->setValue(SG_MainWindow_State, saveState());
    settings->endGroup();
}
void MainWindow::loadMWSettings(QSettings* settings)
{
    settings->beginGroup(SettingGroup_MainWindow);
    // load window geometry
    resize(settings->value(SG_MainWindow_Size, size()).toSize());
    move(settings->value(SG_MainWindow_Pos, pos()).toPoint());

//    // set active panel
//    QString tabSetting =
//        settings->value(SG_MainWindow_ActivePanel, QString()).toString();
//    ui->tabWidget->setCurrentIndex(
//        panelSettingMap.key(tabSetting, ui->tabWidget->currentIndex()));

//    // hide panels
//    ui->tabWidget->hideAction.setChecked(
//        settings->value(SG_MainWindow_HidePanels,
//                        ui->tabWidget->hideAction.isChecked()).toBool());

    // maximize window
    if (settings->value(SG_MainWindow_Maximized).toBool())
    {
        showMaximized();
    }

    // load toolbar/dockwidgets state
    restoreState(settings->value(SG_MainWindow_State).toByteArray());
    settings->setValue(SG_MainWindow_State, saveState());

    settings->endGroup();
}

void MainWindow::on_actionSerialPort_triggered()
{
    portControl.setParent(this);
    portControl.setWindowTitle("Advanced Serial Settings");
    portControl.setWindowFlags(portControl.windowFlags()| Qt::Dialog);
    portControl.setWindowModality(Qt::ApplicationModal);
    portControl.setWindowIcon(QIcon(":/serial_connect_icon.png"));
    portControl.show();
}


void MainWindow::on_actionSave_Settings_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save Settings"), QString(), "INI (*.ini)");

    if (!fileName.isNull()) // user canceled
    {
        QSettings settings(fileName, QSettings::IniFormat);
        saveAllSettings(&settings);
    }
}


void MainWindow::on_actionLoad_Settings_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Load Settings"), QString(), "INI (*.ini)");

    if (!fileName.isNull()) // user canceled
    {
        QSettings settings(fileName, QSettings::IniFormat);
        loadAllSettings(&settings);
    }
}


void MainWindow::setupAboutDialog()
{
    Ui_AboutDialog uiAboutDialog;
    uiAboutDialog.setupUi(&aboutDialog);

    QString aboutText = uiAboutDialog.lbAbout->text();
    aboutText.replace("$VERSION_STRING$", VERSION_STRING);
    uiAboutDialog.lbAbout->setText(aboutText);
}
