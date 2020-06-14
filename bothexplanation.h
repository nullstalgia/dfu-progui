#ifndef BOTHEXPLANATION_H
#define BOTHEXPLANATION_H

#include <QDialog>

namespace Ui {
class BothExplanation;
}

class BothExplanation : public QDialog
{
    Q_OBJECT

public:
    explicit BothExplanation(QWidget *parent = nullptr);
    ~BothExplanation();

private slots:
    void on_closeButton_clicked();

private:
    Ui::BothExplanation *ui;
};

#endif // BOTHEXPLANATION_H
