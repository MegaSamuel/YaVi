#include "func.h"
#include "table_entryvalue.h"

//------------------------------------------------------------------------------

TTableEntryValue::TTableEntryValue( TTableEntry  *pAncestor )
{
    m_node.reset();
    m_node_index = -1;
    m_value_index = -1;

    m_pAncestor = pAncestor;

    m_zValue.clear();

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

void  TTableEntryValue::EntryValueDelete()
{

}

//------------------------------------------------------------------------------

void  TTableEntryValue::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTableEntryValue::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTableEntryValue::getNode()
{
    return m_node;
}

int  TTableEntryValue::getNodeIndex()
{
    return m_node_index;
}

void  TTableEntryValue::setValueIndex( int  index )
{
    m_value_index = index;
}

int  TTableEntryValue::getValueIndex()
{
    return m_value_index;
}

//------------------------------------------------------------------------------

void  TTableEntryValue::setEntryValue( QString&  value )
{
    m_zValue = value;
}

QString  TTableEntryValue::getEntryValue()
{
    return m_zValue;
}

//------------------------------------------------------------------------------
