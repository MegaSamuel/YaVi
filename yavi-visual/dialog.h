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
    explicit TDialog( bool fullsize = true, QString name = "Unknown", QWidget *parent = Q_NULLPTR );
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

    void    setDlgMin( int  val );
    void    setDlgMax( int  val );

    void    setDlgDMin( double  val );
    void    setDlgDMax( double  val );

    void    setDlgCombo( QStringList  list );

    void    setDlgEmpty();

    void    setDlgEnabled( bool  enabled ) noexcept;

    void    setDlgTypeEnabled( bool  enabled ) noexcept;
    void    setDlgPlaceholderEnabled( bool  enabled ) noexcept;
    void    setDlgNewEnabled( bool  enabled ) noexcept;
    void    setDlgAfterEnabled( bool  enabled ) noexcept;
    void    setDlgBeforeEnabled( bool  enabled ) noexcept;
    void    setDlgUlinkEnabled( bool  enabled ) noexcept;
    void    setDlgUnameEnabled( bool  enabled ) noexcept;
    void    setDlgMultiEnabled( bool  enabled ) noexcept;
    void    setDlgMinEnabled( bool  enabled ) noexcept;
    void    setDlgMaxEnabled( bool  enabled ) noexcept;
    void    setDlgComboEnabled( bool  enabled ) noexcept;

Q_SIGNALS:
    void    sendCancel();
    void    sendChanged();
    void    sendValues( TValues&  values );

protected Q_SLOTS:
    void    onTypeChanged( int );
    void 	onBtnAction( QAbstractButton *btn );

private:
    std::unique_ptr<TPrivDialog> priv__;

    int     row_min;
    int     row_max;

    void    setDlgEnabledByType( unsigned  type ) noexcept;
    void    setDlgShowByType( unsigned  type ) noexcept;
};

//------------------------------------------------------------------------------

#endif // DIALOG_H
