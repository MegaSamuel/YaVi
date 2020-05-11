#ifndef DIALOG_H
#define DIALOG_H

#include <QtGui>
#include <QtWidgets>

#include "values.h"

//------------------------------------------------------------------------------

class TPrivDialog;

//------------------------------------------------------------------------------

class TDialog : public QDialog
{
    friend class TPrivDialog;
    Q_OBJECT

public :
    TDialog( bool fullsize = true, QString name = "Unknown", QWidget *parent = Q_NULLPTR );
    ~TDialog();

    void    setDlgName( const QString&  name );
    void    setDlgPlaceholder( const QString&  name );
    void    setDlgNew( const QString&  name );
    void    setDlgAfter( const QString&  name );
    void    setDlgBefore( const QString&  name );
    void    setDlgUlink( const QString&  name );
    void    setDlgUname( const QString&  name );
    void    setDlgMulti( const QString&  name );

    void    setDlgType( unsigned  val );
    void    setDlgMin( unsigned  val );
    void    setDlgMax( unsigned  val );

    void    setDlgCombo( QStringList  list );

Q_SIGNALS:
    void    sendValues( TValues&  values );

protected Q_SLOTS:
    void 	onBtnAction( QAbstractButton *btn );

private:
   std::unique_ptr<TPrivDialog> priv__;
};

//------------------------------------------------------------------------------

#endif // DIALOG_H
