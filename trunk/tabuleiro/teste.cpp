#include "teste.h"

#include <MediaObject>
#include <MediaSource>
#include <AudioOutput>

#include <QMessageBox>
#include <QDebug>
#include <QResource>

Teste::Teste(QWidget* _parent) : QWidget::QWidget(_parent)
{
    this->setFixedHeight(720);

    this->game_over = false;
    this->game_started = false;
    this->server_process = NULL;
    this->single_player = false;
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

    QObject::connect(this->tabuleiro_principal, SIGNAL(encaixe()),
                     this->rede,SLOT(encaixe()));

    QObject::connect(this->rede, SIGNAL(startjogo(quint16,quint16)),
                     this, SLOT(startJogo()));
    
    this->connectsIncommingDataFromNet();
}

Teste::~Teste()
{
    delete this->telachat;
    delete this->telalogin;

    if ( this->server_process != NULL )
    {
        this->server_process->kill();
        this->server_process->waitForFinished();
    }
}

void
Teste::telaloginPedidoSlot()
{
    QString nome_jogador = this->telalogin->edit_nome->text();

    this->tabuleiro_principal->setNomeJogador( nome_jogador);

    if ( this->telalogin->radio_cliente->isChecked() ||
         this->telalogin->radio_servir->isChecked() ||
         this->telalogin->radio_single->isChecked() )
    {
        QString
        ip_server;

        if ( this->telalogin->radio_cliente->isChecked() )
        {
            ip_server = this->telalogin->edit_ip_servidor->text();
        }
        else if ( this->telalogin->radio_servir->isChecked() )
        {
            this->startServerProcess( );
            ip_server = "localhost";
        }
        else if ( this->telalogin->radio_single->isChecked() )
        {
            this->single_player = true;
            this->startServerProcess( true );
            ip_server = "localhost";
        }

        this->rede->conectar( ip_server, 6969, nome_jogador );

        this->janela_telalogin->hide();
    }
    else
    {
        QMessageBox::warning(this,"Diabo!!!",
                             "Você precisa escolher alguma opcao de modo de jogo");
    }

}

void
Teste::redeErroConexao( QString _erro )
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
Teste::gameOver( int _pontuacao )
{
    qDebug() << "game over";

    this->game_over = true;

    QMessageBox::information(this,"GAME OVER!","PERDEU PLAYBOY");
}

void
Teste::redeConectado(quint16 _id)
{
    this->meu_id = _id;
}

void
Teste::novoJogadorConectado( quint16 _id, QString _nome )
{
    if ( _id != this->meu_id)
    {
        Tabuleiro*
        novo_jogador_tabuleiro = new Tabuleiro( _nome , this);

        this->layout()->addWidget(novo_jogador_tabuleiro);
        this->outros_tabuleiros[_id] = novo_jogador_tabuleiro;

        QObject::connect(this->rede,SIGNAL(startjogo(quint16,quint16)),
                         novo_jogador_tabuleiro,SLOT(startjogo(quint16,quint16)));

        novo_jogador_tabuleiro->show();
    }
}

void
Teste::enviaChat()
{
    QString
    mensagem = this->telachat->edit_mensagem->text();

    this->telachat->edit_mensagem->clear();

    if (! this->parseCommandChat( mensagem ) )
    {
        if (! mensagem.isEmpty() )
            this->rede->chat(mensagem);
    }

    if ( this->game_started )
        this->janela_telachat->hide();
}

void
Teste::recebeChat( QString nome, QString _mensagem )
{
    QString
    nova_mensagem = nome + " -> " + _mensagem;

    this->telachat->list_mensagens->addItem(nova_mensagem);
}

void
Teste::incomingRotacionapeca(quint16 _id)
{
    this->getTabuleiroById(_id)->rotacionapeca();
}

void
Teste::incomingNovaPeca(quint16 _id,quint16 _peca)
{
    if ( _id != this->meu_id )
    {
        this->outros_tabuleiros[_id]->desceTudo();
    }

    this->getTabuleiroById(_id)->novapeca( (qint8) _peca );
}

void
Teste::incomingDesceUmPouco( quint16 _id )
{
    this->getTabuleiroById(_id)->desce();
}

void
Teste::incomingMovimentaEsquerda( quint16 _id )
{
    this->getTabuleiroById(_id)->movepeca(false);
}

void
Teste::incomingMovimentaDireita( quint16 _id )
{
    this->getTabuleiroById(_id)->movepeca(true);
}

void
Teste::startJogo()
{
    this->game_started = true;

    this->janela_telachat->hide();

    this->initTrilhaSonora();
}

void
Teste::stdOutServer()
{
    const QByteArray
    stdOut = this->server_process->readLine();

    QString
    linha;

    QDataStream
    stream_out( stdOut );

    stream_out >> linha;

    qDebug() << linha;
}

void
Teste::keyPressEvent(QKeyEvent* _key)
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
            this->tabuleiro_principal->desceTudo();
            this->rede->encaixe();
            break;
        case key_enter:
            this->janela_telachat->show();
            break;
    }
}

void
Teste::connectsIncommingDataFromNet()
{
    QObject::connect(this->rede,SIGNAL(conexaoEstabelecida()),
                     this, SLOT(show()));

    QObject::connect(this->rede,SIGNAL(erroConexao(QString)),
                     this, SLOT(redeErroConexao(QString)));

    QObject::connect(this->rede,SIGNAL(id(quint16)),
                     this,SLOT(redeConectado(quint16)));

    QObject::connect(this->rede,SIGNAL(novoJogador(quint16,QString)),
                     this,SLOT(novoJogadorConectado(quint16,QString)));

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

}

void
Teste::telaloginInit()
{
    this->janela_telalogin = new QWidget();
    this->telalogin = new Ui::Conectar();
    this->telalogin->setupUi(this->janela_telalogin);
    this->janela_telalogin->show();

    QObject::connect(this->telalogin->btn_ok,SIGNAL(clicked()),
                     this,SLOT(telaloginPedidoSlot()));
}

void
Teste::telachatInit()
{
    this->janela_telachat = new QWidget();
    this->telachat = new Ui::TelaChat();
    this->telachat->setupUi(this->janela_telachat);

    QObject::connect(this->rede,SIGNAL(chat(QString,QString)),
                     this,SLOT(recebeChat(QString,QString)));

    QObject::connect(this->telachat->btn_envia,SIGNAL(clicked()),
                     this,SLOT(enviaChat()));

    QObject::connect(this->telachat->edit_mensagem, SIGNAL(returnPressed()),
                     this,SLOT(enviaChat()));

    QObject::connect(this->telachat->btn_cancela,SIGNAL(clicked()),
                     this->janela_telachat,SLOT(hide()));
}

void
Teste::startServerProcess( bool is_single )
{
    QString
    n_jogadores;

    if ( this->telalogin->radio_single->isChecked() )
    {
        n_jogadores = " 1";
    }
    else
    {
        n_jogadores = this->telalogin->edit_n_clientes->text();
    }

    this->server_process = new QProcess();

    this->server_process->start(this->server_bin->absoluteFilePath() +
                                " " + n_jogadores);

    QObject::connect(this->server_process, SIGNAL(readyReadStandardOutput()),
                 this, SLOT(stdOutServer()));

    this->server_process->waitForReadyRead(250);
}

Tabuleiro*
Teste::getTabuleiroById( quint16 _id)
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

bool
Teste::parseCommandChat( QString _command )
{
    if ( _command.at(0) == QChar('/') )
    {
        if (_command == "/pronto")
        {
            this->rede->start(true);
        }
        else if (_command == "/!pronto")
        {
            this->rede->start(false);
        }

        return true;
    }
    return false;
}

void
Teste::initTrilhaSonora()
{
    QFileInfo *musica = new QFileInfo("ken.mp3");
    
    Phonon::MediaObject *media = new Phonon::MediaObject(this);
    media->setCurrentSource(Phonon::MediaSource(musica->absoluteFilePath()));
    Phonon::AudioOutput *audioOutput =
     new Phonon::AudioOutput(Phonon::MusicCategory, this);
    Phonon::Path path = Phonon::createPath(media, audioOutput);

    media->play();

    QObject::connect(media,SIGNAL(finished()), media, SLOT(play()));
}
