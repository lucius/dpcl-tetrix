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
    commandparser.cpp \
    ../Azias/AziaRodaPeca.cpp \
    ../Azias/AziaMaster.cpp \
    ../Azias/Azia.cpp
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
    ../Azias/AziaRodaPeca.h \
    ../Azias/AziaMaster.h \
    ../Azias/Azia.h
FORMS += tela_chat.ui \
    tela_login.ui \
    ../tabuleiro/tabuleiro.ui
