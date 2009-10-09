#ifndef AZIA_H
#define AZIA_H

#include <QObject>
#include <QTimer>
#include <QDebug>



class Cliente;

class Azia : public QObject  {
    Q_OBJECT
    public:
        Azia(Cliente *, quint16);
        virtual ~Azia();
        virtual void start() = 0 ;
        int ticks;
        int total_ticks;
        virtual void configureAzia();

    public slots:
        virtual void timerTick();
        void timerTickEnd();
        void piscaTabuleiro();
    signals:
        void start_azia();
        void end_azia();

    protected:
        QTimer * timer;       
        Cliente * cliente;
        quint16 id_tabuleiro;

};

#endif // AZIA_H
