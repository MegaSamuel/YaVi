#ifndef TABENTRYVALUE_H
#define TABENTRYVALUE_H

#include <QtWidgets>

#include "values.h"
#include "table_dialog.h"

//------------------------------------------------------------------------------

class  TTableEntry;

//------------------------------------------------------------------------------

class TTableEntryValue : public QWidget
{
   Q_OBJECT

public:
    explicit TTableEntryValue( TTableEntry  *pAncestor = Q_NULLPTR );
    ~TTableEntryValue();

    void           setValueIndex( int  index ) noexcept;
    int            getValueIndex() noexcept;

    void           setEntryValue( QString&  value ) noexcept;
    QString        getEntryValue() noexcept;

Q_SIGNALS:
    void           sendEntryValue( QString&  value, int  index = 0 );

public Q_SLOTS:
    void           onBtnDec();
    void           onBtnValue();

private Q_SLOTS:
    void           onSendCancel();
    void           onSendValue( QString& );

private:
    void           clear() noexcept;

    int            m_value_index;  // номер записи в values

    TTableEntry   *m_pAncestor;

    QString        m_zValue;

    TTabDialog    *m_ptTabDialogValue;
};

//------------------------------------------------------------------------------

#endif // TABENTRYVALUE_H
