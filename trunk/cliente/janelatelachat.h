#ifndef JANELATELACHAT_H
#define JANELATELACHAT_H

#include <QWidget>

#include "ui_tela_chat.h"
#include "commandparser.h"

class JanelaTelaChat : public QWidget
{
    Q_OBJECT
public:
    JanelaTelaChat( QWidget* _parent = 0 );

    ~JanelaTelaChat();

signals:

    void
    outMensagemChat( QString _mensagem );

    void
    commPronto( bool );

    void
    aziaFrenetica();

public slots:

    void
    submitStr( );

    void
    incommingMensagem( QString _nome, QString _msg );    

private:

    Ui::TelaChat*
    conteudo;

    CommandParser
    parser;

    void
    keyPressEvent( QKeyEvent* _event );
};

#endif // JANELATELACHAT_H
