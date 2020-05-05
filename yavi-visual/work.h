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
    TWork();
    ~TWork();

    QString zFailReason;  // причина ошибки загрузки/выгрузки ямла

    bool    init( const QString&  filename ); // загружаем ямл из файла
    bool    fini( const QString&  filename ); // выгружаем ямл в файл

    void    draw();
    void    undraw();

    QHBoxLayout *m_hlayout;

private Q_SLOTS:
    void    TimerWork();

private:
    YAML::Node 	m_config; // считанный ямл

    double      cpu_time() const;
    double      current_time() const;
    double      m_fBeginTime;

    //! описание таймера
    QTimer     *m_ptTimer;
    unsigned    m_uTimerCounter;

protected:
    TGoods 	*m_pGoods;    // товары считанные из файла
};

//------------------------------------------------------------------------------

#endif // WORK_H
