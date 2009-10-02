#ifndef QAZIAOBJECT_H
#define QAZIAOBJECT_H

#include <QObject>

enum QEAzia
{
    VIDEO,
    CUSTOM,
    RANDOM
};

class QAziaObject : public QObject
{
    Q_OBJECT
public:
    QAziaObject( QEAzia = RANDOM, QObject* = 0 );

    virtual
    ~QAziaObject();

public slots:

    virtual void
    startAzia();

signals:

    void
    finished();
};

#endif // QAZIAOBJECT_H
