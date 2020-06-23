#include "dialog.h"
#include "mainwindow.h"

//------------------------------------------------------------------------------

class TPrivDialog
{
    friend class  TDialog;
public:

    QLineEdit        *m_ptLineName;
    QSpinBox         *m_ptSpinType;
    QComboBox        *m_ptBoxType;
    QLineEdit        *m_ptLinePlaceholder;
    QLineEdit        *m_ptLineNew;
    QLineEdit        *m_ptLineAfter;
    QLineEdit        *m_ptLineBefore;
    QComboBox        *m_ptBoxUlink;
    QLineEdit        *m_ptLineUname;
    QSpinBox         *m_ptSpinMin;
    QSpinBox         *m_ptSpinMax;
    QDoubleSpinBox   *m_ptDSpinMin;
    QDoubleSpinBox   *m_ptDSpinMax;
    QComboBox        *m_ptLineMulti;
    QComboBox        *m_ptComboList;
    QLineEdit        *m_ptLineLoadLink;

    QDialogButtonBox *m_ptBtnBox;

    QGridLayout      *m_grid;

    TValues           m_tValues;

    inline TPrivDialog()
    {
        m_grid = new QGridLayout;
        m_ptSpinType = NULL;
        m_ptBoxType = NULL;
    }
    ~TPrivDialog()
    {
        m_grid->deleteLater();
    }

};

static QStringList  s_azMultiValues = 
{ 
    "both", "seller", "buyer" 
};

//------------------------------------------------------------------------------

TDialog::TDialog( bool fullsize, QString name, QWidget *parent )
    : QDialog( parent )
{
    int  row = 0;

    m_bFullSize = fullsize;

    // заголовок формы
    setWindowTitle("YAML Visualizer");

    // иконка формы
    setWindowIcon( QIcon( ":/favicon.ico" ) );

    // цепляем местный сигнал к слоту MainWindow
    connect( this, &TDialog::sendChanged, MainWindow::getMainWinPtr(), &MainWindow::onYamlChanged );

    row_min = 0;
    row_max = 0;

    priv__ = std::unique_ptr<TPrivDialog>(new TPrivDialog);

    QLabel *title = new QLabel( name, this );
    title->setMinimumWidth( 93 );
    priv__->m_grid->addWidget( title, row, 0, 1, 1 );
    row++;

    QLabel *lblName = new QLabel( QString( "%1%2" ).arg( tr( "Name" )).arg(":"), this );
    priv__->m_grid->addWidget( lblName, row, 0, 1, 1 );
    QLineEdit *ptLineName = new QLineEdit( this );
    priv__->m_ptLineName = ptLineName;
    priv__->m_grid->addWidget( ptLineName, row, 1, 1, 1 );
    row++;

    if( m_bFullSize )
    {
        QLabel *lblType = new QLabel( QString( "%1%2" ).arg( tr( "Type" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblType, row, 0, 1, 1 );
#if 0
        QSpinBox *ptSpinType = new QSpinBox( this );
        ptSpinType->setMinimum( 0 );
        ptSpinType->setMaximum( 5 );
        priv__->m_ptSpinType = ptSpinType;
        priv__->m_grid->addWidget( ptSpinType, row, 1, 1, 1 );
        connect( ptSpinType, QOverload<int>::of(&QSpinBox::valueChanged), this, &TDialog::onTypeChanged );
#else
        QComboBox   *ptBoxType = new QComboBox( this );
        ptBoxType->addItems( QStringList() << "0 -- пустое поле" 
                << "1 -- строка" 
                << "2 -- целое" 
                << "3 -- дробное" 
                << "4 -- список"
                << "5 -- выбор" );
        priv__->m_ptBoxType = ptBoxType;
        priv__->m_grid->addWidget( ptBoxType, row, 1, 1, 1 );
        connect( ptBoxType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TDialog::onTypeChanged );
#endif
        row++;

        QLabel *lblPlaceholder = new QLabel( QString( "%1%2" ).arg( tr( "Placeholder" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblPlaceholder, row, 0, 1, 1 );
        QLineEdit *ptLinePlaceholder = new QLineEdit( this );
        priv__->m_ptLinePlaceholder = ptLinePlaceholder;
        priv__->m_grid->addWidget( ptLinePlaceholder, row, 1, 1, 1 );
        row++;

        QLabel *lblNew = new QLabel( QString( "%1%2" ).arg( tr( "New" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblNew, row, 0, 1, 1 );
        QLineEdit *ptLineNew = new QLineEdit( this );
        priv__->m_ptLineNew = ptLineNew;
        priv__->m_grid->addWidget( ptLineNew, row, 1, 1, 1 );
        row++;

        QLabel *lblAfter = new QLabel( QString( "%1%2" ).arg( tr( "After" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblAfter, row, 0, 1, 1 );
        QLineEdit *ptLineAfter = new QLineEdit( this );
        priv__->m_ptLineAfter = ptLineAfter;
        priv__->m_grid->addWidget( ptLineAfter, row, 1, 1, 1 );
        row++;

        QLabel *lblBefore = new QLabel( QString( "%1%2" ).arg( tr( "Before" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblBefore, row, 0, 1, 1 );
        QLineEdit *ptLineBefore = new QLineEdit( this );
        priv__->m_ptLineBefore = ptLineBefore;
        priv__->m_grid->addWidget( ptLineBefore, row, 1, 1, 1 );
        row++;
    }

    QLabel *lblUlink = new QLabel( QString( "%1%2" ).arg( tr( "Ulink" ) ).arg(":"), this );
    priv__->m_grid->addWidget( lblUlink, row, 0, 1, 1 );
    QComboBox *ptBoxUlink = new QComboBox( this );
    priv__->m_ptBoxUlink = ptBoxUlink;
    priv__->m_grid->addWidget( ptBoxUlink, row, 1, 1, 1 );
    row++;

    QLabel *lblUname = new QLabel( QString( "%1%2" ).arg( tr( "Uname" ) ).arg(":"), this );
    priv__->m_grid->addWidget( lblUname, row, 0, 1, 1 );
    QLineEdit *ptLineUname = new QLineEdit( this );
    priv__->m_ptLineUname = ptLineUname;
    priv__->m_grid->addWidget( ptLineUname, row, 1, 1, 1 );
    row++;

    if( m_bFullSize )
    {
        QLabel *lblMin = new QLabel( QString( "%1%2" ).arg( tr( "Min" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblMin, row, 0, 1, 1 );
        QSpinBox *ptSpinMin = new QSpinBox( this );
        ptSpinMin->setMinimum( -999 );
        ptSpinMin->setMaximum( 999 );
        priv__->m_ptSpinMin = ptSpinMin;
        priv__->m_grid->addWidget( ptSpinMin, row, 1, 1, 1 );
        row_min = row;
        row++;

        QDoubleSpinBox *ptDSpinMin = new QDoubleSpinBox( this );
        ptDSpinMin->setMinimum( -999 );
        ptDSpinMin->setMaximum( 999 );
        priv__->m_ptDSpinMin = ptDSpinMin;
        priv__->m_ptDSpinMin->hide();

        QLabel *lblMax = new QLabel( QString( "%1%2" ).arg( tr( "Max" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblMax, row, 0, 1, 1 );
        QSpinBox *ptSpinMax = new QSpinBox( this );
        ptSpinMax->setMinimum( -999 );
        ptSpinMax->setMaximum( 999 );
        priv__->m_ptSpinMax = ptSpinMax;
        priv__->m_grid->addWidget( ptSpinMax, row, 1, 1, 1 );
        row_max = row;
        row++;

        QDoubleSpinBox *ptDSpinMax = new QDoubleSpinBox( this );
        ptDSpinMax->setMinimum( -999 );
        ptDSpinMax->setMaximum( 999 );
        priv__->m_ptDSpinMax = ptDSpinMax;
        priv__->m_ptDSpinMax->hide();

        QLabel *lblMulti = new QLabel( QString( "%1%2" ).arg( tr( "Multi" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblMulti, row, 0, 1, 1 );
        QComboBox *ptLineMulti = new QComboBox( this );
        ptLineMulti->addItems( QStringList() << tr( "both" ) << tr( "seller" ) << tr( "buyer" ) );
        priv__->m_ptLineMulti = ptLineMulti;
        priv__->m_grid->addWidget( ptLineMulti, row, 1, 1, 1 );
        row++;

        QLabel *lblList = new QLabel( QString( "%1%2" ).arg( tr( "Values" ) ).arg(":"), this );
        priv__->m_grid->addWidget( lblList, row, 0, 1, 1 );
        QComboBox *ptComboList = new QComboBox( this );
        priv__->m_ptComboList = ptComboList;
        priv__->m_grid->addWidget( ptComboList, row, 1, 1, 1 );
        row++;

        QPushButton *btnLoadFrom = new QPushButton( QString( "%1%2" ).arg( tr( "Load From" ) ).arg(":"), this );
        connect( btnLoadFrom, &QPushButton::clicked, this, &TDialog::onLoadFrom );
        priv__->m_grid->addWidget( btnLoadFrom, row, 0, 1, 1 );
        QLineEdit *ptLineLoadLink = new QLineEdit( this );
        priv__->m_ptLineLoadLink = ptLineLoadLink;
        priv__->m_grid->addWidget( ptLineLoadLink, row, 1, 1, 1 );
        row++;

    }

    QLabel *empty = new QLabel( this );
    priv__->m_grid->addWidget( empty, row, 0, 1, 2 );
    row++;

    // создаем диалоговые кнопки
    QDialogButtonBox  *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );

    //connect( box, &QDialogButtonBox::accepted, this, &TDialog::accepted );
    //connect( box, &QDialogButtonBox::rejected, this, &TDialog::rejected );

    connect( box, &QDialogButtonBox::accepted, this, &TDialog::close );
    connect( box, &QDialogButtonBox::rejected, this, &TDialog::close );

    connect( box, &QDialogButtonBox::clicked, this, &TDialog::onBtnAction );

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->m_grid->addWidget( box, row, 0, 1, 2 );
    row++;

    setLayout( priv__->m_grid );

    setMinimumSize( priv__->m_grid->minimumSize().width(), priv__->m_grid->minimumSize().height() );
}

TDialog::~TDialog()
{

}

//------------------------------------------------------------------------------

void  TDialog::setDlgName( const QString& name )
{
    priv__->m_ptLineName->setText( name );
}

void  TDialog::setDlgPlaceholder( const QString&  name )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLinePlaceholder->setText( name );
}

void  TDialog::setDlgNew( const QString&  name )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineNew->setText( name );
}

void  TDialog::setDlgAfter( const QString&  name )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineAfter->setText( name );
}

void  TDialog::setDlgBefore( const QString&  name )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineBefore->setText( name );
}

void  TDialog::setDlgUlink( const QString&  name )
{
    int     index = priv__->m_ptBoxUlink->findText( name );

    if( index > 0 )
        priv__->m_ptBoxUlink->setCurrentIndex( index );
    else
        priv__->m_ptBoxUlink->setItemText( priv__->m_ptBoxUlink->currentIndex(), name );
}

void  TDialog::setDlgUlink( const QStringList&  list )
{
    priv__->m_ptBoxUlink->clear();
    priv__->m_ptBoxUlink->addItems( list );
}

void  TDialog::setDlgUname( const QString&  name )
{
    priv__->m_ptLineUname->setText( name );
}

void  TDialog::setDlgMulti( const QString&  name )
{
    if( !m_bFullSize )
        return;

    int     index = s_azMultiValues.indexOf( name );
    if( index >= 0 )
        priv__->m_ptLineMulti->setCurrentIndex( index );
}

void  TDialog::setDlgLoadFrom( const QString&  name )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineLoadLink->setText( name );
}

void  TDialog::setDlgCombo( QStringList  list )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptComboList->clear();

    for( auto& it : list )
    {
        priv__->m_ptComboList->addItem( it );
    }
}

void  TDialog::setDlgType( unsigned  val )
{
    if( !m_bFullSize )
        return;

    //priv__->m_ptSpinType->setValue( static_cast<int>(val) );
    priv__->m_ptBoxType->setCurrentIndex( static_cast<int>(val) );

    setDlgEnabledByType( val );

    setDlgShowByType( val );
}

void  TDialog::setDlgMin( int  val )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptSpinMin->setValue( val );
}

void  TDialog::setDlgMax( int  val )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptSpinMax->setValue( val );
}

void  TDialog::setDlgDMin( double  val )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptDSpinMin->setValue( val );
}

void  TDialog::setDlgDMax( double  val )
{
    if( !m_bFullSize )
        return;

    priv__->m_ptDSpinMax->setValue( val );
}

//------------------------------------------------------------------------------

void  TDialog::setDlgEmpty()
{
    int  val;
    QString  name;

    val = 0;
    name.clear();

    priv__->m_ptLineName->setText( name );

    priv__->m_ptBoxUlink->setCurrentIndex( 0 );
    priv__->m_ptLineUname->setText( name );

    if( m_bFullSize )
    {
        priv__->m_ptLinePlaceholder->setText( name );
        priv__->m_ptLineNew->setText( name );
        priv__->m_ptLineAfter->setText( name );
        priv__->m_ptLineBefore->setText( name );
        priv__->m_ptLineMulti->setCurrentIndex( 0 );
        priv__->m_ptLineLoadLink->setText( name );

        //priv__->m_ptSpinType->setValue( val );
        priv__->m_ptBoxType->setCurrentIndex( val );

        priv__->m_ptSpinMin->setValue( val );
        priv__->m_ptSpinMax->setValue( val );

        priv__->m_ptDSpinMin->setValue( 0.0 );
        priv__->m_ptDSpinMax->setValue( 0.0 );

        priv__->m_ptComboList->clear();
    }
}

void  TDialog::setDlgEnabled( bool enabled ) noexcept
{
    setDlgTypeEnabled( enabled );
    setDlgPlaceholderEnabled( enabled );
    setDlgNewEnabled( enabled );
    setDlgAfterEnabled( enabled );
    setDlgBeforeEnabled( enabled );
    setDlgUlinkEnabled( enabled );
    setDlgUnameEnabled( enabled );
    setDlgMultiEnabled( enabled );
    setDlgLoadFromEnabled( enabled );
    setDlgMinEnabled( enabled );
    setDlgMaxEnabled( enabled );
    setDlgComboEnabled( enabled );
}

void  TDialog::setDlgTypeEnabled( bool enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    //priv__->m_ptSpinType->setEnabled( enabled );
    priv__->m_ptBoxType->setEnabled( enabled );
}

void  TDialog::setDlgPlaceholderEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLinePlaceholder->setEnabled( enabled );
}

void  TDialog::setDlgNewEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineNew->setEnabled( enabled );
}

void  TDialog::setDlgAfterEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineAfter->setEnabled( enabled );
}

void  TDialog::setDlgBeforeEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineBefore->setEnabled( enabled );
}

void  TDialog::setDlgUlinkEnabled( bool  enabled ) noexcept
{
    priv__->m_ptBoxUlink->setEnabled( enabled );
}

void  TDialog::setDlgUnameEnabled( bool  enabled ) noexcept
{
    priv__->m_ptLineUname->setEnabled( enabled );
}

void  TDialog::setDlgMultiEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineMulti->setEnabled( enabled );
}

void  TDialog::setDlgLoadFromEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptLineLoadLink->setEnabled( enabled );
}

void  TDialog::setDlgMinEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptSpinMin->setEnabled( enabled );
    priv__->m_ptDSpinMin->setEnabled( enabled );
}

void  TDialog::setDlgMaxEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptSpinMax->setEnabled( enabled );
    priv__->m_ptDSpinMax->setEnabled( enabled );
}

void  TDialog::setDlgComboEnabled( bool  enabled ) noexcept
{
    if( !m_bFullSize )
        return;

    priv__->m_ptComboList->setEnabled( enabled );
}

//------------------------------------------------------------------------------

void  TDialog::setDlgEnabledByType( unsigned  type ) noexcept
{
    setDlgPlaceholderEnabled( 0 != type );
    setDlgNewEnabled( 0 != type );
    setDlgAfterEnabled( 0 != type );
    setDlgBeforeEnabled( 0 != type );
    setDlgUlinkEnabled( 0 != type );
    setDlgUnameEnabled( 0 != type );
    setDlgMultiEnabled( 5 == type );
    setDlgLoadFromEnabled( ( 4 == type ) || ( 5 == type ) );
    setDlgMinEnabled( ( 2 == type ) || ( 3 == type ) );
    setDlgMaxEnabled( ( 2 == type ) || ( 3 == type ) );
    setDlgComboEnabled( ( 4 == type ) || ( 5 == type ) );
}

void  TDialog::setDlgShowByType( unsigned  type ) noexcept
{
    if( !m_bFullSize )
        return;

    if( 3 == type )
    {
        priv__->m_ptSpinMin->hide();
        priv__->m_ptSpinMax->hide();

        priv__->m_grid->removeWidget( priv__->m_ptSpinMin );
        priv__->m_grid->removeWidget( priv__->m_ptSpinMax );

        priv__->m_grid->addWidget( priv__->m_ptDSpinMin, row_min, 1, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptDSpinMax, row_max, 1, 1, 1 );

        priv__->m_ptDSpinMin->show();
        priv__->m_ptDSpinMax->show();
    }
    else
    {
        priv__->m_ptDSpinMin->hide();
        priv__->m_ptDSpinMax->hide();

        priv__->m_grid->removeWidget( priv__->m_ptDSpinMin );
        priv__->m_grid->removeWidget( priv__->m_ptDSpinMax );

        priv__->m_grid->addWidget( priv__->m_ptSpinMin, row_min, 1, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptSpinMax, row_max, 1, 1, 1 );

        priv__->m_ptSpinMin->show();
        priv__->m_ptSpinMax->show();
    }
}

//------------------------------------------------------------------------------

void  TDialog::onTypeChanged( int  type )
{
    setDlgEnabledByType( static_cast<unsigned>(type) );

    setDlgShowByType( static_cast<unsigned>(type) );
}

void  TDialog::onBtnAction( QAbstractButton*  btn )
{
    // нажали Ok
    if( QDialogButtonBox::AcceptRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        //qDebug() << "dialog Ok" << priv__->m_ptLineName->text();

        priv__->m_tValues.m_zName = priv__->m_ptLineName->text();

        priv__->m_tValues.m_zUlink = priv__->m_ptBoxUlink->currentText();
        priv__->m_tValues.m_zUname = priv__->m_ptLineUname->text();

        if( m_bFullSize )
        {
            priv__->m_tValues.m_zPlaceholder = priv__->m_ptLinePlaceholder->text();
            priv__->m_tValues.m_zNew = priv__->m_ptLineNew->text();
            priv__->m_tValues.m_zAfter = priv__->m_ptLineAfter->text();
            priv__->m_tValues.m_zBefore = priv__->m_ptLineBefore->text();
            priv__->m_tValues.m_zMulti = s_azMultiValues[ priv__->m_ptLineMulti->currentIndex() ];
            priv__->m_tValues.m_zLoadLink = priv__->m_ptLineLoadLink->text();

            //priv__->m_tValues.m_uType = static_cast<unsigned>(priv__->m_ptSpinType->value());
            priv__->m_tValues.m_uType = static_cast<unsigned>(priv__->m_ptBoxType->currentIndex());

            priv__->m_tValues.m_nMin = priv__->m_ptSpinMin->value();
            priv__->m_tValues.m_nMax = priv__->m_ptSpinMax->value();

            priv__->m_tValues.m_fMin = priv__->m_ptDSpinMin->value();
            priv__->m_tValues.m_fMax = priv__->m_ptDSpinMax->value();
        }
        else
        {
            priv__->m_tValues.m_zPlaceholder.clear();
            priv__->m_tValues.m_zNew.clear();
            priv__->m_tValues.m_zAfter.clear();
            priv__->m_tValues.m_zBefore.clear();
            priv__->m_tValues.m_zMulti.clear();
            priv__->m_tValues.m_zLoadLink.clear();

            priv__->m_tValues.m_uType = 0;

            priv__->m_tValues.m_nMin = 0;
            priv__->m_tValues.m_nMax = 0;

            priv__->m_tValues.m_fMin = 0;
            priv__->m_tValues.m_fMax = 0;
        }

        // шлем сигнал с данными
        Q_EMIT sendValues( priv__->m_tValues );

        // шлем сигнал что мы что-то поменяли
        Q_EMIT sendChanged();
    }

    // нажали Reset
    if( QDialogButtonBox::ResetRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        //qDebug() << "dialog Reset" << priv__->m_ptLineName->text();
    }

    // нажали Cancel
    if( QDialogButtonBox::RejectRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        //qDebug() << "dialog Cancel" << priv__->m_ptLineName->text();

        // шлем сигнал об отмене
        Q_EMIT sendCancel();
    }
}

void    TDialog::onLoadFrom()
{
    QString     url = priv__->m_ptLineLoadLink->text();
    if( ! url.isEmpty() )
        QDesktopServices::openUrl( QUrl::fromUserInput( url ) );
}

//------------------------------------------------------------------------------
