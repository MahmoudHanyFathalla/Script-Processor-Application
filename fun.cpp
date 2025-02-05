#include "fun.h"

class funData : public QSharedData
{
public:
};

fun::fun()
    : data(new funData)
{}

fun::fun(const fun &rhs)
    : data{rhs.data}
{}

fun &fun::operator=(const fun &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

fun::~fun() {}
