#ifndef TESTE_H
#define TESTE_H

#include <QMap>
#include <QString>
#include <QWidget>
#include <QProcess>
#include <QFileInfo>

#include "tabuleiro.h"
#include "../rede_cliente/tradutor.h"

#include "ui_tela_login.h"
#include "ui_tela_chat.h"

class Teste : public QWidget
{
    Q_OBJECT
public:
    Teste( QWidget* = 0);
    ~Teste();

public slots:

    void
    telaloginPedidoSlot();

    void
    redeErroConexao( QString );

    void
    gameOver( int );

    void
    redeConectado(quint16);

    void
    novoJogadorConectado( quint16, QString );

    void
    enviaChat();

    void
    recebeChat( QString, QString);

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

protected slots:

    void
    startJogo();

    void
    stdOutServer();

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

    Ui::Conectar*
    telalogin;

    QWidget*
    janela_telalogin;

    Ui::TelaChat*
    telachat;

    QWidget*
    janela_telachat;

    QProcess*
    server_process;

    QFileInfo*
    server_bin;

    bool
    game_over;

    bool
    game_started;

    bool
    single_player;

    quint16
    meu_id;

    void
    connectsIncommingDataFromNet();

    void
    telaloginInit();

    void
    telachatInit();

    void
    startServerProcess( bool = false);

    Tabuleiro*
    getTabuleiroById( quint16 );

    bool
    parseCommandChat( QString _command );

    void
    initTrilhaSonora();
};

#endif // TESTE_H
