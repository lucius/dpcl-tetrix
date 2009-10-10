#ifndef AZIAESCONDETABULEIRO_H
#define AZIAESCONDETABULEIRO_H

#include "./Azia.h"
#include "../cliente/ui_tabuleiro.h"
#include <QWidget>

class Cliente;

class AziaEscondeTabuleiro : public Azia
{

    Q_OBJECT

    public:
        AziaEscondeTabuleiro(Cliente *, quint16);
        ~AziaEscondeTabuleiro();
        void configureAzia();
        void start();

    public slots:
        void timerTick();
        void timerTickEnd();
};

#endif // AZIAESCONDETABULEIRO_H
