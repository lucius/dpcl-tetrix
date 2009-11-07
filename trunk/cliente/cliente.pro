QT += network

# phonon
# phonon
# INCLUDEPATH += /usr/include/KDE/Phonon/
SOURCES += main.cpp \
    cliente.cpp \
    ../tabuleiro/mapapecas.cpp \
    ../tabuleiro/pecinha.cpp \
    ../tabuleiro/pivo.cpp \
    ../tabuleiro/tabuleiro.cpp \
    ../rede_cliente/tradutor.cpp \
    ../rede_cliente/conexao.cpp \
    janelatelachat.cpp \
    janelatelalogin.cpp \
    ../Azias/AziaMaster.cpp \
    ../Azias/Azia.cpp \
    ../Azias/AziaRodaPeca.cpp \
    ../Azias/AziaPontuacao.cpp \
    commandparser.cpp
    ../Azias/AziaEscondeTabuleiro.cpp \
    commandparser.cpp 


HEADERS += cliente.h \
    ../tabuleiro/basicos.h \
    ../tabuleiro/mapapecas.h \
    ../tabuleiro/pecinha.h \
    ../tabuleiro/pivo.h \
    ../tabuleiro/tabuleiro.h \
    ../rede_cliente/tradutor.h \
    ../rede_cliente/conexao.h \
    janelatelachat.h \
    janelatelalogin.h \
    commandparser.h \
    ../Azias/AziaMaster.h \
    ../Azias/Azia.h \
    ../Azias/AziaRodaPeca.h \
    ../Azias/AziaPontuacao.h \
    ../Azias/AziaEscondeTabuleiro.h
FORMS += tela_chat.ui \
    tela_login.ui \
    ../tabuleiro/tabuleiro.ui
RESOURCES += images.qrc
