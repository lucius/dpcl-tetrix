#include "Azia.h"


Azia::Azia(QObject* _parent ): QObject(_parent) {

    this->timer = new QTimer(this);

    connect(this->timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    this->timer->start(1000);
}
Azia::~Azia() {
}

void Azia::executa(Tabuleiro * tabuleiro) {
    qDebug() << "AZIA";
}
void Azia::timerTick() {
    qDebug() << "timerTICK";
}
