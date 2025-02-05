#ifndef FUN_H
#define FUN_H

#include <QDeclarativeItem>
#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>

class funData;

class fun
{
    Q_OBJECT
    QML_ELEMENT
public:
    fun();
    fun(const fun &);
    fun &operator=(const fun &);
    ~fun();

private:
    QSharedDataPointer<funData> data;
};

#endif // FUN_H
