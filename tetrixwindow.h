#ifndef TETRIXWINDOW_H
#define TETRIXWINDOW_H

#include <QtGui/QWidget>

namespace Ui
{
    class TetrixWindowClass;
}

class TetrixWindow : public QWidget
{
    Q_OBJECT

public:
    TetrixWindow(QWidget *parent = 0);
    ~TetrixWindow();

private:
    Ui::TetrixWindowClass *ui;
};

#endif // TETRIXWINDOW_H
