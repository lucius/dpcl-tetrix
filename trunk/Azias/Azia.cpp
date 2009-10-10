#include "Azia.h"
#include "../cliente/cliente.h"

Azia::Azia(Cliente * _cliente, quint16 _id_tabuleiro) {
    this->cliente = _cliente;
    this->id_tabuleiro = _id_tabuleiro;    
    QObject::connect(this,SIGNAL(end_azia()), this,SLOT(timerTickEnd()));
}
Azia::~Azia() {
}

void Azia::configureAzia() {
    this->timer = new QTimer(this);
    this->ticks = 0;
    this->total_ticks = 5;
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    if(this->id_tabuleiro != this->cliente->meu_id){
        QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(piscaTabuleiro()));
    }
}

void Azia::piscaTabuleiro() {
    QString border_style = this->ticks % 2 == 0 ? "" : "#frame { border:1px solid red;}";
    this->cliente->getTabuleiroById(this->id_tabuleiro)->ui->frame->setStyleSheet(border_style);
}
void Azia::timerTickEnd() {
    qDebug() << " tick end father ";
    this->timer->disconnect(this->timer,SIGNAL(timeout()),this,SLOT(timerTick()));
    this->timer->disconnect(this->timer,SIGNAL(timeout()),this,SLOT(piscaTabuleiro()));
    this->cliente->getTabuleiroById(this->id_tabuleiro)->ui->frame->setStyleSheet("");
}


void Azia::timerTick() {

    this->ticks++;

    if(this->ticks >= this->total_ticks) {
        emit this->end_azia();
    }



}

