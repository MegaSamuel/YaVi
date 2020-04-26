#ifndef WORK_H
#define WORK_H

#include <QtWidgets>

#include <yaml-cpp/yaml.h>

class Work : public QObject
{
    Q_OBJECT

public:
    Work( const QString &filename, QObject  *parent = Q_NULLPTR );
    ~Work();

    bool    init( const QString&  filename );
};

#endif // WORK_H
