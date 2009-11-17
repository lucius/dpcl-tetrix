#include "parser.h"

#include <QStringList>

Parser::Message
Parser::parseMessage( QString _message )
{
    QString
    comando_str = _message.toLower().split(":")[0];

    if ( comando_str == "butt" )
    {
        return BUTT;
    }
    else if ( comando_str == "enca" )
    {
        return ENCA;
    }
    else if ( comando_str == "down" )
    {
        return DOWN;
    }
    else if ( comando_str == "chat" )
    {
        return CHAT;
    }
    else if ( comando_str == "init" )
    {
        return INIT;
    }
    else if ( comando_str == "star" )
    {
        return STAR;
    }
    else if ( comando_str == "azia" )
    {
        return AZIA;
    }
    else if ( comando_str == "over" )
    {
        return OVER;
    }
    else if ( comando_str == "novo" )
    {
        return NOVO;
    }
    else if ( comando_str == "rank" )
    {
        return RANK;
    }
    else
    {
        return ERRO;
    }
}
