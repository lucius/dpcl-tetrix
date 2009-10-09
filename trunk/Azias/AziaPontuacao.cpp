#include "AziaPontuacao.h"
#include "../cliente/cliente.h"

AziaPontuacao::AziaPontuacao(Cliente * _cliente, quint16 _id_tabuleiro) : Azia(_cliente, _id_tabuleiro) {
    this->configureAzia();
}
AziaPontuacao::~AziaPontuacao() {
}

void AziaPontuacao::configureAzia() {
    Azia::configureAzia();
    this->total_ticks = 25;
}
void AziaPontuacao::start() {
    this->timer->start(50);
}
void AziaPontuacao::timerTick() {
    Azia::timerTick();
    int pontuacao = this->cliente->getTabuleiroById(this->id_tabuleiro)->getPontuacao();

    this->cliente->getTabuleiroById(this->id_tabuleiro)->setPontuacao(pontuacao-1);
}
