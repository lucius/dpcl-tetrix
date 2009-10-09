#ifndef AZIA_H
#define AZIA_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include "../tabuleiro/tabuleiro.h"

class Azia : public QObject  {
    Q_OBJECT
    public:
        Azia(QObject* _parent = 0);
        virtual ~Azia();
        virtual void executa(Tabuleiro *);
        int ticks;
    public slots:
        virtual void timerTick();
    signals:
        void start_azia();
        void end_azia();

    protected:
       QTimer * timer;       

};

#endif // AZIA_H
