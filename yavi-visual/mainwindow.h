#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "work.h"

namespace cobu
{

//------------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

protected Q_SLOTS :
    void    onBtnOpen();
    void    onBtnSave();

private:
    QWidget    *createStuff(QWidget *parent);

    QPushButton  *m_ptBtnOpen;
    QPushButton  *m_ptBtnSave;
    QLabel       *m_ptLblNotice;

    Work  *m_pWork;
};

//------------------------------------------------------------------------------

} // namespace cobu

#endif // MAINWINDOW_H
