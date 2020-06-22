#include "func.h"
#include "tabentryvalue.h"

//------------------------------------------------------------------------------

TTabEntryValue::TTabEntryValue( TTabEntry  *pAncestor )
{
    m_node.reset();
    m_node_index = -1;
    m_value_index = -1;

    m_pAncestor = pAncestor;

    m_zValue.clear();

    // диалог для имени
    m_ptTabDialogValue = new TTabDialog( false, "Table", this, "Value" );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogValue, &TTabDialog::sendCancel, this, &TTabEntryValue::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogValue, &TTabDialog::sendValue, this, &TTabEntryValue::onSendValue );
}

TTabEntryValue::~TTabEntryValue()
{

}

//------------------------------------------------------------------------------

void  TTabEntryValue::onBtnDec()
{
    qDebug() << "TTabEntryValue" << __func__ << getEntryValue();
}

void  TTabEntryValue::onBtnValue()
{
    qDebug() << "TTabEntryValue" << __func__ << getEntryValue();

    // диалог с пустыми параметрами
    m_ptTabDialogValue->setDlgEmpty();

    m_ptTabDialogValue->setDlgName( getEntryValue() );

    m_ptTabDialogValue->open();
}

//------------------------------------------------------------------------------

void  TTabEntryValue::onSendCancel()
{
    qDebug() << "TTabEntryValue" << __func__ << getEntryValue();
}

void  TTabEntryValue::onSendValue( QString&  a_zValue )
{
    qDebug() << "TTabEntryValue" << __func__ << getEntryValue();

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

void  TTabEntryValue::EntryValueDelete()
{

}

//------------------------------------------------------------------------------

void  TTabEntryValue::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTabEntryValue::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTabEntryValue::getNode()
{
    return m_node;
}

int  TTabEntryValue::getNodeIndex()
{
    return m_node_index;
}

void  TTabEntryValue::setValueIndex( int  index )
{
    m_value_index = index;
}

int  TTabEntryValue::getValueIndex()
{
    return m_value_index;
}

//------------------------------------------------------------------------------

void  TTabEntryValue::setEntryValue( QString&  value )
{
    m_zValue = value;
}

QString  TTabEntryValue::getEntryValue()
{
    return m_zValue;
}

//------------------------------------------------------------------------------
