#ifndef PARSEDDATASTRUCTURE_H
#define PARSEDDATASTRUCTURE_H

#include "Data.h"
#include "dbcdata.h"
#include <QVector>
#include <QString>
#include <QPair>


typedef struct {
    int msgBoxId;
    QString name;
    QPair<QString, int> *pairs;
}enum_t;

typedef struct {
    QString name;
    int startbit;
    int size;
    bool issigned;
    QString minValue;
    QString maxValue;
    QString type;
//    enum_t *enumtype;
    QString sender;
    QString comment;
} sgnl_t;

typedef struct {
    QString ID;
    QString msgBoxName;
    int size;
    QString sender;
//    QString comment;
    QVector<sgnl_t*> *msgSignals;
} msgbox_t;


class DBCParsedData : public AbstractData
{
public:
    QString Reciever;
    QString Transmitter;
    QVector<msgbox_t*> *msgboxes;

public:
    void init()
    {
        msgboxes = new QVector<msgbox_t*>;
    }

    void push_back(msgbox_t *msgBox)
    {
        msgboxes->append(msgBox);
    }
};




#endif // PARSEDDATASTRUCTURE_H
