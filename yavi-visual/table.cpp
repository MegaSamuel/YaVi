#include "func.h"
#include "goods.h"
#include "table.h"
#include "mainwindow.h"

//------------------------------------------------------------------------------

TTable::TTable( TGoods  *pAncestor )
{
    clear();

    // диалог для айди
    m_ptTabDialogId = new TTabDialog( false, "Table", this, "ID" );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogId, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogId, &TTabDialog::sendValue, this, &TTable::onSendValue );

    // диалог для имени
    m_ptTabDialogName = new TTabDialog( false, "Table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogName, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogName, &TTabDialog::sendValue, this, &TTable::onSendValue );

    // диалог для новой таблицы
    m_ptTabDialogAdd = new TTabDialog( true, "Add table", this );

    // ловим сигнал от диалога об отмене
    connect( m_ptTabDialogAdd, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

    // ловим сигнал от диалога с данными
    connect( m_ptTabDialogAdd, &TTabDialog::sendValues, this, &TTable::onSendValues );

    // указатель на родителя
    m_pAncestor = pAncestor;

    // цепляем местный сигнал к слоту MainWindow
    connect( this, &TTable::sendChanged, MainWindow::getMainWinPtr(), &MainWindow::onYamlChanged );

    m_grid = new QGridLayout;
    m_grid->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_grid->setMargin(0);

    m_vlayout = new QVBoxLayout;
    m_vlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_vlayout->setMargin( 0 );

    m_hlayout = new QHBoxLayout;
    m_hlayout->setAlignment( Qt::AlignLeft | Qt::AlignTop );
    m_hlayout->setMargin( 0 );

    // кнопка с id
    m_ptBtnId = new QPushButton( "id" );
    m_ptBtnId->setToolTip( "ID таблицы" );
    m_ptBtnId->setFixedWidth( 93 );
    connect( m_ptBtnId, &QPushButton::clicked, this, &TTable::onBtnId );
    m_grid->addWidget( m_ptBtnId, m_row, 0, Qt::AlignLeft );

    // кнопка с именем
    m_ptBtnName = new QPushButton( "name" );
    m_ptBtnName->setToolTip( "Название таблицы" );
    m_ptBtnName->setFixedWidth( 93 );
    connect( m_ptBtnName, &QPushButton::clicked, this, &TTable::onBtnName );
    m_grid->addWidget( m_ptBtnName, m_row, 1, Qt::AlignLeft );

    // кнопка плюс
    m_ptBtnInc = new QPushButton( "+" );
    m_ptBtnInc->setToolTip( "Добавить таблицу" );
    m_ptBtnInc->setFixedWidth( 93 );
    connect( m_ptBtnInc, &QPushButton::clicked, this, &TTable::onBtnInc );
    m_grid->addWidget( m_ptBtnInc, m_row+1, 0, Qt::AlignLeft | Qt::AlignTop );

    // перевод строки и колонки в grid для следующих добавлений
    nextRow();
    nextColumn();

    setLayout( m_grid );

    widget_size_reset();

    int  width = 2*m_grid->margin() + m_ptBtnId->minimumSize().width() + m_ptBtnName->minimumSize().width() + m_grid->spacing();
    int  height = 2*m_grid->margin() + m_ptBtnId->minimumSizeHint().height() + m_ptBtnInc->minimumSizeHint().height() + m_grid->spacing();

    widget_stretch( width, height );
}

TTable::~TTable()
{

}

void  TTable::clear() noexcept
{
    resetRow();
    resetColumn();

    m_node.reset();
    m_node_parent.reset();
    m_node_index = -1;

    m_temporary_node = YAML::Node();
    m_temporary_inner_node = YAML::Node();

    m_uTableType = TValues::keTypeNone;

    m_zId.clear();
    m_zName.clear();
    m_zLink.clear();

    need_to_add = false;
    need_to_add_row = false;
    need_to_add_column = false;

    clear_edit();
}

void  TTable::clear_edit() noexcept
{
    edit_id = false;
    edit_name = false;
    edit_link = false;
}

//------------------------------------------------------------------------------

void  TTable::onBtnId()
{
    // признак что хотим отредактировать имя
    edit_id = true;

    // диалог с пустыми параметрами
    m_ptTabDialogId->setDlgEmpty();

    m_ptTabDialogId->setDlgName( getTableId() );

    m_ptTabDialogId->open();
}

void  TTable::onBtnName()
{
    // признак что хотим отредактировать имя
    edit_name = true;

    // диалог с пустыми параметрами
    m_ptTabDialogName->setDlgEmpty();

    m_ptTabDialogName->setDlgName( getTableName() );

    m_ptTabDialogName->open();
}

void  TTable::onBtnLink()
{
    // признак что хотим отредактировать ссылку
    edit_link = true;

    // диалог с пустыми параметрами
    m_ptTabDialogLink->setDlgEmpty();

    m_ptTabDialogLink->setDlgName( getTableLink() );

    m_ptTabDialogLink->open();
}

void  TTable::onBtnDec()
{
    widget_shrink( getTableWidth(), getTableHeight() + m_grid->spacing() );

    TableDelete();
}

void  TTable::onBtnInc()
{
    // признак что хотим создать новую таблицу
    need_to_add = true;

    // диалог с пустыми параметрами
    m_ptTabDialogAdd->setDlgEmpty();

    m_ptTabDialogAdd->setDlgId( "Id" );
    m_ptTabDialogAdd->setDlgName( "Table" );

    m_ptTabDialogAdd->open();
}

void  TTable::onBtnRowInc()
{
    //qDebug() << __func__;

    // признак что хотим создать новую запись
    need_to_add_row = true;

    // диалог с пустыми параметрами
    m_ptTabDialogAddEntry->setDlgEmpty();

    m_ptTabDialogAddEntry->setDlgName( "NewRow" );

    m_ptTabDialogAddEntry->open();
}

void  TTable::onBtnRowName( QString&  name, int  entry_index )
{
    //qDebug() << __func__ << name;

    if( 0 == name.length() )
    {
        // удаляемся из layout-а
        m_vlayout->removeWidget( m_apTabEntryList.at(entry_index) );

        // удаляем себя из списка
        m_apTabEntryList.removeAt(entry_index);

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_apTabEntryList.count(); i++ )
        {
            index = m_apTabEntryList.at(i)->getNodeIndex();

            if( index > entry_index )
            {
                index -= 1;

                m_apTabEntryList.at(i)->setNodeIndex( index );
            }
        }
    }
}

void  TTable::onBtnRowValInc()
{
    qDebug() << __func__;
}

void  TTable::onBtnRowValName()
{
    qDebug() << __func__;
}

void  TTable::onBtnColumnInc()
{
    //qDebug() << __func__;

    // признак что хотим создать новую запись
    need_to_add_column = true;

    // диалог с пустыми параметрами
    m_ptTabDialogAddEntry->setDlgEmpty();

    m_ptTabDialogAddEntry->setDlgName( "NewColumn" );

    m_ptTabDialogAddEntry->open();
}

void  TTable::onBtnColumnName( QString&  name, int  entry_index )
{
    //qDebug() << __func__ << name;

    if( 0 == name.length() )
    {
        // удаляемся из layout-а
        m_hlayout->removeWidget( m_apTabEntryList.at(entry_index) );

        // удаляем себя из списка
        m_apTabEntryList.removeAt(entry_index);

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_apTabEntryList.count(); i++ )
        {
            index = m_apTabEntryList.at(i)->getNodeIndex();

            if( index > entry_index )
            {
                index -= 1;

                m_apTabEntryList.at(i)->setNodeIndex( index );
            }
        }
    }
}

void  TTable::onBtnColumnValInc()
{
    qDebug() << __func__;
}

void  TTable::onBtnColumnValName()
{
    qDebug() << __func__;
}

void  TTable::onSendCancel()
{
    need_to_add = false;
    need_to_add_row = false;
    need_to_add_column = false;

    clear_edit();
}

void  TTable::onSendValue( QString& a_zValue )
{
    // редактируем текущую таблицу

    if( edit_id )
    {
        m_tValues.m_zId = a_zValue;

        setTableId( m_tValues.m_zId.toStdString(), true );

        if( 0 == m_tValues.m_zId.length() )
        {
            // если имя пустое, то удаляем таблицу
            onBtnDec();
        }
    }

    if( edit_name )
    {
        m_tValues.m_zName = a_zValue;

        setTableName( m_tValues.m_zName.toStdString(), true );

        if( 0 == m_tValues.m_zName.length() )
        {
            // если имя пустое, то удаляем таблицу
            onBtnDec();
        }
    }

    if( edit_link )
    {
        setTableLink( a_zValue.toStdString(), true );
    }

    clear_edit();
}

void  TTable::onSendValues( TValues& a_tValues )
{
    m_tValues = a_tValues;

    if( true == need_to_add )
    {
        // создаем новую таблицу

        if( 0 == m_tValues.m_zName.length() )
        {
            // если имя пустое, то ставим дефолтное имя
            m_tValues.m_zName = "noname";
        }

        // добавляем таблицу
        TTable  *pTable;
        pTable = new TTable( m_pAncestor );

        // добавляемся к родителю
        m_pAncestor->m_vlayout->addWidget( pTable );
        m_pAncestor->m_apTableList.append(pTable);

        // ставим значения параметров
        pTable->setTableId( m_tValues.m_zId.toStdString() );
        pTable->setTableName( m_tValues.m_zName.toStdString() );
        pTable->setTableType( m_tValues.m_uType );

        // очищаем ямл
        m_temporary_node.reset();

        // пишем их в пустой ямл
        __yaml_SetString( m_temporary_node, GoodsTableId, m_tValues.m_zId.toStdString() );
        __yaml_SetString( m_temporary_node, GoodsTableName, m_tValues.m_zName.toStdString() );

        if( TValues::keTypeLink == m_tValues.m_uType )
        {
            __yaml_SetString( m_temporary_node, GoodsTableLink, "link" );
        }
        else if( TValues::keTypeColumn == m_tValues.m_uType )
        {
            // очищаем ямл
            m_temporary_inner_node.reset();

            // пустая запись столбца
            __yaml_SetString( m_temporary_inner_node, GoodsTableName, "name" );
            __yaml_SetString( m_temporary_inner_node, GoodsTableValue, "value" );

            // добавляем ямл к временному
            m_temporary_node[ GoodsTableColumn ].push_back( m_temporary_inner_node );
        }
        else if( TValues::keTypeRow == m_tValues.m_uType )
        {
            // очищаем ямл
            m_temporary_inner_node.reset();

            // пустая запись строки
            __yaml_SetString( m_temporary_inner_node, GoodsTableName, "name" );
            __yaml_SetString( m_temporary_inner_node, GoodsTableValue, "value" );

            // добавляем ямл к временному
            m_temporary_node[ GoodsTableRow ].push_back( m_temporary_inner_node );
        }

        // добавляем ямл к основному
        m_node_parent.push_back( m_temporary_node );

        int index = static_cast<int>(m_node_parent.size()) - 1;

        pTable->setNode( m_node_parent[index] );
        pTable->setNodeParent( m_node_parent );
        pTable->setNodeIndex( index );
    }

    need_to_add = false;
}

void  TTable::onSendEntry( TValues& a_tValues )
{
    m_tValues = a_tValues;

    if( true == need_to_add_row )
    {
        // создаем новую запись

        if( 0 == m_tValues.m_zName.length() )
        {
            // если имя пустое, то ставим дефолтное имя
            m_tValues.m_zName = "noname";
        }

        int  index = m_apTabEntryList.size();
        TTableEntry  *pEntry;

        // новая запись
        pEntry = new TTableEntry( this );
        pEntry->setNode( m_node[ GoodsTableRow ][index] );
        pEntry->setNodeParent( m_node[ GoodsTableRow ] );
        pEntry->setNodeIndex( index );
        pEntry->setEntryType( TValues::keTypeRow );

        // добавляем запись в список
        m_apTabEntryList.append( pEntry );

        // имя
        std::string  row_name = m_tValues.m_zName.toStdString();
        pEntry->setEntryName( row_name, true );

        // значение в виде строки
        const std::string  row_val = "";
        pEntry->setEntryValues( row_val, true );

        // значение в виде списока
        QStringList  row_list;
        row_list.clear();

        pEntry->setParamList( row_list, row_val );
        setTableRow( row_name, row_list );
    }

    if( true == need_to_add_column )
    {
        // создаем новую запись

        if( 0 == m_tValues.m_zName.length() )
        {
            // если имя пустое, то ставим дефолтное имя
            m_tValues.m_zName = "noname";
        }

        int  index = m_apTabEntryList.size();
        TTableEntry  *pEntry;

        // новая запись
        pEntry = new TTableEntry( this );
        pEntry->setNode( m_node[ GoodsTableColumn ][index] );
        pEntry->setNodeParent( m_node[ GoodsTableColumn ] );
        pEntry->setNodeIndex( index );
        pEntry->setEntryType( TValues::keTypeColumn );

        // добавляем запись в список
        m_apTabEntryList.append( pEntry );

        // имя
        std::string  col_name = m_tValues.m_zName.toStdString();
        pEntry->setEntryName( col_name, true );

        // значение в виде строки
        const std::string  col_val = "";
        pEntry->setEntryValues( col_val, true );

        // значение в виде списока
        QStringList  col_list;
        col_list.clear();

        pEntry->setParamList( col_list, col_val );
        setTableColumn( col_name, col_list );
    }

    need_to_add_row = false;
    need_to_add_column = false;
}

//------------------------------------------------------------------------------

void  TTable::clearNodeSequence()
{
    // если не знаем индекс, то удаляем поля
    if( -1 == m_node_index )
    {
        m_node.remove( GoodsTableId );
        m_node.remove( GoodsTableName );
        m_node.remove( GoodsTableValue );
        m_node.remove( GoodsTableColumn );
        m_node.remove( GoodsTableRow );
        m_node.remove( GoodsTableLink );
    }

    // если знаем индекс, то удаляем всю ветку
    if( -1 != m_node_index )
    {
        m_node_parent.remove( m_node_index );
    }
}

void  TTable::TableDelete()
{
    QLayoutItem *child;

    // уничтожаем виджеты
    while( ( child = m_grid->takeAt(0) ) != Q_NULLPTR )
    {
        delete child->widget();
        delete child;
    }

    // уничтожаем grid
    m_grid->deleteLater();

    // удаляем себя из списка родителя
    if( Q_NULLPTR != m_pAncestor )
    {
        for( int i = 0; i < m_pAncestor->m_apTableList.count(); i++ )
        {
            if( this == m_pAncestor->m_apTableList.at(i) )
            {
                //qDebug() << m_pAncestor->m_apTableList.at(i)->getTableName() << "obsolete";

                m_pAncestor->m_apTableList.removeAt(i);

                break;
            }
        }

        int  index;

        // делаем переиндексацию оставшихся детей
        for( int i = 0; i < m_pAncestor->m_apTableList.count(); i++ )
        {
            index = m_pAncestor->m_apTableList.at(i)->getNodeIndex();

            if( index > m_node_index )
            {
                index -= 1;

                m_pAncestor->m_apTableList.at(i)->setNodeIndex( index );
            }
        }

        // удаляемся из родительского layout-а
        m_pAncestor->m_vlayout->removeWidget(this);
    }

    // очищаем ветку
    clearNodeSequence();
}

//------------------------------------------------------------------------------

void  TTable::setTableType( unsigned type ) noexcept
{
    m_uTableType = type;

    if( TValues::keTypeLink == m_uTableType )
    {
        // диалог для ссылки
        m_ptTabDialogLink = new TTabDialog( false, "Table", this, "Link" );

        // ловим сигнал от диалога об отмене
        connect( m_ptTabDialogLink, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

        // ловим сигнал от диалога с данными
        connect( m_ptTabDialogLink, &TTabDialog::sendValue, this, &TTable::onSendValue );

        m_ptBtnLink = new QPushButton( "link" );
        connect( m_ptBtnLink, &QPushButton::clicked, this, &TTable::onBtnLink );
        m_grid->addWidget( m_ptBtnLink, m_row, 2, Qt::AlignLeft );

        // эта кнопка на второй строке, высоту этой строки учли в конструкторе класса
        //widget_stretch( m_grid->minimumSize().width(), m_ptBtnLink->minimumSizeHint().height() );
    }
    else if( TValues::keTypeColumn == m_uTableType )
    {
        // диалог для новой записи
        m_ptTabDialogAddEntry = new TTabDialog( "Add column", this );

        // ловим сигнал от диалога об отмене
        connect( m_ptTabDialogAddEntry, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

        // ловим сигнал от диалога с данными
        connect( m_ptTabDialogAddEntry, &TTabDialog::sendValues, this, &TTable::onSendEntry );

        m_grid->addLayout( m_hlayout, 1, 1, Qt::AlignLeft );
    }
    else if( TValues::keTypeRow == m_uTableType )
    {
        // диалог для новой записи
        m_ptTabDialogAddEntry = new TTabDialog( "Add row", this );

        // ловим сигнал от диалога об отмене
        connect( m_ptTabDialogAddEntry, &TTabDialog::sendCancel, this, &TTable::onSendCancel );

        // ловим сигнал от диалога с данными
        connect( m_ptTabDialogAddEntry, &TTabDialog::sendValues, this, &TTable::onSendEntry );

        m_grid->addLayout( m_vlayout, 1, 1, Qt::AlignLeft );
    }
}

//------------------------------------------------------------------------------

void  TTable::setNode( const YAML::Node&  node )
{
    m_node = node;
}

void  TTable::setNodeParent( const YAML::Node&  node )
{
    m_node_parent = node;
}

void  TTable::setNodeIndex( int  index )
{
    m_node_index = index;
}

YAML::Node&  TTable::getNode()
{
    return m_node;
}

YAML::Node&  TTable::getNodeParent()
{
    return m_node_parent;
}

int  TTable::getNodeIndex()
{
    return m_node_index;
}

//------------------------------------------------------------------------------

void  TTable::setTableId( const std::string&  name, bool  set_to_node )
{
    int  height;

    m_zId = QString::fromStdString(name);

    height = m_ptBtnId->minimumSizeHint().height();

    m_zBtnId = QString::fromStdString(name);
    m_zBtnId.replace( QRegExp("[ ]{2,}"), " " );     // убираем подряд идущие пробелы на один
    m_zBtnId.replace( " ", "\n" );                   // заменяем пробелы на перевод строки
    m_ptBtnId->setText( m_zBtnId );                  // правленное имя кнопки
    m_ptBtnId->setToolTip( "Таблица: " + m_zId );    // подсказка с оригинальным именем

    height = m_ptBtnId->minimumSizeHint().height() - height;

    if( 0 < height )
    {
        widget_stretch( 0, height, false );
    }
    else
    {
        widget_shrink( 0, -1 * height );
    }

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsTableId, name );
    }
}

const QString TTable::getTableId()
{
    return m_zId;
}

void  TTable::setTableName( const std::string&  name, bool  set_to_node )
{
    int  height;

    m_zName = QString::fromStdString(name);

    height = m_ptBtnName->minimumSizeHint().height();

    m_zBtnName = QString::fromStdString(name);
    m_zBtnName.replace( QRegExp("[ ]{2,}"), " " );       // убираем подряд идущие пробелы на один
    m_zBtnName.replace( " ", "\n" );                     // заменяем пробелы на перевод строки
    m_ptBtnName->setText( m_zBtnName );                  // правленное имя кнопки
    m_ptBtnName->setToolTip( "Таблица: " + m_zName );    // подсказка с оригинальным именем

    height = m_ptBtnName->minimumSizeHint().height() - height;

    if( 0 < height )
    {
        widget_stretch( 0, height, false );
    }
    else
    {
        widget_shrink( 0, -1 * height );
    }

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsTableName, name );
    }
}

const QString TTable::getTableName()
{
    return m_zName;
}

void  TTable::setTableLink( const std::string&  name, bool  set_to_node )
{
    m_zLink = QString::fromStdString(name);

    m_ptBtnLink->setText( m_zLink );
    m_ptBtnLink->setToolTip( "Внешняя ссылка" );

    if( set_to_node )
    {
        __yaml_SetString( m_node, GoodsTableLink, name );
    }
}

const QString TTable::getTableLink()
{
    return m_zLink;
}

//------------------------------------------------------------------------------

void TTable::setTableEntryValue( TTableEntry  *pEntry, QString&  value, int  index )
{
    TTableEntryValue  *pEntryValue;

    // новая запись
    pEntryValue = new TTableEntryValue( pEntry );
    pEntryValue->setEntryValue( value );
    pEntryValue->setValueIndex( index );

    // ловим сигнал с данными значения
    connect( pEntryValue, &TTableEntryValue::sendEntryValue, pEntry, &TTableEntry::onSendEntryValue );

    // добавляем запись в список
    pEntry->m_apValueList.append( pEntryValue );
}

void TTable::setTableRow( const std::string&  name, QStringList& list )
{
    TTableEntry  *pEntry = m_apTabEntryList.last();
    int column = 0;

    // кнопка плюс для добавления значения
    QPushButton *ptBtnRowValInc = new QPushButton( "+" );
    ptBtnRowValInc->setToolTip( "Добавить значение" );
    ptBtnRowValInc->setFixedWidth( 93 );
    //connect( ptBtnRowValInc, &QPushButton::clicked, this, &TTable::onBtnRowValInc );
    connect( ptBtnRowValInc, &QPushButton::clicked, pEntry, &TTableEntry::onBtnInc );
    pEntry->m_hlayout->addWidget( ptBtnRowValInc, 0, Qt::AlignLeft );

    column += 1;

    // кнопка с именем строки
    QPushButton *ptBtnRowName = new QPushButton( QString::fromStdString(name) );
    ptBtnRowName->setToolTip( "Строка: " + QString::fromStdString(name) );
    ptBtnRowName->setFixedWidth( 93 );
    connect( ptBtnRowName, &QPushButton::clicked, pEntry, &TTableEntry::onBtnName );
    connect( pEntry, &TTableEntry::sendName, this, &TTable::onBtnRowName );
    connect( pEntry, &TTableEntry::sendName, ptBtnRowName, &QPushButton::setText );
    pEntry->m_hlayout->addWidget( ptBtnRowName, 0, Qt::AlignLeft );

    column += 1;

    int i = 0;

    for( i = 0; i < static_cast<int>(list.size()); i++ )
    {
        // кнопки со значениями
        QPushButton  *button = new QPushButton( list[i] );
        button->setToolTip( "Значение: " + list[i] );
        button->setFixedWidth( 93 );
        setTableEntryValue( pEntry, list[i], i );
        connect( button, &QPushButton::clicked, pEntry->m_apValueList.last(), &TTableEntryValue::onBtnValue );
        //connect( button, &QPushButton::clicked, this, &TTable::onBtnRowValName );
        connect( pEntry->m_apValueList.last(), &TTableEntryValue::sendEntryValue, button, &QPushButton::setText );
        pEntry->m_hlayout->addWidget( button, 0, Qt::AlignLeft );

        column += 1;
    }

    // кнопка плюс для добавления строки
    QPushButton *ptBtnRowInc = new QPushButton( "+" );
    ptBtnRowInc->setToolTip( "Добавить строку" );
    ptBtnRowInc->setFixedWidth( 93 );
    connect( ptBtnRowInc, &QPushButton::clicked, this, &TTable::onBtnRowInc );
    pEntry->m_hlayout->addWidget( ptBtnRowInc, 0, Qt::AlignLeft );

    column += 1;

    m_vlayout->addLayout( pEntry->m_hlayout );

    int  width = ( 4 + i ) * ptBtnRowValInc->minimumSize().width() + ( 3 + i ) * m_grid->spacing();

    // подгоняем ширину
    //widget_stretch( m_grid->minimumSize().width() + i * m_grid->spacing(), 0 );
    widget_stretch( width, 0 );

    nextRow();
}

void TTable::setTableColumn( const std::string&  name, QStringList& list )
{
    TTableEntry  *pEntry = m_apTabEntryList.last();
    int row = 0;

    // кнопка плюс для добавления значения
    QPushButton *ptBtnColumnValInc = new QPushButton( "+" );
    ptBtnColumnValInc->setToolTip( "Добавить значение" );
    ptBtnColumnValInc->setFixedWidth( 93 );
    //connect( ptBtnColumnValInc, &QPushButton::clicked, this, &TTable::onBtnColumnValInc );
    connect( ptBtnColumnValInc, &QPushButton::clicked, pEntry, &TTableEntry::onBtnInc );
    pEntry->m_vlayout->addWidget( ptBtnColumnValInc, 0, Qt::AlignLeft );

    row += 1;

    // кнопка с именем столбца
    QPushButton *ptBtnColumnName = new QPushButton( QString::fromStdString(name) );
    ptBtnColumnName->setToolTip( "Столбец: " + QString::fromStdString(name) );
    ptBtnColumnName->setFixedWidth( 93 );
    connect( ptBtnColumnName, &QPushButton::clicked, pEntry, &TTableEntry::onBtnName );
    connect( pEntry, &TTableEntry::sendName, this, &TTable::onBtnColumnName );
    connect( pEntry, &TTableEntry::sendName, ptBtnColumnName, &QPushButton::setText );
    pEntry->m_vlayout->addWidget( ptBtnColumnName, 0, Qt::AlignLeft );

    row += 1;

    for( int i = 0; i < static_cast<int>(list.size()); i++ )
    {
        // кнопки со значениями
        QPushButton  *button = new QPushButton( list[i] );
        button->setToolTip( "Значение: " + list[i] );
        button->setFixedWidth( 93 );
        setTableEntryValue( pEntry, list[i], i );
        connect( button, &QPushButton::clicked, pEntry->m_apValueList.last(), &TTableEntryValue::onBtnValue );
        //connect( button, &QPushButton::clicked, this, &TTable::onBtnColumnValName );
        connect( pEntry->m_apValueList.last(), &TTableEntryValue::sendEntryValue, button, &QPushButton::setText );
        pEntry->m_vlayout->addWidget( button, 0, Qt::AlignLeft );

        row += 1;
    }

    // кнопка плюс для добавления строки
    QPushButton *ptBtnColumnInc = new QPushButton( "+" );
    ptBtnColumnInc->setToolTip( "Добавить столбец" );
    ptBtnColumnInc->setFixedWidth( 93 );
    connect( ptBtnColumnInc, &QPushButton::clicked, this, &TTable::onBtnColumnInc );
    pEntry->m_vlayout->addWidget( ptBtnColumnInc, 0, Qt::AlignLeft );

    m_hlayout->addLayout( pEntry->m_vlayout );

    //widget_stretch( m_grid->minimumSize().width(), label->minimumSize().height() + m_grid->spacing() );

    nextColumn();
}

//------------------------------------------------------------------------------

void TTable::fixTableHeight( unsigned  row_count )
{
    widget_stretch( 0, static_cast<int>(row_count) * ( m_ptBtnInc->minimumSizeHint().height() + m_grid->spacing() ) );
}

//------------------------------------------------------------------------------

void TTable::resetRow() noexcept
{
    m_row = 0;
}

void TTable::resetColumn() noexcept
{
    m_column = 0;
}


void TTable::nextRow() noexcept
{
    m_row += 1;
}

void TTable::nextColumn() noexcept
{
    m_column += 1;
}

//------------------------------------------------------------------------------

void  TTable::widget_size_reset() noexcept
{
    m_width = 0;
    m_height = 0;
}

void  TTable::widget_stretch( int width, int height, bool add_height ) noexcept
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

void  TTable::widget_parent_stretch( int width, int height, bool add_height ) noexcept
{
    int  val = 0;

    if( Q_NULLPTR != m_pAncestor )
    {
        if( add_height )
        {
            val = m_pAncestor->m_vlayout->spacing();
        }

        m_pAncestor->widget_stretch( width, height + val );
    }
}

void  TTable::widget_shrink( int width, int height ) noexcept
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

void  TTable::widget_parent_shrink( int width, int height ) noexcept
{
    if( Q_NULLPTR != m_pAncestor )
    {
        m_pAncestor->widget_shrink( width, height );
    }
}

int TTable::getTableWidth() noexcept
{
    return minimumSize().width();
}

int TTable::getTableHeight() noexcept
{
    return minimumSize().height();
}

//------------------------------------------------------------------------------
