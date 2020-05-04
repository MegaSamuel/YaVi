#ifndef WORK_H
#define WORK_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

#include "goods.h"

//------------------------------------------------------------------------------

class TWork : public QWidget
{
    Q_OBJECT

public:
    TWork(QWidget *parent = Q_NULLPTR);
    ~TWork();

    QString zFailReason;  // причина ошибки загрузки/выгрузки ямла

    bool    init( const QString&  filename ); // загружаем ямл из файла
    bool    fini( const QString&  filename ); // выгружаем ямл в файл

private:
    YAML::Node 	m_config; // считанный ямл

protected:
    TGoods 	*m_pGoods;    // товары считанные из файла
};

//------------------------------------------------------------------------------

#endif // WORK_H
