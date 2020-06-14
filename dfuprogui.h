#ifndef DFUPROGUI_H
#define DFUPROGUI_H

#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class dfuprogui; }
QT_END_NAMESPACE

class dfuprogui : public QMainWindow
{
    Q_OBJECT

public:
    dfuprogui(QWidget *parent = nullptr);
    ~dfuprogui();

private slots:
    void on_actionClear_Recent_Files_triggered();

    void on_bothButton_clicked();

    void on_openFileButton_clicked();

    void on_flashButton_clicked();

    void printDFUOutput(QString extraOutput = "");
    void dfuQueueAction(int exitCode, QProcess::ExitStatus exitStatus);

    void on_clearOutputButton_clicked();

    void on_saveFileStart_clicked();

    void on_saveFileSelect_clicked();

    void on_eraseButton_clicked();

    void on_launchButton_clicked();

    void on_autoButton_clicked();

    void on_actionQuit_triggered();

    void on_actionAbout_triggered();

private:
    Ui::dfuprogui *ui;
    void SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled);
    void disableColonOptions();
    QString fileName;
    QStringList recentFiles;
    const int maxRecentFiles = 10;
    QSettings appSettings;

    void loadSettings();
    void saveSettings();

    QProcess* dfu;

    void startQueue();

    void flashChip(bool flash, bool eeprom);
    void eraseChip();
    void launchChip();
    void saveChip(bool flash, bool eeprom);

    QQueue<QStringList> dfuQueue;
};
#endif // DFUPROGUI_H
