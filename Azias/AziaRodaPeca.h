#ifndef AZIARODAPECA_H
#define AZIARODAPECA_H


#include "./Azia.h"

class AziaRodaPeca : public Azia {

    public:
        AziaRodaPeca();
        ~AziaRodaPeca();
        void executa(Tabuleiro *);
    public slots:
        void timerTick();
};

#endif // AZIARODAPECA_H
