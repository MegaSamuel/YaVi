#ifndef TABDIALOG_H
#define TABDIALOG_H

#include <QtGui>
#include <QtWidgets>

#include "values.h"

//------------------------------------------------------------------------------

class TPrivTabDialog;

//------------------------------------------------------------------------------

class TTabDialog : public QDialog
{
    friend class TPrivTabDialog;
    Q_OBJECT

public :
    explicit TTabDialog( bool fullsize = true, QString name = "Unknown", QWidget *parent = Q_NULLPTR, QString value = "" );
    ~TTabDialog();

    void    setDlgId( const QString&  name );
    void    setDlgName( const QString&  name );

    void    setDlgEmpty();

Q_SIGNALS:
    void    sendCancel();
    void    sendChanged();
    void    sendValue( QString&  value );
    void    sendValues( TValues&  values );

protected Q_SLOTS:
    void 	onBtnAction( QAbstractButton *btn );

private:
    std::unique_ptr<TPrivTabDialog> priv__;

    bool    m_bFullSize;
};

//------------------------------------------------------------------------------

#endif // TABDIALOG_H
