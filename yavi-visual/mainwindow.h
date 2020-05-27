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
    explicit MainWindow( QWidget *parent = Q_NULLPTR );
    ~MainWindow();

private Q_SLOTS:
    void         onTimerWork();
    void         onBtnOpen();
    void         onBtnSave();
    void         onYamlChanged();

private:
    QPushButton  *m_ptBtnOpen;
    QPushButton  *m_ptBtnSave;
    QLabel       *m_ptLblNotice;

    QString       m_zFailReason;  // причина ошибки загрузки/выгрузки ямла

    bool          init( const QString&  filename ); // загружаем ямл из файла
    bool          fini( const QString&  filename ); // выгружаем ямл в файл

    QString       m_zPrgName;
    QString       m_zPrgTitle;
    bool          m_bPrgTitleChanged;

    void          setPrgTitleText( const QString&  text = "" );

    bool          getPrgTitleChanged();
    void          setPrgTitleChanged( bool  changed );

    YAML::Node    m_config; // считанный ямл

    double        cpu_time() const;
    double        current_time() const;
    double        m_fBeginTime;

    //! описание таймера
    QTimer       *m_ptTimer;
    unsigned      m_uTimerCounter;

    TGoods 	     *m_pGoods;    // товары считанные из файла

    YAML::Node    m_cfg;                 // считанный ямл с настройками

    QString       m_cfg_last_open_path;  // последняя папка из диалога чтения
    QString       m_cfg_last_save_path;  // последняя папка из диалога записи
    QString       m_cfg_last_open_file;  // имя последнего загруженного файла

    QString       m_cfg_current_path;    // папка откуда мы запустились
    QString       m_cfg_filename;        // имя файла с настройками

    unsigned      m_cfg_autoload;        // автоматическая загрузка последнего файла (0 - no / 1 - yes)

    void          actionAfterStart();

    void          cfgReset() noexcept;

    bool          cfgRead( const QString&  filename );   // прочитать конфиг
    bool          cfgWrite( const QString&  filename );  // сохранить конфиг

    void          cfgSetAutoload( unsigned  load ) noexcept;
    unsigned      cfgGetAutoload() noexcept;

    void          cfgSetCurrentPath( const QString&  path ) noexcept;
    QString       cfgGetCurrentPath() noexcept;

    void          cfgSetLastOpenPath( const QString&  path = "" ) noexcept;
    void          cfgSetLastSavePath( const QString&  path = "" ) noexcept;
    void          cfgSetLastOpenFile( const QString&  file = "" ) noexcept;

    QString       cfgGetLastOpenPath() noexcept;
    QString       cfgGetLastSavePath() noexcept;
    QString       cfgGetLastOpenFile() noexcept;

    void          closeEvent( QCloseEvent * );
};

//------------------------------------------------------------------------------

#endif // MAINWINDOW_H
