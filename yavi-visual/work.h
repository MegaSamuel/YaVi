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
    Work();
    ~Work();

    QString zFailReason; // причина ошибки загрузки/выгрузки ямла

    bool    init( const QString&  filename ); // загружаем ямл из файла
    bool    fini( const QString&  filename ); // выгружаем ямл в файл

protected:
    TGoods 	m_tGoods;    // товары считанные из файла
};

//------------------------------------------------------------------------------

} // namespace cobu

#endif // WORK_H
