#ifndef PARSEDDATASTRUCTURE_H
#define PARSEDDATASTRUCTURE_H

#include "Data.h"
#include "dbcdata.h"
#include <QStringList>
#include <QPair>
#include <QVector>
#include <QString>
#include <QMap>
#include <math.h>

typedef struct {
    QString name;
    QMap<int, QString> values;

    void set_name(QString sgnl_name){
        name = sgnl_name;
        if (sgnl_name.size() >= 6 &&
                sgnl_name.right(6).toLower() != "_state"){
             name += "_state";
        }
    }

    bool compare(QMap<int, QString> r_values){
        return values == r_values;
    }
}enum_t;


typedef struct {
    QString name;
    int startbit;
    int size;
    bool issigned;
    QString start_value;
    QPair <QString, QString> values;
    enum_t type_values;
    QString sender;
    QString comment;

    void set_type(){
        if (size == 2){
            type_values.name = "bool";
            start_value = "false";
        }
        else if (size % 8 == 0){
            type_values.name = "int" + QString::number(size) + "_t";

            if (issigned == false)
                type_values.name.prepend("u");

            start_value = "0";
        }
        else{
            int digit = round(size/8+1)*8;
            if (digit > 16) {digit = 32;}
            type_values.name = "int" + QString::number(digit) + + "_t";
            start_value = "0";
        }
    }

    void set_type(QString name){
        type_values.name = name;

        if (name == "float")
            start_value = "0.0f";
    }

    void set_type(enum_t type_vals){
        type_values = type_vals;
        if (type_values.values.contains(0))
            start_value = type_values.values[0].toUpper();
        else
            start_value = type_values.values.last().toUpper();
    }
} sgnl_t;


typedef struct {
    QString ID;
    QString msgBoxName;
    int size;
    int cycle_time;
    QString sender;
    QVector<sgnl_t*> *msgSignals;

    sgnl_t* get_sgnl_by_name(QString name){
        sgnl_t *sgnl = nullptr;

        for (int i=0; i < msgSignals->size(); i++){
            auto sg_name = msgSignals->at(i)->name;
            if (msgSignals->at(i)->name.size() >=32)
                sg_name = msgSignals->at(i)->name.left(32);
            if (sg_name == name)
                sgnl = msgSignals->at(i);
        }
        return sgnl;
    }
} msgbox_t;


class DBCParsedData : public AbstractData
{
public:
    QString Reciever;
    QString Transmitter;
    QVector<msgbox_t*> *msgboxes;
    QVector<enum_t *> *enums;

public:
    void init(){
        msgboxes = new QVector<msgbox_t*>;
        enums = new QVector<enum_t *>;
    }

    void push_back(msgbox_t *msgBox){
        msgboxes->append(msgBox);
    }

    void push_back(enum_t *type_value){
        bool founded = false;
        for (int i=0; i<enums->size(); i++){
            if (enums->at(i)->compare(type_value->values) == true){
                *type_value = *enums->at(i);
                founded = true;
                break;
            }
        }
        if (founded==false){
            enums->push_back(type_value);
        }
    }

    msgbox_t* get_by_id(QString id){
        msgbox_t *msgBox = nullptr;
        for (int i=0; i < msgboxes->size(); i++){
            if (msgboxes->at(i)->ID == id)
                msgBox = msgboxes->at(i);
        }
        return msgBox;
    }
};




#endif // PARSEDDATASTRUCTURE_H
