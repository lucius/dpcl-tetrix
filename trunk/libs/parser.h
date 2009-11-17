#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QString>

class Parser : public QObject
{
public:

    enum Message
    {
        INIT,
        NOVO,
        STAR,
        CHAT,
        BUTT,
        ENCA,
        AZIA,
        OVER,
        DOWN,
        RANK,
        ERRO
    };

    static Message
    parseMessage( QString _message );

};

#endif // PARSER_H
