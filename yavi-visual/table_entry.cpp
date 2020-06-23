#include "func.h"
#include "table_entry.h"

//------------------------------------------------------------------------------

TTableEntry::TTableEntry( TTable  *pAncestor )
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_pAncestor = pAncestor;

    m_zName.clear();
    m_zValues.clear();

    m_vList.clear();

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    m_hlayout = new QHBoxLayout;
    m_hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_hlayout->setMargin( 0 );

    // диалог для имени
    m_ptTabDialogName = new TTabDialog( false, "Table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogName, &TTabDialog::sendCancel, this, &TTableEntry::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogName, &TTabDialog::sendValue, this, &TTableEntry::onSendValue );
}

TTableEntry::~TTableEntry()
{

}

//------------------------------------------------------------------------------

void  TTableEntry::onBtnDec()
{
    //qDebug() << "TTableEntry" << __func__ << getEntryName();

    //widget_shrink( getTableWidth(), getTableHeight() + m_grid->spacing() );

    EntryDelete();
}

void  TTableEntry::onBtnName()
{
    //qDebug() << "TTableEntry" << __func__ << getEntryName();

    // диалог с пустыми параметрами
    m_ptTabDialogName->setDlgEmpty();

    m_ptTabDialogName->setDlgName( getEntryName() );

    m_ptTabDialogName->open();
}

//------------------------------------------------------------------------------

void  TTableEntry::onSendCancel()
{
    //qDebug() << "TTableEntry" << __func__ << getEntryName();
}

void  TTableEntry::onSendValue( QString& a_zValue )
{
    //qDebug() << "TTableEntry" << __func__ << getEntryName();

    setEntryName( a_zValue.toStdString(), true );

    // шлем сигнал с именем
    Q_EMIT sendName( a_zValue, getNodeIndex() );

    if( 0 == a_zValue.length() )
    {
        // если имя пустое, то удаляем запись
        onBtnDec();
    }
}

void  TTableEntry::onSendEntryValue( QString& a_zValue, int  a_nIndex )
{
    qDebug() << __func__ << getEntryName() << a_zValue << a_nIndex;

    if( 0 != a_zValue.length() )
    {
        // переименовываем запись в values
        renameParamList( a_zValue, a_nIndex, true );
    }

    if( 0 == a_zValue.length() )
    {
        // если имя пустое, то удаляем запись

        // удаляемся из списка детей
        m_apValueList.removeAt( a_nIndex );

        // удаляем запись в values
        removeParamList( a_zValue, a_nIndex, true );

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_apValueList.count(); i++ )
        {
            index = m_apValueList.at(i)->getValueIndex();

            if( index > a_nIndex )
            {
                index -= 1;

                m_apValueList.at(i)->setValueIndex( index );
            }
        }
    }
}

//------------------------------------------------------------------------------

void  TTableEntry::makeStrByList() noexcept
{
    int  ind = 0;
    m_zList.clear();

    // формируем новую строку
    for( auto& it : m_vList )
    {
        if( 0 != ind )
            m_zList.append( "\n" );

        m_zList.append( it.toStdString() );

        ind++;
    }
}

void  TTableEntry::renameParamList( QString&  item, int  index, bool  set_to_node )
{
    m_vList[ index ] = item;

    makeStrByList();

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
    }
}

void  TTableEntry::removeParamList( QString&  item, int  index, bool  set_to_node )
{
    Q_UNUSED( item );

    m_vList.removeAt( index );

    makeStrByList();

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
    }
}

void  TTableEntry::addParamList( QString&  item, bool  set_to_node )
{
    // добавляем запись в строку
    if( 0 == m_zList.length() )
    {
        // если строка списка пустая, то просто добавляем
        m_zList.append( item.toStdString() );
    }
    else
    {
        // если строка списка не пустая, то добавляем с префиксом
        m_zList.append( "\n" );
        m_zList.append( item.toStdString() );
    }

    // добавляем запись в список
    m_vList.append( item );

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
    }
}

void  TTableEntry::setParamList( QStringList  vlist, const std::string&  zlist, bool  set_to_node )
{
    m_vList = vlist;

    m_zList = zlist;

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsValuesSection, m_zList );
    }
}

//------------------------------------------------------------------------------

void  TTableEntry::clearNodeSequence()
{
    // если не знаем индекс, то удаляем поля
    if( -1 == m_node_index )
    {
        m_node.remove( GoodsTableName );
        m_node.remove( GoodsTableValue );
    }

    // если знаем индекс, то удаляем всю ветку
    if( -1 != m_node_index )
    {
        m_node_parent.remove( m_node_index );
    }
}

void  TTableEntry::EntryDelete()
{
    QLayoutItem *child;

    // уничтожаем виджеты
    while( ( child = m_vlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем виджеты
    while( ( child = m_hlayout->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем layout-ы
    m_vlayout->deleteLater();
    m_hlayout->deleteLater();

    // очищаем ветку
    clearNodeSequence();
}

//------------------------------------------------------------------------------

void  TTableEntry::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTableEntry::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TTableEntry::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTableEntry::getNode()
{
    return m_node;
}

YAML::Node&  TTableEntry::getNodeParent()
{
    return m_node_parent;
}

int  TTableEntry::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TTableEntry::setEntryName( const std::string&  name, bool  set_to_node )
{
    m_zName = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, "name", name );
    }
}

/*
void  TTableEntry::setEntryValues( const std::string&  name, bool  set_to_node )
{
    m_zValues = QString::fromStdString(name);

    if( set_to_node )
    {
        __yaml_SetString( m_node, "values", name );
    }
}
*/

QString  TTableEntry::getEntryName()
{
    return m_zName;
}

/*
QString  TTableEntry::getEntryValues()
{
    return m_zValues;
}
*/

//------------------------------------------------------------------------------
