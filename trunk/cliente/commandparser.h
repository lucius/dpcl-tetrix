#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QObject>

enum Command
{
    CHAT,
    PRONTO,
    N_PRONTO
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
