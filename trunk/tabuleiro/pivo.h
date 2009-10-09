#ifndef PECA_H
#define PECA_H

#include <QPair>
#include <QPoint>
#include <QTimer>
#include <QVector>
#include <QWidget>

#include "mapapecas.h"
#include "pecinha.h"


namespace Tab
{
    class Pivo : public Tab::Pecinha
    {
        Q_OBJECT

        public:
            Pivo( qint8, QColor, Tab::XyView, QWidget* _parent );

            ~Pivo( );

            Tab::TipoPeca
            getTipoPeca( );

            qint8
            getPecaInt();

            Tab::XyView
            getOndeEstou();

            QColor
            cor;

        public slots:
            void
            rotaciona( );

            void
            desce( );

            unsigned int
            desceTudo( );

            void
            moveDireita( );

            void
            moveEsquerda( );

            bool
            possoColocarAqui( Tab::TipoPeca, Tab::XyView );
        signals:
            void
            mudou( Tab::XyView );

            void
            colidiu( );

        private:
            qint8
            tipoPecaInt;

            Tab::TipoPeca
            peca;
            
            QVector<Tab::Pecinha*>
            agregadas;

            Tab::XyView
            ondeEuToExibido;

            void
            configuraAgregados( );
    };
}

#endif // PECA_H
