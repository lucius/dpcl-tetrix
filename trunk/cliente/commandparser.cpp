#include "commandparser.h"

CommandParser::CommandParser()
{
}

CommandParser::~CommandParser()
{

}

Command
CommandParser::parse( QString _str )
{
    if ( _str == "/pronto" )
    {
        return PRONTO;
    }
    else if ( _str == "/!pronto" )
    {
        return N_PRONTO;
    }
    else if( _str == "/azia_frenetica" ) {
        return AZIA_FRENETICA;
    }
    return CHAT;
}
