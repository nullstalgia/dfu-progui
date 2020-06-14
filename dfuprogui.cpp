#include "dfuprogui.h"
#include "ui_dfuprogui.h"

dfuprogui::dfuprogui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::dfuprogui)
{
    ui->setupUi(this);
}

dfuprogui::~dfuprogui()
{
    delete ui;
}

