#include "func.h"
#include "table_entry.h"

//------------------------------------------------------------------------------

TTableEntry::TTableEntry( TTable  *pAncestor )
{
    clear();

    m_pAncestor = pAncestor;

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

    // диалог для нового значения
    m_ptTabDialogAdd = new TTabDialog( false, "Table", this, "Value" );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogAdd, &TTabDialog::sendCancel, this, &TTableEntry::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogAdd, &TTabDialog::sendValue, this, &TTableEntry::onSendValue );
}

TTableEntry::~TTableEntry()
{

}

void  TTableEntry::clear() noexcept
{
    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_entry_type = 0;

    m_zName.clear();

    m_zList.clear();
    m_vList.clear();

    need_to_add = false;
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

void  TTableEntry::onBtnInc()
{
    // признак что хотим создать новый набор параметров
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptTabDialogAdd->setDlgEmpty();

    m_ptTabDialogAdd->setDlgName( "NewValue" );

    m_ptTabDialogAdd->open();
}

//------------------------------------------------------------------------------

void  TTableEntry::onSendCancel()
{
    //qDebug() << "TTableEntry" << __func__ << getEntryName();

    need_to_add = false;
}

void  TTableEntry::onSendValue( QString& a_zValue )
{
    //qDebug() << "TTableEntry" << __func__ << getEntryName();

    QString  value = a_zValue;

    if( false == need_to_add )
    {
        setEntryName( value.toStdString(), true );

        // шлем сигнал с именем
        Q_EMIT sendName( value, getNodeIndex() );

        if( 0 == value.length() )
        {
            // если имя пустое, то удаляем запись
            onBtnDec();
        }
    }

    if( true == need_to_add )
    {
        // создаем новое значение

        qDebug() << "new value";

        if( 0 == value.length() )
        {
            // если имя пустое, то ставим дефолтное имя
            value = "noname";
        }

        addParamList( value, true );

        // кнопки со значениями
        QPushButton  *button = new QPushButton( value );
        button->setToolTip( "Значение: " + value );
        button->setFixedWidth( 93 );
        //setTableEntryValue( pEntry, value, i );

        TTableEntryValue  *pEntryValue;

        // новая запись
        pEntryValue = new TTableEntryValue( this );
        pEntryValue->setEntryValue( value );
        pEntryValue->setValueIndex( m_apValueList.size() );

        // ловим сигнал с данными значения
        connect( pEntryValue, &TTableEntryValue::sendEntryValue, this, &TTableEntry::onSendEntryValue );

        // добавляем запись в список
        m_apValueList.append( pEntryValue );

        connect( button, &QPushButton::clicked, m_apValueList.last(), &TTableEntryValue::onBtnValue );
        //connect( button, &QPushButton::clicked, this, &TTable::onBtnRowValName );
        connect( m_apValueList.last(), &TTableEntryValue::sendEntryValue, button, &QPushButton::setText );

        // в конце кнопка Плюс, надо добавлять на предпоследнюю позицию
        if( TValues::keTypeColumn == getEntryType() )
        {
            m_vlayout->insertWidget( m_vlayout->count()-1, button, 0, Qt::AlignLeft );
        }
        else if( TValues::keTypeRow == getEntryType() )
        {
            m_hlayout->insertWidget( m_hlayout->count()-1, button, 0, Qt::AlignLeft );
        }
    }

    need_to_add = false;
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

        QLayoutItem *child;

        // уничтожаем кнопку
        // +2 т.к. у нас в начале layout-а кнопка Плюс и кнопка Имя
        if( ( child = m_vlayout->takeAt(a_nIndex+2) ) != Q_NULLPTR )
        {
            delete child->widget();
            delete child;
        }

        // уничтожаем кнопку
        if( ( child = m_hlayout->takeAt(a_nIndex+2) ) != Q_NULLPTR )
        {
            delete child->widget();
            delete child;
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

void  TTableEntry::setNodeIndex( int  index ) noexcept
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

int  TTableEntry::getNodeIndex() noexcept
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

QString  TTableEntry::getEntryName() noexcept
{
    return m_zName;
}

void  TTableEntry::setEntryType( int  type ) noexcept
{
    m_entry_type = type;
}

int  TTableEntry::getEntryType() noexcept
{
    return m_entry_type;
}

//------------------------------------------------------------------------------

void  TTableEntry::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;
}

void  TTableEntry::widget_stretch( int width, int height, bool add_height ) noexcept
{
    // ширину выбираем максимальную из элементов
    if( width > m_width )
        m_width = width;

    //qDebug() << "stretch" << width << height;

    // высоту увеличиваем на каждый элемент
    m_height += height;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_stretch( width, height, add_height );
}

void  TTableEntry::widget_parent_stretch( int width, int height, bool add_height ) noexcept
{
    int  val = 0;

    if( Q_NULLPTR != m_pAncestor )
    {
        if( add_height )
        {
//            val = m_pAncestor->m_vlayout->spacing();
        }

//        m_pAncestor->widget_stretch( width, height + val );
    }
}

void  TTableEntry::widget_shrink( int width, int height ) noexcept
{
    Q_UNUSED( width );

    //qDebug() << "shrink" << width << height;

    m_height -= height;

    if( m_height < 0 )
        m_height = 0;

    // ставим размер самого себя
    setMinimumWidth( m_width );
    setMinimumHeight( m_height );

    widget_parent_shrink( width, height );
}

void  TTableEntry::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
//        m_pAncestor->widget_shrink( width, height );
    }
}

int TTableEntry::getTableEntryWidth() noexcept
{
    return minimumSize().width();
}

int TTableEntry::getTableEntryHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------
