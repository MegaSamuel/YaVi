#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui>
#include <QtWidgets>

//------------------------------------------------------------------------------

class TPrivDialog;

//------------------------------------------------------------------------------

class TDialog : public QDialog
{
    friend class TPrivDialog;
    Q_OBJECT

public :
    TDialog( QWidget *parent = Q_NULLPTR );
    ~TDialog();


protected Q_SLOTS:
    void 	onReset( QAbstractButton *btn );

private:
   std::unique_ptr<TPrivDialog> priv__;
};

//------------------------------------------------------------------------------

#endif // DIALOG_H
