#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include <vector>

#include "goods.h"

//------------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = Q_NULLPTR );
    ~MainWindow();

    static MainWindow *getMainWinPtr();

    TGoods*     goods() const { return m_pGoods; }

public Q_SLOTS:
    void         onYamlChanged();

private Q_SLOTS:
    void         onTimerWork();
    void         onBtnNew();
    void         onBtnOpen();
    void         onBtnSave();
    void         onBtnUndo();

protected:
    void         keyPressEvent( QKeyEvent *event );

private:
    static MainWindow *pMainWindow;

    QPushButton  *m_ptBtnNew;
    QPushButton  *m_ptBtnOpen;
    QPushButton  *m_ptBtnSave;
    QLabel       *m_ptLblNotice;
    QPushButton  *m_ptBtnUndo;

    QString       m_zFailReason;  // причина ошибки загрузки/выгрузки ямла

    bool          init( const QString&  filename ); // загружаем ямл из файла
    bool          fini( const QString&  filename ); // выгружаем ямл в файл

    QString       m_zPrgName;
    QString       m_zPrgTitle;
    bool          m_bPrgTitleChanged;

    void          setPrgTitleText( const QString&  text = "" );

    void          setPrgTitleChanged( bool  changed );

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

    bool          askSaveIfChanged();
    void          actionAfterStart();

    void          writeSettings();
    void          readSettings();

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

    YAML::Node    m_config; // считанный ямл
    std::vector<std::string>    m_history;

    bool          addHistory( const YAML::Node&  node );
    bool          undoHistory();
    void          showHistory();
};

//------------------------------------------------------------------------------

#endif // MAINWINDOW_H
