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
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(piscaTabuleiro()));
}

void Azia::piscaTabuleiro() {
    QString border_style = this->ticks % 2 == 0 ? "" : "border:3px solid red";
    this->cliente->getTabuleiroById(this->id_tabuleiro)->setStyleSheet(border_style);
}
void Azia::timerTickEnd() {    
    this->timer->disconnect(this->timer,SIGNAL(timeout()),this,SLOT(timerTick()));
}


void Azia::timerTick() {

    QString border_style = this->ticks % 2 == 0 ? "" : "border:3px solid red";

    this->cliente->getTabuleiroById(this->id_tabuleiro)->setStyleSheet(border_style);
    if(this->ticks >= this->total_ticks) {
        emit this->end_azia();
    }

    this->ticks++;

}

