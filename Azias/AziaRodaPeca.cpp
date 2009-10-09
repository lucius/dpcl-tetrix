#include "AziaRodaPeca.h"

AziaRodaPeca::AziaRodaPeca() {
    qDebug() << "AziaRodaPeca";
}
AziaRodaPeca::~AziaRodaPeca() {
    qDebug() << "~AziaRodaPeca";
}


void AziaRodaPeca::executa(Tabuleiro * tabuleiro) {
    qDebug() << "Executa roda Peca";
}


void AziaRodaPeca::timerTick() {
    this->ticks++;       
    qDebug() << "Roda Peca TIMER TICK";

    if(this->ticks == 5) {
        qDebug() << "FIM Roda Peca";
        this->timer->disconnect(this,SLOT(timerTick()));
        emit this->end_azia();
    }
}
