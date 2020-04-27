#ifndef WORK_H
#define WORK_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

#include "goods.h"

namespace cobu
{

//------------------------------------------------------------------------------

class Work : public QObject
{
    Q_OBJECT

public:
//    Work( const QString &filename, QObject  *parent = Q_NULLPTR );
    Work();
    ~Work();

    bool    bInit;       // результат загрузки ямла
    QString zFailReason; // причина ошибки загрузки ямла

    bool    init( const QString&  filename ); // загружаем ямл из файла

protected:
    TGoods 	m_tGoods;    // товары считанные из файла
};

//------------------------------------------------------------------------------

} // namespace cobu

#endif // WORK_H
