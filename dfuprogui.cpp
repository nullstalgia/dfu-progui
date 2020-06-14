#include "dfuprogui.h"
#include "ui_dfuprogui.h"

#include "bothexplanation.h"
#include "aboutbox.h"

dfuprogui::dfuprogui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dfuprogui)
{
    ui->setupUi(this);
    disableColonOptions();
    //SetComboBoxItemEnabled(ui->targetBox, 0, false);
    //SetComboBoxItemEnabled(ui->targetBox, 14, false);
    //SetComboBoxItemEnabled(ui->targetBox, 20, false);
    //SetComboBoxItemEnabled(ui->targetBox, 58, false);
    loadSettings();
}

dfuprogui::~dfuprogui()
{
    saveSettings();
    delete ui;
}

void dfuprogui::loadSettings()
{
    // Recent Files
    QComboBox* comboBox = ui->fileComboBox;
    QString recentFilesImport = appSettings.value("recentFiles").toString();
    if(recentFilesImport.isEmpty() == false){
        recentFiles = recentFilesImport.split(",");
        comboBox->addItems(recentFiles);
    }

    // Last Target Index
    int lastTargetIndex = appSettings.value("lastTargetIndex", 1).toInt();
    ui->targetBox->setCurrentIndex(lastTargetIndex);

    // Radio Buttons for Flash, EEPROM, Both
    int loadArea = appSettings.value("loadArea", 0).toInt();
    int saveArea = appSettings.value("saveArea", 0).toInt();
    if(loadArea == 1){
        ui->radioEEPROMLoad->setChecked(true);
    } else if (loadArea == 2){
        ui->radioBothLoad->setChecked(true);
    }
    if(saveArea == 1){
        ui->radioEEPROMSave->setChecked(true);
    } else if (saveArea == 2){
        ui->radioBothSave->setChecked(true);
    }

    // Extra Args
    ui->extraArgumentsField->setText(appSettings.value("extraArgs", "").toString());

    // Save Location
    ui->saveFileEdit->setText(appSettings.value("saveLocation", "").toString());

    // Force/Auto checkboxes
    ui->autoErase->setChecked((appSettings.value("autoErase", false).toBool()));
    ui->autoFlash->setChecked((appSettings.value("autoFlash", false).toBool()));
    ui->autoLaunch->setChecked((appSettings.value("autoLaunch", false).toBool()));

    ui->forceErase->setChecked((appSettings.value("forceErase", false).toBool()));
    ui->forceFlash->setChecked((appSettings.value("forceFlash", false).toBool()));
}

void dfuprogui::saveSettings()
{
    //QComboBox* comboBox = ui->fileComboBox;
    /*
    if(recentFiles.count() != 0)
    {
        recentFilesExport += recentFiles[0];
        if(recentFiles.count() != 1)
        {
            for( int i = 1; i < recentFiles.count(); i++ )
            {
                recentFilesExport += ",";
                recentFilesExport += recentFiles[i];
            }
        }
    }*/

    // Recent Files
    QString recentFilesExport;
    recentFilesExport = recentFiles.join(",");
    appSettings.setValue("recentFiles", recentFilesExport);

    // Last Target Index
    appSettings.setValue("lastTargetIndex", ui->targetBox->currentIndex());

    // Radio Buttons for Flash, EEPROM, Both
    int loadArea = 0;
    if (ui->radioEEPROMLoad->isChecked()){
        loadArea = 1;
    } else if (ui->radioBothLoad->isChecked()){
        loadArea = 2;
    }
    int saveArea = 0;
    if (ui->radioEEPROMSave->isChecked()){
        saveArea = 1;
    } else if (ui->radioBothSave->isChecked()){
        saveArea = 2;
    }

    appSettings.setValue("loadArea", loadArea);
    appSettings.setValue("saveArea", saveArea);

    // Extra Args
    appSettings.setValue("extraArgs", ui->extraArgumentsField->text());

    // Save Location
    appSettings.setValue("saveLocation", ui->saveFileEdit->text());

    // Force/Auto checkboxes
    appSettings.setValue("autoErase", ui->autoErase->isChecked());
    appSettings.setValue("autoFlash", ui->autoFlash->isChecked());
    appSettings.setValue("autoLaunch", ui->autoLaunch->isChecked());

    appSettings.setValue("forceErase", ui->forceErase->isChecked());
    appSettings.setValue("forceFlash", ui->forceFlash->isChecked());
}

// Yoinked from https://stackoverflow.com/a/62261745
void dfuprogui::SetComboBoxItemEnabled(QComboBox * comboBox, int index, bool enabled)
{
    auto * model = qobject_cast<QStandardItemModel*>(comboBox->model());
    assert(model);
    if(!model) return;

    auto * item = model->item(index);
    assert(item);
    if(!item) return;
    item->setEnabled(enabled);
}

void dfuprogui::disableColonOptions()
{
    QComboBox* comboBox = ui->targetBox;

    for( int i = 0; i < comboBox->count(); i++ )
    {
        if(comboBox->itemText( i ).contains(":", Qt::CaseInsensitive)){
            SetComboBoxItemEnabled(comboBox, i, false);
        }
    }
}

void dfuprogui::on_openFileButton_clicked()
{
    QDir directory;

    QString fileName = QFileDialog::getOpenFileName(this, ("Open File"),
                                                      appSettings.value("defaultDir", directory.currentPath()).toString(),
                                                      ("Intel Hex Files (*.ihex *.hex *.eep);;All Files (*)"));

    if(fileName.isNull() == false){
        ui->fileComboBox->clear();
        recentFiles.prepend(fileName);
        if(recentFiles.length() > maxRecentFiles){
            recentFiles.removeLast();
        }
        ui->fileComboBox->addItems(recentFiles);
        appSettings.setValue("defaultDir", directory.absoluteFilePath(fileName));
    }
}

void dfuprogui::on_actionClear_Recent_Files_triggered()
{
    recentFiles.clear();
    ui->fileComboBox->clear();
}

void dfuprogui::on_bothButton_clicked()
{
    BothExplanation dlg;
    dlg.setModal(true);
    dlg.show();
    dlg.exec();
}

void dfuprogui::printDFUOutput(QString extraOutput)
{
        //ui->dfuOutput->appendPlainText(dfu->readAllStandardOutput());


        if(extraOutput.isEmpty() == false){
            ui->dfuOutput->insertPlainText(extraOutput+"\n");
        } else {
            ui->dfuOutput->insertPlainText(dfu->readAll());
        }

        ui->dfuOutput->moveCursor(QTextCursor::End);
}

void dfuprogui::flashChip(bool flash, bool eeprom)
{
    QStringList arguments;
    // First argument is the target chip
    arguments.append(ui->targetBox->currentText());
    // After that is the action
    arguments.append("flash");
    arguments.append(ui->fileComboBox->currentText());
    // If force is checked.
    if(ui->forceFlash->isChecked()){
        arguments.append("--force");
    }

    if(eeprom && flash){
        dfuQueue.enqueue(arguments);
        arguments[2] += ".eep";
        arguments.append("--eeprom");
    } else if (eeprom){
        arguments.append("--eeprom");
    }

    dfuQueue.enqueue(arguments);
}

void dfuprogui::dfuQueueAction(int exitCode, QProcess::ExitStatus exitStatus){
    // So, when you run an erase on an already blank MCU, it returns "5"
    // And I can't find a list of error codes, so this is just what I'm doing.
    bool exitCodeOK = (exitCode == 0 || exitCode == 5);
    if (exitCodeOK && exitStatus == QProcess::NormalExit && !dfuQueue.isEmpty()){
        QStringList arguments = dfuQueue.dequeue();
        arguments.append(ui->extraArgumentsField->text().split(" "));
        dfu = new QProcess(this);
        dfu->setProcessChannelMode(QProcess::MergedChannels);
        if(arguments[1] == "read"){
            dfu->setStandardOutputFile(arguments[2]);
            printDFUOutput("Saving to: "+arguments[2]+"");
            arguments.removeAt(2);
        } else {
            connect( dfu, SIGNAL(readyReadStandardOutput()), this, SLOT(printDFUOutput()));
            connect( dfu, SIGNAL(readyReadStandardError()), this, SLOT(printDFUOutput()));
        }
        connect( dfu, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(dfuQueueAction(int, QProcess::ExitStatus)));
        printDFUOutput(arguments.join(" "));
        dfu->start("dfu-programmer", arguments);
    } else if (exitCodeOK && exitStatus == QProcess::NormalExit && dfuQueue.isEmpty()){
        printDFUOutput("Finished!");
    } else {
        dfuQueue.clear();
        dfu->kill();
        printDFUOutput("Done, but with errors?");
    }
}

void dfuprogui::eraseChip()
{
    QStringList arguments;
    // First argument is the target chip
    arguments.append(ui->targetBox->currentText());
    // After that is the action
    arguments.append("erase");
    // If force is checked.
    if(ui->forceErase->isChecked()){
        arguments.append("--force");
    }

    dfuQueue.enqueue(arguments);
}
void dfuprogui::saveChip(bool flash, bool eeprom)
{
    QStringList arguments;
    // First argument is the target chip
    arguments.append(ui->targetBox->currentText());
    // After that is the action
    arguments.append("read");
    arguments.append(ui->saveFileEdit->text());
    // If force is checked.

    // This function is meant to write directly to a .hex (or .eep) file
    // So this makes sure any errors like (this is empty) isn't written
    arguments.append("--quiet");

    if(eeprom && flash){
        dfuQueue.enqueue(arguments);
        // So, you can't actually give dfu-programmer a file to save to (for some reason...)
        // In order to avoid weird cases, this is kept to give the queue a file to point to
        // And once the queue sets said file, it removes the element as to not cause issues
        // with dfu-programmer.
        arguments[2] += ".eep";
        arguments.append("--eeprom");
    } else if (eeprom){
        arguments.append("--eeprom");
    }

    dfuQueue.enqueue(arguments);
}

void dfuprogui::launchChip()
{
    QStringList arguments;
    // First argument is the target chip
    arguments.append(ui->targetBox->currentText());
    // After that is the action
    arguments.append("launch");
    dfuQueue.enqueue(arguments);
}

void dfuprogui::on_clearOutputButton_clicked()
{
    ui->dfuOutput->clear();
}


void dfuprogui::on_eraseButton_clicked()
{
    eraseChip();
    startQueue();
}

void dfuprogui::on_flashButton_clicked()
{
    bool flash = ui->radioFlashLoad->isChecked() || ui->radioBothLoad->isChecked();
    bool eeprom = ui->radioEEPROMLoad->isChecked() || ui->radioBothLoad->isChecked();
    flashChip(flash, eeprom);
    startQueue();
}

void dfuprogui::on_launchButton_clicked()
{
    launchChip();
    startQueue();
}

void dfuprogui::on_autoButton_clicked()
{
    bool flash = ui->radioFlashLoad->isChecked() || ui->radioBothLoad->isChecked();
    bool eeprom = ui->radioEEPROMLoad->isChecked() || ui->radioBothLoad->isChecked();
    if(ui->autoErase->isChecked()){
        eraseChip();
    }
    if(ui->autoFlash->isChecked()){
        flashChip(flash, eeprom);
    }
    if(ui->autoLaunch->isChecked()){
        launchChip();
    }
    startQueue();
}

void dfuprogui::on_saveFileStart_clicked()
{
    bool flash = ui->radioFlashSave->isChecked() || ui->radioBothSave->isChecked();
    bool eeprom = ui->radioEEPROMSave->isChecked() || ui->radioBothSave->isChecked();
    saveChip(flash, eeprom);
    startQueue();
}

void dfuprogui::on_saveFileSelect_clicked()
{
    QDir directory;

    QString fileName = QFileDialog::getSaveFileName(this, ("Save File"),
                                                      appSettings.value("defaultDir", directory.currentPath()).toString(),
                                                      ("Intel Hex Files (*.ihex *.hex *.eep)"));

    if(fileName.isNull() == false){
        ui->saveFileEdit->setText(fileName);
        appSettings.setValue("defaultDir", directory.absoluteFilePath(fileName));
    }
}

void dfuprogui::startQueue(){
    dfuQueueAction(0, QProcess::NormalExit);
}

void dfuprogui::on_actionQuit_triggered()
{
    this->close();
}

void dfuprogui::on_actionAbout_triggered()
{
    AboutBox dlg;
    dlg.setModal(true);
    dlg.show();
    dlg.exec();
}
