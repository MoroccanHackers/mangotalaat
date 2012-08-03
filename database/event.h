#ifndef DATABASE_EVENT_H
#define DATABASE_EVENT_H

#include "model/event.h"

namespace Database {

class Event
{
public:
    static bool add(Model::Event event);
    static QList<Model::Event> getAll();
    static QList<Model::Event> getBetweenDateTime(QDateTime from, QDateTime to);
};


}
#endif // DATABASE_EVENT_H
