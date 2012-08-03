#ifndef MODEL_ADDITIONALS_H
#define MODEL_ADDITIONALS_H

#include <QString>

#include "baseitem.h"

namespace Model
{

class Additional : public BaseItem
{
public:

    Additional(int id);
    Additional(int id, int price, const QString& arabicName, const QString& englishName);

    int price() const;
    void setPrice(int price);

private:
    int m_price;
};

}

#endif // MODEL_ADDITIONALS_H
