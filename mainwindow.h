#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "portcontrol.h"
#include "datatextview.h"
#include "demoreader.h"
#include "ui_about_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void closeEvent(QCloseEvent * event);
    /// Stores settings for all modules
    void saveAllSettings(QSettings* settings);
    /// Load settings for all modules
    void loadAllSettings(QSettings* settings);
    /// Stores main window settings into a `QSettings`
    void saveMWSettings(QSettings* settings);
    /// Loads main window settings from a `QSettings`
    void loadMWSettings(QSettings* settings);
    void setupAboutDialog();
private slots:
    void on_actionSerialPort_triggered();

    void on_actionSave_Settings_triggered();

    void on_actionLoad_Settings_triggered();
private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    PortControl portControl;
    Sink sink;
    DemoReader demoReader;
    DataTextView dataTextView;
    QDialog aboutDialog;
};
#endif // MAINWINDOW_H
