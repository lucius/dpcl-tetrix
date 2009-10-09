#include "AziaRodaPeca.h"
#include "../cliente/cliente.h"

AziaRodaPeca::AziaRodaPeca(Cliente * _cliente, quint16 _id_tabuleiro) : Azia(_cliente, _id_tabuleiro) {
    this->configureAzia();
}
AziaRodaPeca::~AziaRodaPeca() {
}

void AziaRodaPeca::configureAzia() {
    Azia::configureAzia();
    this->total_ticks = 100;
}
void AziaRodaPeca::start() {
    this->timer->start(200);
}
void AziaRodaPeca::timerTick() {
    Azia::timerTick();

    if(this->id_tabuleiro == this->cliente->meu_id) {
        this->cliente->pecaRotaciona();
    }
}
