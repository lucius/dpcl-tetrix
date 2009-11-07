#include "cliente.h"

//#include <MediaObject>
//#include <MediaSource>
//#include <AudioOutput>

#include <QMessageBox>
#include <QDebug>
#include <QResource>

#include <cstdlib>

Cliente::Cliente(QWidget* _parent) : QWidget(_parent)
{
    this->setFixedHeight(720);

    this->game_over = false;
    this->game_started = false;
    this->server_process = NULL;
    this->server_bin = new QFileInfo("../server/server");

    QHBoxLayout*
    layout_tela_principal = new QHBoxLayout(this);

    layout_tela_principal->setDirection(QHBoxLayout::LeftToRight);

    this->tabuleiro_principal = new Tabuleiro("boga", this);
    this->rede = new Rede_Cliente::Tradutor();
    this->layout()->addWidget(this->tabuleiro_principal);

    this->telaloginInit();
    this->telachatInit();

    QObject::connect(this->tabuleiro_principal,SIGNAL(gameover(int)),
                     this, SLOT(gameOver(int)));

    QObject::connect(this->tabuleiro_principal,SIGNAL(aziasMudou(int)),
                     this, SLOT(aziasMudou(int)));

    QObject::connect(this->tabuleiro_principal, SIGNAL(encaixe()),
                     this->rede,SLOT(encaixe()));

    QObject::connect(this->rede, SIGNAL(startjogo(quint16,quint16)),
                     this, SLOT(startJogo()));

    QObject::connect(this->rede, SIGNAL(over(quint16,quint16)),
                     this, SLOT(jogadorOver(quint16,quint16)));

    this->connectsIncommingDataFromNet();

    this->aziaMaster = new AziaMaster(this);


}

Cliente::~Cliente()
{
    delete this->janela_telachat;
    delete this->janela_telalogin;

    if ( this->server_process != NULL )
    {
        this->server_process->kill();
        this->server_process->waitForFinished();
    }
}

void
Cliente::modoJogoCliente( QString _nome, QString _ip_servidor )
{
    this->tabuleiro_principal->setNomeJogador(_nome);
    this->rede->conectar( _ip_servidor, 6969, _nome);
}

void
Cliente::modoJogoServer( QString _nome, quint8 _n_jogadores )
{
    this->tabuleiro_principal->setNomeJogador(_nome);
    this->startServerProcess( _n_jogadores );
    this->rede->conectar("localhost",6969,_nome);
}

void
Cliente::modoJogoSingle( QString _nome )
{
    this->tabuleiro_principal->setNomeJogador(_nome);
    this->startServerProcess( 1 );
    this->rede->conectar("localhost",6969,_nome);
}

void
Cliente::redeErroConexao( QString _erro )
{
    this->hide();
    QMessageBox::warning(this,"Erro de conexao",_erro);

    this->janela_telalogin->show();

    this->tabuleiro_principal->clear();
    this->outros_tabuleiros.clear();

    this->game_over = false;
    this->game_started = false;
}

void
Cliente::gameOver( int _pontuacao )
{
    qDebug() << "game over";

    this->game_over = true;

    this->rede->gameOver(_pontuacao);

    //display o game over!
}

void
Cliente::redeConectado(quint16 _id)
{
    this->meu_id = _id;
}

void
Cliente::novoJogadorConectado( quint16 _id, QString _nome )
{
    if ( _id != this->meu_id)
    {
        Tabuleiro*
        novo_jogador_tabuleiro = new Tabuleiro( _nome , this);

        this->layout()->addWidget(novo_jogador_tabuleiro);
        this->outros_tabuleiros[_id] = novo_jogador_tabuleiro;

        QObject::connect(this->rede,SIGNAL(startjogo(quint16,quint16)),
                         novo_jogador_tabuleiro,SLOT(startjogo(quint16,quint16)));

        QSignalMapper *signalMapper = new QSignalMapper(novo_jogador_tabuleiro);

        QObject::connect(novo_jogador_tabuleiro,SIGNAL(aziaEmAlguem()),
                     signalMapper, SLOT(map()));

        signalMapper->setMapping(novo_jogador_tabuleiro, _id);

        QObject::connect(signalMapper,SIGNAL(mapped(int)),this,SLOT(daAzia(int)));




        novo_jogador_tabuleiro->show();
    }
}

void
Cliente::incomingRotacionapeca(quint16 _id)
{
    this->getTabuleiroById(_id)->rotacionapeca();
}

void
Cliente::incomingNovaPeca(quint16 _id,quint16 _peca)
{
    if ( _id != this->meu_id )
    {
        this->outros_tabuleiros[_id]->desceTudo();
    }
    else
    {
        this->tabuleiro_principal->desceTudo();
    }

    this->getTabuleiroById(_id)->novapeca( (qint8) _peca );
}

void
Cliente::incomingDesceUmPouco( quint16 _id )
{
    this->getTabuleiroById(_id)->desce();
}

void
Cliente::incomingMovimentaEsquerda( quint16 _id )
{
    this->getTabuleiroById(_id)->movepeca(false);
}

void
Cliente::incomingMovimentaDireita( quint16 _id )
{
    this->getTabuleiroById(_id)->movepeca(true);
}
void
Cliente::incomingAziaFrenetica(quint16 _id, quint16 _id_origem ){
    qDebug() << "Incoming azia to "<< _id << " by" << _id_origem;

    this->aziaMaster->novaAzia(_id);

    this->getTabuleiroById(_id_origem)->setAzias(this->getTabuleiroById(_id_origem)->getAzias()-1);
}
void
Cliente::incomingPontuacao(quint16 _id, int _pontuacao) {
    this->getTabuleiroById(_id)->setPontuacao(_pontuacao);
}

void
Cliente::startJogo()
{
    this->game_started = true;

    this->initTrilhaSonora();
}

void
Cliente::stdOutServer()
{
//  todo!!!
}

void
Cliente::jogadorDown( quint16 _id )
{
    Tabuleiro*
    jogador_caido = this->outros_tabuleiros[_id];

    QString
    nome_jogador = jogador_caido->getNomeJogador();

    QString
    mensagem = nome_jogador + " saiu do jogo!";

    this->janela_telachat->incommingMensagem("Server",mensagem);

    delete jogador_caido;

    this->outros_tabuleiros.remove(_id);
}

void
Cliente::jogadorOver( quint16 _id_jogador , quint16 _pontuacao)
{
    qDebug() << "Cliente::jogadorOver";

    QString
    nome_jogador;

    QString
    mensagem;

    if ( this->meu_id != _id_jogador )
    {
        nome_jogador = this->outros_tabuleiros[_id_jogador]->getNomeJogador();
    }
    else
    {
        nome_jogador = this->tabuleiro_principal->getNomeJogador();
    }

    mensagem = nome_jogador + " tomou um game over com " +
               QString::number(_pontuacao);

    this->janela_telachat->incommingMensagem("Server", mensagem);
}

void
Cliente::keyPressEvent(QKeyEvent* _key)
{
    const int
    key_enter = 16777220;    

    switch(_key->key())
    {
        case Qt::Key_Up:
            this->rede->rotacao();
            break;
        case Qt::Key_Down:
            this->rede->desce();
            break;
        case Qt::Key_Left:
            this->rede->movimentaEsquerda();
            break;
        case Qt::Key_Right:
            this->rede->movimentaDireita();
            break;
        case Qt::Key_Space:
            this->rede->encaixe();
            break;
        case key_enter:
        case Qt::Key_Enter:
            this->janela_telachat->show();
            break;
        case Qt::Key_P:
            this->rede->start(true);
    }
}

void
Cliente::pecaRotaciona()
{
    this->rede->rotacao();
}

void
Cliente::pecaDesce()
{
    this->rede->desce();
}

void
Cliente::pecaMovimentaEsquerda()
{
    this->rede->movimentaEsquerda();
}

void
Cliente::pecaMovimentaDireita()
{
    this->rede->movimentaDireita();
}

void
Cliente::pecaEncaixa()
{
    this->rede->encaixe();
}

void
Cliente::connectsIncommingDataFromNet()
{
    QObject::connect(this->rede,SIGNAL(conexaoEstabelecida()),
                     this, SLOT(show()));

    QObject::connect(this->rede,SIGNAL(erroConexao(QString)),
                     this, SLOT(redeErroConexao(QString)));

    QObject::connect(this->rede,SIGNAL(id(quint16)),
                     this,SLOT(redeConectado(quint16)));

    QObject::connect(this->rede,SIGNAL(novoJogador(quint16,QString)),
                     this,SLOT(novoJogadorConectado(quint16,QString)));

    QObject::connect(this->rede,SIGNAL(down(quint16)),
                     this,SLOT(jogadorDown(quint16)));

    QObject::connect(this->rede,SIGNAL(startjogo(quint16,quint16)),
                     this->tabuleiro_principal,SLOT(startjogo(quint16,quint16)));

    QObject::connect(this->rede, SIGNAL(rotacionaPeca(quint16)),
                     this,SLOT(incomingRotacionapeca(quint16)));

    QObject::connect(this->rede,SIGNAL(novapeca(quint16,quint16)),
                     this,SLOT(incomingNovaPeca(quint16,quint16)));

    QObject::connect(this->rede,SIGNAL(descePeca(quint16)),
                     this,SLOT(incomingDesceUmPouco(quint16)));

    QObject::connect(this->rede,SIGNAL(movePecaDireita(quint16)),
                     this,SLOT(incomingMovimentaDireita(quint16)));

    QObject::connect(this->rede,SIGNAL(movePecaEsquerda(quint16)),
                     this,SLOT(incomingMovimentaEsquerda(quint16)));

    QObject::connect(this->rede,SIGNAL(daAzia(quint16, quint16)),this,SLOT(incomingAziaFrenetica(quint16, quint16)));

}

void
Cliente::telaloginInit()
{
    this->janela_telalogin = new JanelaTelaLogin();
    this->janela_telalogin->show();

    QObject::connect(this->janela_telalogin,SIGNAL(cliente(QString, QString)),
                     this,SLOT(modoJogoCliente(QString,QString)));

    QObject::connect(this->janela_telalogin,SIGNAL(cliente(QString,QString)),
                     this->janela_telalogin,SLOT(hide()));


    QObject::connect(this->janela_telalogin,SIGNAL(servir(QString,quint8)),
                     this,SLOT(modoJogoServer(QString,quint8)));

    QObject::connect(this->janela_telalogin,SIGNAL(servir(QString,quint8)),
                     this->janela_telalogin,SLOT(hide()));


    QObject::connect(this->janela_telalogin,SIGNAL(single(QString)),
                     this,SLOT(modoJogoSingle(QString)));

    QObject::connect(this->janela_telalogin,SIGNAL(single(QString)),
                     this->janela_telalogin,SLOT(hide()));
}

void
Cliente::telachatInit()
{
    this->janela_telachat = new JanelaTelaChat();

    QObject::connect(this->janela_telachat, SIGNAL(outMensagemChat(QString)),
                     this->rede, SLOT(chat(QString)));

    QObject::connect(this->rede,SIGNAL(chat(QString,QString)),
               this->janela_telachat,SLOT(incommingMensagem(QString,QString)));

    //QObject::connect(this->janela_telachat, SIGNAL(commPronto(bool)), this->rede,SLOT(start(bool)));

    QObject::connect(this->janela_telachat, SIGNAL(commPronto(bool)), this,SLOT(jogadorPronto(bool)));
}


void Cliente::daAzia(int _id) {
    this->aziasMudou(this->tabuleiro_principal->getAzias());
    this->rede->azia(_id);
}
void Cliente::jogadorPronto(bool pronto) {
    QString _mensagem;

    _mensagem = (pronto ? "Esta pronto" : "Não esta pronto");
    _mensagem =  _mensagem + " para jogar. Aguardando próximos jogadores";

    this->rede->chat(_mensagem);
    this->rede->start(pronto);
}



void Cliente::startServerProcess( quint8 _n_jogadores )
{
    QFileInfo server("../server/server");


    if (! server.exists() )
    {
        QMessageBox::critical(this,"Deu merda","Executavel servidor não encontrado");
    }

    QString
    n_jogadores = QString::number( _n_jogadores );

    this->server_process = new QProcess();

    this->server_process->start(this->server_bin->absoluteFilePath() +
                                " " + n_jogadores);

    QObject::connect(this->server_process, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(stdOutServer()));

}

Tabuleiro*
Cliente::getTabuleiroById( quint16 _id)
{
    if (_id == this->meu_id )
    {
        return this->tabuleiro_principal;
    }
    else
    {
        return this->outros_tabuleiros[_id];
    }
}
void
Cliente::daAziaFrenetica(QString q, QString t) {
    qDebug() << "DA UMA AZIA FRENETICA EM" << q << " de " <<t;
}
void
Cliente::initTrilhaSonora()
{
/*    QFileInfo *musica = new QFileInfo("musicas/ken.mp3");

    Phonon::MediaObject *media = new Phonon::MediaObject(this);
    media->setCurrentSource(Phonon::MediaSource(musica->absoluteFilePath()));
    Phonon::AudioOutput *audioOutput =
     new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::Path path = Phonon::createPath(media, audioOutput);

    media->play();

    QObject::connect(media,SIGNAL(finished()), media, SLOT(play())); */
}

void
Cliente::aziasMudou(int _n) {
    qDebug() << "as azias mudaram mano" << _n;
    foreach(int i, this->outros_tabuleiros.keys()) {
        this->outros_tabuleiros.value(i)->habilitaAzia((_n>=1));
    }
}
