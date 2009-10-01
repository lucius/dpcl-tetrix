#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>

enum Command
{
    CHAT,
    PRONTO,
    N_PRONTO,
    AZIA_FRENETICA
};

class CommandParser : public QObject
{
public:
    CommandParser();

    ~CommandParser();

    Command
    parse( QString _str );
};

#endif // COMMANDPARSER_H
