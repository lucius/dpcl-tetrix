#include "AziaEscondeTabuleiro.h"
#include "../cliente/cliente.h"

AziaEscondeTabuleiro::AziaEscondeTabuleiro(Cliente * _cliente, quint16 _id_tabuleiro) : Azia(_cliente, _id_tabuleiro) {
    this->configureAzia();
    QObject::connect(this,SIGNAL(end_azia()), this,SLOT(timerTickEnd()));
}
AziaEscondeTabuleiro::~AziaEscondeTabuleiro() {
}

void AziaEscondeTabuleiro::configureAzia() {
    Azia::configureAzia();
    this->total_ticks = 50;
}
void AziaEscondeTabuleiro::start() {
    this->timer->start(200);
}
void AziaEscondeTabuleiro::timerTick() {
    Azia::timerTick();
    qDebug() << "da azia escode tabuleiro";
    if(this->id_tabuleiro == this->cliente->meu_id) {

        if(this->ticks % 5!=0) {
           qDebug() << "fuck";
           this->cliente->getTabuleiroById(this->id_tabuleiro)->ui->parede->setStyleSheet("background-color: black;");
        } else {
            qDebug() << "blzzz'";
            this->cliente->getTabuleiroById(this->id_tabuleiro)->ui->parede->setStyleSheet("");
        }
    }
}

void AziaEscondeTabuleiro::timerTickEnd() {
     this->timer->disconnect(this->timer,SIGNAL(timeout()),this,SLOT(timerTick()));
    if(this->id_tabuleiro == this->cliente->meu_id) {
        qDebug() << " tick end filho ";
        this->cliente->getTabuleiroById(this->id_tabuleiro)->ui->parede->setStyleSheet("");
    }
}
