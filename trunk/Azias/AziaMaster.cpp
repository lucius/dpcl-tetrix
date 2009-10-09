#include "AziaMaster.h"



AziaMaster::AziaMaster() {
}
AziaMaster::~AziaMaster() {
}


void AziaMaster::novaAzia(Tabuleiro * tabuleiro) {
    int random = 1;
    Azia * azia;

    switch(random) {
        case 1:
            azia = new AziaRodaPeca();
        break;
        case 2:
            azia = new AziaRodaPeca();
        break;
    }

    this->setTabuleiro(tabuleiro);

    QObject::connect(azia,SIGNAL(end_azia()),this,SLOT(azia_terminou()));

    this->enqueueAzia(azia);

}

void AziaMaster::setTabuleiro(Tabuleiro * tabuleiro) {
    this->tabuleiro = tabuleiro;
}
Tabuleiro * AziaMaster::getTabuleiro() {
    return this->tabuleiro;
}

void AziaMaster::enqueueAzia(Azia * azia) {
    this->queue_azias.append( azia );

    if(this->queue_azias.count()==1) {
        azia->executa(this->getTabuleiro());
    }
}
void AziaMaster::dequeuAzia() {
    this->queue_azias.pop_back();
}


void AziaMaster::azia_terminou() {
    this->dequeuAzia();

    if(this->queue_azias.count()>1) {
        this->queue_azias.first()->executa(this->getTabuleiro());
    }
}
