#ifndef TESTE_H
#define TESTE_H

#include <QWidget>
#include <QLineEdit>

#include "tradutor.h"

class Teste : public QWidget
{
public:
    Teste();

    void
    keyPressEvent(QKeyEvent *);

    Rede_Cliente::Tradutor*
    inicio;

    bool
    pronto;

    QLineEdit*
    nome;
};

#endif // TESTE_H
