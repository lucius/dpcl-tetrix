    #ifndef CLIENTE_H
#define CLIENTE_H

#include <QMap>
#include <QString>
#include <QWidget>
#include <QProcess>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QSignalMapper>

#include "../tabuleiro/tabuleiro.h"
#include "../rede_cliente/tradutor.h"


#include "janelatelachat.h"
#include "janelatelalogin.h"

class Cliente : public QWidget
{
    Q_OBJECT
public:
    Cliente( QWidget* = 0);

    ~Cliente();

public slots:

    void
    modoJogoCliente( QString _nome , QString _ip_servidor);

    void
    modoJogoServer( QString _nome , quint8 _n_jogadores );

    void
    modoJogoSingle( QString _nome );

    void
    redeErroConexao( QString _erro );

    void
    gameOver( int );

    void
    redeConectado(quint16);

    void
    novoJogadorConectado( quint16, QString );

    void jogadorPronto(bool);

    void daAzia(int);

    void
    incomingRotacionapeca(quint16);

    void
    incomingNovaPeca(quint16,quint16);

    void
    incomingDesceUmPouco(quint16);

    void
    incomingMovimentaEsquerda(quint16);

    void
    incomingMovimentaDireita(quint16);

    void
    incomingAziaFrenetica(quint16);

    void
    daAziaFrenetica(QString, QString);

    void
    levelMudou();


protected slots:

    void
    startJogo();

    void
    stdOutServer();

    void
    jogadorDown( quint16 );

    void
    jogadorOver( quint16 _id_jogador , quint16 _pontuacao);
	
protected:

    void
    keyPressEvent(QKeyEvent *);

private:

    Tabuleiro*
    tabuleiro_principal;

    QMap< int, Tabuleiro* >
    outros_tabuleiros;

    Rede_Cliente::Tradutor*
    rede;

    JanelaTelaLogin*
    janela_telalogin;

    JanelaTelaChat*
    janela_telachat;

    QProcess*
    server_process;

    QFileInfo*
    server_bin;

    bool
    game_over;

    bool
    game_started;

    quint16
    meu_id;

    void
    connectsIncommingDataFromNet();

    void
    telaloginInit();

    void
    telachatInit();

    void
    startServerProcess( quint8 _n_jogadores );

    Tabuleiro*
    getTabuleiroById( quint16 );

    void
    initTrilhaSonora();
};

#endif // CLIENTE_H
