#include "AziaMaster.h"
#include "../cliente/cliente.h"


AziaMaster::AziaMaster(Cliente * _cliente) {
    this->cliente = _cliente;
}

AziaMaster::~AziaMaster() {
}


void AziaMaster::novaAzia(quint16 _id_tabuleiro) {

    int random = qrand() % 3;

    Azia * azia;

    switch(random) {
        case 0:
            azia = new AziaRodaPeca(this->cliente, _id_tabuleiro);
        break;
        case 1:
            azia = new AziaPontuacao(this->cliente, _id_tabuleiro);
        break;
        case 2:
            azia = new AziaEscondeTabuleiro(this->cliente, _id_tabuleiro);
        break;
    }
    QObject::connect(azia,SIGNAL(end_azia()),this,SLOT(azia_terminou()));

    this->enqueueAzia(azia);


}


void AziaMaster::enqueueAzia(Azia * azia) {
    this->queue_azias.append( azia );

    if(this->queue_azias.count()==1) {
        azia->start();
    }

    qDebug() << "TOTAL AZIAS " << this->queue_azias.count();
}
void AziaMaster::dequeuAzia() {
    this->queue_azias.pop_front();
    qDebug() << "Menos AZIAS " << this->queue_azias.count();
}


void AziaMaster::azia_terminou() {
    this->dequeuAzia();

    if(this->queue_azias.count()>0) {
        this->queue_azias.first()->start();
    }
}
