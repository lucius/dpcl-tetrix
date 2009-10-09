#ifndef AZIAMASTER_H
#define AZIAMASTER_H

#include "./Azia.h"
#include "./AziaRodaPeca.h"
#include "../tabuleiro/tabuleiro.h"

#include <QMap>
#include <QVector>
#include <QObject>

class AziaMaster :public QObject {
     Q_OBJECT
     public:
        AziaMaster( );
        ~AziaMaster();
        void setTabuleiro( Tabuleiro *);
        Tabuleiro * getTabuleiro();

    public slots:
        void novaAzia(Tabuleiro *);
        void azia_terminou();

    private:
        Tabuleiro * tabuleiro;
        void enqueueAzia(Azia *);
        void dequeuAzia();

        QVector<Azia *> queue_azias;
};

#endif // AZIAMASTER_H
