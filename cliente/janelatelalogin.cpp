#include "janelatelalogin.h"

#include <QDebug>
#include <QMessageBox>

JanelaTelaLogin::JanelaTelaLogin( QWidget* _parent ) : QWidget(_parent)
{
    this->conteudo = new Ui::Conectar();

    this->conteudo->setupUi(this);

    this->conteudo->server_additional_options->hide();
    //this->conteudo->client_additional_options->hide();


    QObject::connect(this->conteudo->btn_ok, SIGNAL(clicked()),
                     this,SLOT(btnOkClick()));
    QObject::connect(this->conteudo->radio_cliente, SIGNAL(clicked()),this,SLOT(radioGameTypeClick()));
    QObject::connect(this->conteudo->radio_servir, SIGNAL(clicked()),this,SLOT(radioGameTypeClick()));
    QObject::connect(this->conteudo->radio_single, SIGNAL(clicked()),this,SLOT(radioGameTypeClick()));

}

JanelaTelaLogin::~JanelaTelaLogin()
{
    delete this->conteudo;
}
void JanelaTelaLogin::radioGameTypeClick() {
    if (this->conteudo->radio_cliente->isChecked() )
    {
        this->conteudo->server_additional_options->hide();
        this->conteudo->client_additional_options->show();        
    }
    else if ( this->conteudo->radio_servir->isChecked() )
    {     
        this->conteudo->server_additional_options->show();
        this->conteudo->client_additional_options->hide();
    }
    else if ( this->conteudo->radio_single->isChecked() )
    {        
        this->conteudo->server_additional_options->hide();
        this->conteudo->client_additional_options->hide();
    }
}

void
JanelaTelaLogin::btnOkClick()
{
    this->nome = this->conteudo->edit_nome->text();

    if(this->nome.length() == 0) {
        QMessageBox::warning(this,
                             "Erro",
                             "Informe o seu nome de jogador!");
        return;
    }

    if (this->conteudo->radio_cliente->isChecked() )
    {
        this->clienteSlot();
    }
    else if ( this->conteudo->radio_servir->isChecked() )
    {
       this->servirSlot();
    }
    else if ( this->conteudo->radio_single->isChecked() )
    {
       emit this->single( this->nome );
    }
    else
    {
        QMessageBox::warning(this,
                             "Erro",
                             "Você não preencheu os campos corretamente!");
    }
}

void
JanelaTelaLogin::keyPressEvent( QKeyEvent* _event )
{
    const int
    key_enter = 16777220;

    switch ( _event->key() )
    {
        case Qt::Key_Escape:
            this->close();
            break;
        case key_enter:
            this->btnOkClick();
            break;
    }
}

void
JanelaTelaLogin::clienteSlot()
{
    if(this->conteudo->edit_ip_servidor->text().length()==0) {
        QMessageBox::warning(this,
                             "Erro",
                             "Informe o endereço de IP do servidor!");
        return;
    }
    emit this->cliente( this->nome, this->conteudo->edit_ip_servidor->text() );
}

void
JanelaTelaLogin::servirSlot()
{
    if(this->conteudo->edit_n_clientes->text().toInt()<=0) {
        QMessageBox::warning(this,
                             "Erro",
                             "Informe corretamente o número de jogadores!");
        return;
    }
    emit this->servir( this->nome,
                       this->conteudo->edit_n_clientes->text().toInt() );
}
