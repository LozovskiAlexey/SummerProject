#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "BaseMediator.h"
#include "ParsedDataStructure.h"
#include "Data.h"
#include "dbcdata.h"

class BaseComponent {
protected:
    BaseMediator *mediator_;
    
public:
    BaseComponent(BaseMediator *mediator = nullptr)
        : mediator_(mediator) {}

    void set_mediator(BaseMediator *mediator)
    {
        this->mediator_ = mediator;
    }
    
    virtual void setData(DBCdata *) {}
    virtual void setData(DBCParsedData *) {}
    virtual void exec()=0;
};


#endif // BASECOMPONENT_H
