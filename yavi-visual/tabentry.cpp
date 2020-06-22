#include "func.h"
#include "tabentry.h"

//------------------------------------------------------------------------------

TTabEntry::TTabEntry( TTable  *pAncestor )
{
    m_node.reset();
    m_node_index = -1;

    m_pAncestor = pAncestor;

    m_zName.clear();
    m_zValues.clear();

    // диалог для имени
    m_ptTabDialogName = new TTabDialog( false, "Table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogName, &TTabDialog::sendCancel, this, &TTabEntry::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogName, &TTabDialog::sendValue, this, &TTabEntry::onSendValue );
}

TTabEntry::~TTabEntry()
{

}

//------------------------------------------------------------------------------

void  TTabEntry::onBtnDec()
{
    //qDebug() << "TTabEntry" << __func__ << getEntryName();
}

void  TTabEntry::onBtnName()
{
    //qDebug() << "TTabEntry" << __func__ << getEntryName();

    // диалог с пустыми параметрами
    m_ptTabDialogName->setDlgEmpty();

    m_ptTabDialogName->setDlgName( getEntryName() );

    m_ptTabDialogName->open();
}

//------------------------------------------------------------------------------

void  TTabEntry::onSendCancel()
{
    //qDebug() << "TTabEntry" << __func__ << getEntryName();
}

void  TTabEntry::onSendValue( QString& a_zValue )
{
    //qDebug() << "TTabEntry" << __func__ << getEntryName();

    setEntryName( a_zValue.toStdString(), true );

    // шлем сигнал с именем
    Q_EMIT sendName( a_zValue );

    if( 0 == a_zValue.length() )
    {
        // если имя пустое, то удаляем запись
        onBtnDec();
    }
}

//------------------------------------------------------------------------------

void  TTabEntry::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTabEntry::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTabEntry::getNode()
{
    return m_node;
}

int  TTabEntry::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TTabEntry::setEntryName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, "name", name );
    }
}

void  TTabEntry::setEntryValues( const std::string&  name, bool  set_to_node )
{
    m_zValues = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, "values", name );
    }
}

QString  TTabEntry::getEntryName()
{
    return m_zName;
}

QString  TTabEntry::getEntryValues()
{
    return m_zValues;
}

//------------------------------------------------------------------------------
