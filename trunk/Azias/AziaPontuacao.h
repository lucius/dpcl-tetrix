#ifndef AZIAPONTUACAO_H
#define AZIAPONTUACAO_H


#include "./Azia.h"

class Cliente;

class AziaPontuacao : public Azia {

    Q_OBJECT

    public:
        AziaPontuacao(Cliente *, quint16);
        ~AziaPontuacao();
        void configureAzia();
        void start();
    public slots:
        void timerTick();
};

#endif // AZIAPONTUACAO_H
