#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "goods.h"

//------------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private Q_SLOTS:
    void         onTimerWork();
    void         onBtnOpen();
    void         onBtnSave();

private:
    QPushButton  *m_ptBtnOpen;
    QPushButton  *m_ptBtnSave;
    QLabel       *m_ptLblNotice;

    QString       zFailReason;  // причина ошибки загрузки/выгрузки ямла

    bool          init( const QString&  filename ); // загружаем ямл из файла
    bool          fini( const QString&  filename ); // выгружаем ямл в файл

    YAML::Node    m_config; // считанный ямл

    double        cpu_time() const;
    double        current_time() const;
    double        m_fBeginTime;

    //! описание таймера
    QTimer       *m_ptTimer;
    unsigned      m_uTimerCounter;

protected:
    TGoods 	     *m_pGoods;    // товары считанные из файла
};

//------------------------------------------------------------------------------

#endif // MAINWINDOW_H
