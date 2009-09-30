#include "janelatelachat.h"

JanelaTelaChat::JanelaTelaChat( QWidget* _parent ) :
        QWidget(_parent)
{
    this->conteudo = new Ui::TelaChat();
    this->conteudo->setupUi(this);

    QObject::connect( this->conteudo->edit_mensagem, SIGNAL(returnPressed()),
                      this, SLOT(submitStr()));
}

JanelaTelaChat::~JanelaTelaChat()
{
    delete this->conteudo;
}

void
JanelaTelaChat::submitStr( )
{
    QString
    str = this->conteudo->edit_mensagem->text();

    this->conteudo->edit_mensagem->clear();

    switch ( this->parser.parse( str ) )
    {
        case CHAT:
            emit this->outMensagemChat( str );
            break;
        case PRONTO:
            emit this->commPronto(true);
            break;
        case N_PRONTO:
            emit this->commPronto(false);
            break;
    }
}

void
JanelaTelaChat::incommingMensagem( QString _nome, QString _msg )
{
    QString
    mensagem = _nome + " -> " + _msg;

    this->conteudo->list_mensagens->addItem(mensagem);
    this->conteudo->list_mensagens->scrollToBottom();
}

void
JanelaTelaChat::keyPressEvent( QKeyEvent* _event )
{


}
