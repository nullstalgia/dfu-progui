#include "bothexplanation.h"
#include "ui_bothexplanation.h"

BothExplanation::BothExplanation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BothExplanation)
{
    ui->setupUi(this);
}

BothExplanation::~BothExplanation()
{
    delete ui;
}

void BothExplanation::on_closeButton_clicked()
{
    this->close();
}
