#include "tetrixwindow.h"
#include "ui_tetrixwindow.h"

TetrixWindow::TetrixWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::TetrixWindowClass)
{
    ui->setupUi(this);
}

TetrixWindow::~TetrixWindow()
{
    delete ui;
}
