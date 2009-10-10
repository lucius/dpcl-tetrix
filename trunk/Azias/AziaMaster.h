#ifndef AZIAMASTER_H
#define AZIAMASTER_H

#include "./Azia.h"
#include "./AziaRodaPeca.h"
#include "./AziaPontuacao.h"
#include "./AziaEscondeTabuleiro.h"


#include <QMap>
#include <QVector>
#include <QObject>


class Cliente;

class AziaMaster : public QObject {
    Q_OBJECT

     public:
        AziaMaster( Cliente *);
        virtual ~AziaMaster();

    public slots:
        void novaAzia(quint16);
        void azia_terminou();

    private:
        Cliente * cliente;
        void enqueueAzia(Azia *);
        void dequeuAzia();
        QVector<Azia *> queue_azias;

};

#endif // AZIAMASTER_H
