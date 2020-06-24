#include "func.h"
#include "table_entryvalue.h"

//------------------------------------------------------------------------------

TTableEntryValue::TTableEntryValue( TTableEntry  *pAncestor )
{
    clear();

    m_pAncestor = pAncestor;

    // диалог для имени
    m_ptTabDialogValue = new TTabDialog( false, "Table", this, "Value" );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogValue, &TTabDialog::sendCancel, this, &TTableEntryValue::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogValue, &TTabDialog::sendValue, this, &TTableEntryValue::onSendValue );
}

TTableEntryValue::~TTableEntryValue()
{

}

void  TTableEntryValue::clear() noexcept
{
    m_value_index = -1;

    m_zValue.clear();
}

//------------------------------------------------------------------------------

void  TTableEntryValue::onBtnDec()
{
    qDebug() << "TTableEntryValue" << __func__ << getEntryValue();
}

void  TTableEntryValue::onBtnValue()
{
    qDebug() << "TTableEntryValue" << __func__ << getEntryValue();

    // диалог с пустыми параметрами
    m_ptTabDialogValue->setDlgEmpty();

    m_ptTabDialogValue->setDlgName( getEntryValue() );

    m_ptTabDialogValue->open();
}

//------------------------------------------------------------------------------

void  TTableEntryValue::onSendCancel()
{
    qDebug() << "TTableEntryValue" << __func__ << getEntryValue();
}

void  TTableEntryValue::onSendValue( QString&  a_zValue )
{
    qDebug() << "TTableEntryValue" << __func__ << getEntryValue();

    setEntryValue( a_zValue );

    // шлем сигнал с именем
    Q_EMIT sendEntryValue( a_zValue, getValueIndex() );

    if( 0 == a_zValue.length() )
    {
        // если имя пустое, то удаляем запись
        onBtnDec();
    }
}

//------------------------------------------------------------------------------

void  TTableEntryValue::setValueIndex( int  index ) noexcept
{
    m_value_index = index;
}

int  TTableEntryValue::getValueIndex() noexcept
{
    return m_value_index;
}

//------------------------------------------------------------------------------

void  TTableEntryValue::setEntryValue( QString&  value ) noexcept
{
    m_zValue = value;
}

QString  TTableEntryValue::getEntryValue() noexcept
{
    return m_zValue;
}

//------------------------------------------------------------------------------
