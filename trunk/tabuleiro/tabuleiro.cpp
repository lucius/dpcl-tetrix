#include <QDebug>
#include <QTime>

#include "tabuleiro.h"
#include "ui_tabuleiro.h"


Tabuleiro::Tabuleiro(QString nome, QWidget *parent)
    : QWidget(parent), ui(new Ui::Tabuleiro)
{

    this->posso_mexer = false;

    this->setFixedWidth(230);
    ui->setupUi(this);

    this->ui->label_nome_jogador->setText(nome);


    this->timer = new QTimer(this);
    this->numeroPecasUtilizadas = 0;
    this->pontuacao = 0;
    this->azias = 1;
    this->level = 1;


    this->btn_azia = this->ui->btn_azia;
    this->btn_azia->hide();

    connect( this, SIGNAL(linhaCheia(Tab::XyView)),
             this, SLOT(apagaLinhaCheia(Tab::XyView)) );

    connect( this, SIGNAL(procuraLinhas()),
             this, SLOT(procuraLinhasCheias()) );

    connect( this, SIGNAL(pontuacaoMudou()),
             this, SLOT(setPontuacao()) );

    connect(this, SIGNAL(aziasMudou(int)),
            this, SLOT(setAzias(int)));

    connect(this->ui->btn_azia,SIGNAL(clicked()),this,SLOT(daAziaEmAlguem()));

    emit this->aziasMudou( this->azias );
    emit this->pontuacaoMudou( );

}

Tabuleiro::~Tabuleiro()
{
    delete ui;
}

void
Tabuleiro::setNomeJogador( QString _nome)
{
    this->ui->label_nome_jogador->setText(_nome);
}

QString
Tabuleiro::getNomeJogador( )
{
    return this->ui->label_nome_jogador->text();
}

void
Tabuleiro::procuraLinhasCheias( )
{
    QWidget*
    objeto;

    unsigned short int
    elementosNaLinha;

    Tab::XyView
    posicaoAVerificar;

    posicaoAVerificar.setY( (Tab::P_SIZE.height()*24) );

    while( posicaoAVerificar.y() >= 0 )
    {
        elementosNaLinha = 0;
        posicaoAVerificar.setX( 0 );

        while( posicaoAVerificar.x() <= (Tab::P_SIZE.width()*9) )
        {
            objeto = this->ui->piecesContainer->childAt( posicaoAVerificar );

            if( objeto != 0 )
            {
                ++elementosNaLinha;
            }
            posicaoAVerificar.rx() += Tab::P_SIZE.width();
        }

        if(elementosNaLinha == 10)
        {
            emit this->apagaLinhaCheia( posicaoAVerificar );
            posicaoAVerificar.setY( (Tab::P_SIZE.height()*24) );
        }
        else
        {
            posicaoAVerificar.ry() -= Tab::P_SIZE.height();
        }
    }
}

void
Tabuleiro::apagaLinhaCheia( Tab::XyView _posicaoAApagar )
{
    QWidget*
    objeto;

    _posicaoAApagar.setX( 0 );

    while( _posicaoAApagar.x() <= (Tab::P_SIZE.width()*9) )
    {
        objeto = this->ui->piecesContainer->childAt( _posicaoAApagar );

        delete objeto;

        _posicaoAApagar.rx() += Tab::P_SIZE.width();
    }

    this->pontuacao += 10;

    if(this->pontuacao % 20 == 0) {
        ++this->azias;
        emit this->aziasMudou(this->azias);
    }

    emit pontuacaoMudou();

    this->desceLinhas( _posicaoAApagar );
}

/** void Tabuleiro::desceLinhas( Tab::XyView _posicao)
 *
 * Desce as linhas acima de uma linha apontada pela posicao _posicao
 */
void
Tabuleiro::desceLinhas( Tab::XyView _posicao )
{
    QWidget*
    objeto;

    Tab::XyView
    novaPosicao;

    while( _posicao.y() >= 0 )
    {
        _posicao.setX( 0 );

        while( _posicao.x() <= (Tab::P_SIZE.width()*9) )
        {
            objeto = this->ui->piecesContainer->childAt( _posicao );

            if( objeto != 0 )
            {
                novaPosicao = _posicao;
                novaPosicao.ry() += Tab::P_SIZE.height();

                objeto->move( novaPosicao );
            }

            _posicao.rx() += Tab::P_SIZE.width();
        }

        _posicao.ry() -= Tab::P_SIZE.height();
    }
}

void
Tabuleiro::setAzias(int _n)
{    
    this->azias = _n;
    this->ui->level->display((int) _n );
}

void
Tabuleiro::desce()
{
    if (this->posso_mexer)
        this->currentPiece->desce();
}

void
Tabuleiro::setPontuacao( )
{
    this->ui->pontuacao->display( (int) this->pontuacao );
}



void
Tabuleiro::rotacionapeca( )
{
    if ( this->posso_mexer )
        this->currentPiece->rotaciona();
}

void
Tabuleiro::colidiu( )
{
    if ( this->posso_mexer )
    {
        this->posso_mexer = false;

        this->timer->disconnect( this->currentPiece, SLOT(desce()) );

        ++ numeroPecasUtilizadas;

        if ( this->numeroPecasUtilizadas % 2 == 0 )
        {

            emit this->aziasMudou(this->azias);
        }

        emit this->encaixe();
        emit this->procuraLinhas( );
    }
}

void
Tabuleiro::movepeca( bool _direcao )
{
    if ( this->posso_mexer )
    {
        if( _direcao )
        {
            this->currentPiece->moveDireita();
        }
        else
        {
            this->currentPiece->moveEsquerda();
        }
    }
}

void
Tabuleiro::startjogo( quint16 descendo, quint16 proxima )
{
    Tab::XyView
    pos(60,60);

    Tab::XyView
    pos1(30,30);

    this->currentPiece = new Tab::Pivo( descendo, QColor(0,0,0), pos, this->ui->piecesContainer );
    this->previewPiece = new Tab::Pivo( proxima, QColor(0,0,0), pos1, this->ui->piecePreview );

    connect( this->timer, SIGNAL(timeout()), this->currentPiece, SLOT(desce()) );
    connect( this->currentPiece, SIGNAL(colidiu()), this, SLOT(colidiu()) );

    this->timer->start( this->getTimeOut() );

    this->currentPiece->desce();
    this->posso_mexer = true;
}

void
Tabuleiro::novapeca( qint8 nova )
{
    Tab::XyView
    pos(30,30);

    Tab::XyView
    pos1(60,60);

    if ( this->checkGameOver( Tab::MapaPecas::MeDaOMapa( )->ComoEhAPeca( nova ) ) )
    {
        emit this->gameover( this->pontuacao );
    }
    else
    {
        this->currentPiece = new Tab::Pivo( this->previewPiece->getPecaInt(),
                                        QColor(0,0,0),
                                        pos1,
                                        this->ui->piecesContainer );

        delete this->previewPiece;
        this->previewPiece = new Tab::Pivo( nova, QColor(0,0,0), pos, this->ui->piecePreview );
    
        connect(this->timer, SIGNAL(timeout()), this->currentPiece, SLOT(desce()));
        connect(this->currentPiece, SIGNAL(colidiu()), this, SLOT(colidiu()));
    
        this->timer->start( this->getTimeOut() );
        this->posso_mexer = true;
    }
}

unsigned int
Tabuleiro::getTimeOut( )
{
    return ( 2000 / (1+this->level) );
}

void
Tabuleiro::desceTudo()
{
    this->timer->disconnect( this->currentPiece, SLOT(desce()) );
    this->currentPiece->desceTudo();
    this->procuraLinhasCheias();
}

bool
Tabuleiro::checkGameOver( Tab::TipoPeca _novasPosicoes )
{
    Tab::XyView
    _posicaoRealPivo(60,60);

    QWidget*
    objeto;

    Tab::XyView
    posicaoAVerificar;

    Tab::TipoPeca::iterator
    posicao;

    for( posicao = _novasPosicoes.begin(); posicao != _novasPosicoes.end(); ++posicao )
    {
        posicaoAVerificar.setX( (_posicaoRealPivo.x() + (posicao->first*Tab::P_SIZE.width())) );
        posicaoAVerificar.setY( (_posicaoRealPivo.y() + (posicao->second*Tab::P_SIZE.height())) );

        objeto = this->ui->piecesContainer->childAt( posicaoAVerificar );

        if( (objeto != 0) )
        {
            return true;
        }
    }

    return false;
}

void
Tabuleiro::habilitaAzia(bool ok) {
    this->ui->btn_azia->hide();
    this->ui->btn_azia->setDisabled(true);

    if(ok) {
        this->ui->btn_azia->show();
        this->ui->btn_azia->setDisabled(false);
    }
}


void
Tabuleiro::daAziaEmAlguem() {   
    emit this->aziaEmAlguem();
}
int
Tabuleiro::getAzias() {
    return this->azias;
}
void
Tabuleiro::aziado() {
    qDebug() << "kralho mano fui aziado";


    /*
    azia -3 pontos
    this->pontuacao -= 3;
    emit pontuacaoMudou();
    */

    /* azia 2*/

    //connect( this->timer, SIGNAL(timeout()), this->currentPiece, SLOT(rotaciona()));



}
