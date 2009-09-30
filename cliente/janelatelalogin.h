#ifndef JANELATELALOGIN_H
#define JANELATELALOGIN_H

#include <QWidget>
#include <QKeyEvent>

#include "ui_tela_login.h"

class JanelaTelaLogin : public QWidget
{
    Q_OBJECT
public:
    JanelaTelaLogin( QWidget* = 0 );
    ~JanelaTelaLogin();

signals:

    void
    cliente( QString _nome, QString _ip_servidor );

    void
    servir( QString _nome, quint8 _n_jogadores );

    void
    single( QString _nome );

protected slots:

    void
    btnOkClick();

private:

    QString
    nome;

    Ui::Conectar*
    conteudo;

    void
    keyPressEvent( QKeyEvent* );

    void
    servirSlot();

    void
    clienteSlot();

};

#endif // JANELATELALOGIN_H
