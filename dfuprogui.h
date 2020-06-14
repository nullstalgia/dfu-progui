#ifndef DFUPROGUI_H
#define DFUPROGUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class dfuprogui; }
QT_END_NAMESPACE

class dfuprogui : public QMainWindow
{
    Q_OBJECT

public:
    dfuprogui(QWidget *parent = nullptr);
    ~dfuprogui();

private:
    Ui::dfuprogui *ui;
};
#endif // DFUPROGUI_H
