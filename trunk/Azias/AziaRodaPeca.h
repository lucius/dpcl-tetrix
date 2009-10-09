#ifndef AZIARODAPECA_H
#define AZIARODAPECA_H


#include "./Azia.h"

class Cliente;

class AziaRodaPeca : public Azia {

    Q_OBJECT

    public:
        AziaRodaPeca(Cliente *, quint16);
        ~AziaRodaPeca();
        void configureAzia();
        void start();
    public slots:
        void timerTick();
};

#endif // AZIARODAPECA_H
