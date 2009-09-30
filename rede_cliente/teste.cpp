#include "teste.h"

#include <QLineEdit>

Teste::Teste()
{
    this->inicio = new Rede_Cliente::Tradutor(this);
    this->pronto = false;
}

void
Teste::keyPressEvent(QKeyEvent *key)
{
    switch(key->key())
    {
        case Qt::Key_C:
            this->inicio->conectar("192.168.1.101", 6969, "Joselito");
            break;
        case Qt::Key_S:
            this->pronto = !this->pronto;
            this->inicio->start(pronto);
            break;
        case Qt::Key_O:
            this->inicio->gameOver(666);
            break;
        case Qt::Key_Up:
            this->inicio->rotacao();
            break;
        case Qt::Key_Left:
            this->inicio->movimentaEsquerda();
            break;
        case Qt::Key_Right:
            this->inicio->movimentaDireita();
            break;
        case Qt::Key_Down:
            this->inicio->desce();
            break;
        case Qt::Key_Space:
            this->inicio->encaixe();
            break;
        case Qt::Key_M:
            this->inicio->chat("Mensagem do chat");
            break;
    }
}
