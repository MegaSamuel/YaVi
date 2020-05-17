#include "dialog.h"

//------------------------------------------------------------------------------

class TPrivDialog
{
    friend class  TDialog;
public:

    QLineEdit    *m_ptLineName;
    QSpinBox     *m_ptSpinType;
    QLineEdit    *m_ptLinePlaceholder;
    QLineEdit    *m_ptLineNew;
    QLineEdit    *m_ptLineAfter;
    QLineEdit    *m_ptLineBefore;
    QLineEdit    *m_ptLineUlink;
    QLineEdit    *m_ptLineUname;
    QSpinBox     *m_ptSpinMin;
    QSpinBox     *m_ptSpinMax;
    QLineEdit    *m_ptLineMulti;
    QComboBox    *m_ptComboList;

    QDialogButtonBox  *m_ptBtnBox;

    QGridLayout  *m_grid;

    TValues       m_tValues;

    inline TPrivDialog()
    {
        m_grid = new QGridLayout;

        m_ptLineName = new QLineEdit;
        m_ptLinePlaceholder = new QLineEdit;
        m_ptLineNew = new QLineEdit;
        m_ptLineAfter = new QLineEdit;
        m_ptLineBefore = new QLineEdit;
        m_ptLineUlink = new QLineEdit;
        m_ptLineUname = new QLineEdit;
        m_ptLineMulti = new QLineEdit;

        m_ptSpinType = new QSpinBox;
        m_ptSpinType->setMinimum( 0 );
        m_ptSpinType->setMaximum( 5 );

        m_ptSpinMin = new QSpinBox;
        m_ptSpinMax = new QSpinBox;

        m_ptComboList = new QComboBox;
    }
    ~TPrivDialog()
    {
        m_grid->deleteLater();
    }

};

//------------------------------------------------------------------------------

TDialog::TDialog( bool fullsize, QString name, QWidget *parent )
    : QDialog( parent )
{
    int  row = 0;

    // заголовок формы
    setWindowTitle("YAML Visualizer");

    // иконка формы
    setWindowIcon( QIcon( ":/favicon.ico" ) );

    priv__ = std::unique_ptr<TPrivDialog>(new TPrivDialog);

    QLabel *title = new QLabel( name, this );
    title->setMinimumWidth( 93 );
    priv__->m_grid->addWidget( title, row, 0, 1, 1 );
    row++;

    QLabel *lblName = new QLabel( QString( "%1%2" ).arg("Name").arg(":"), this );
    priv__->m_grid->addWidget( lblName, row, 0, 1, 1 );
    priv__->m_grid->addWidget( priv__->m_ptLineName, row, 1, 1, 1 );
    row++;

    //priv__->m_ptLineName->setFocus();

    if( fullsize )
    {
        QLabel *lblType = new QLabel( QString( "%1%2" ).arg("Type").arg(":"), this );
        priv__->m_grid->addWidget( lblType, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptSpinType, row, 1, 1, 1 );
        row++;

        QLabel *lblPlaceholder = new QLabel( QString( "%1%2" ).arg("Placeholder").arg(":"), this );
        priv__->m_grid->addWidget( lblPlaceholder, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLinePlaceholder, row, 1, 1, 1 );
        row++;

        QLabel *lblNew = new QLabel( QString( "%1%2" ).arg("New").arg(":"), this );
        priv__->m_grid->addWidget( lblNew, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLineNew, row, 1, 1, 1 );
        row++;

        QLabel *lblAfter = new QLabel( QString( "%1%2" ).arg("After").arg(":"), this );
        priv__->m_grid->addWidget( lblAfter, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLineAfter, row, 1, 1, 1 );
        row++;

        QLabel *lblBefore = new QLabel( QString( "%1%2" ).arg("Before").arg(":"), this );
        priv__->m_grid->addWidget( lblBefore, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLineBefore, row, 1, 1, 1 );
        row++;

        QLabel *lblUlink = new QLabel( QString( "%1%2" ).arg("Ulink").arg(":"), this );
        priv__->m_grid->addWidget( lblUlink, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLineUlink, row, 1, 1, 1 );
        row++;

        QLabel *lblUname = new QLabel( QString( "%1%2" ).arg("Uname").arg(":"), this );
        priv__->m_grid->addWidget( lblUname, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLineUname, row, 1, 1, 1 );
        row++;

        QLabel *lblMin = new QLabel( QString( "%1%2" ).arg("Min").arg(":"), this );
        priv__->m_grid->addWidget( lblMin, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptSpinMin, row, 1, 1, 1 );
        row++;

        QLabel *lblMax = new QLabel( QString( "%1%2" ).arg("Max").arg(":"), this );
        priv__->m_grid->addWidget( lblMax, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptSpinMax, row, 1, 1, 1 );
        row++;

        QLabel *lblMulti = new QLabel( QString( "%1%2" ).arg("Multi").arg(":"), this );
        priv__->m_grid->addWidget( lblMulti, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptLineMulti, row, 1, 1, 1 );
        row++;

        QLabel *lblList = new QLabel( QString( "%1%2" ).arg("Values").arg(":"), this );
        priv__->m_grid->addWidget( lblList, row, 0, 1, 1 );
        priv__->m_grid->addWidget( priv__->m_ptComboList, row, 1, 1, 1 );
        row++;
    }

    QLabel *empty = new QLabel( this );
    priv__->m_grid->addWidget( empty, row, 0, 1, 2 );
    row++;

    // создаем диалоговые кнопки
    QDialogButtonBox  *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );

    connect( box, SIGNAL(accepted()), this, SIGNAL(accepted()) );
    connect( box, SIGNAL(rejected()), this, SIGNAL(rejected()) );
    connect( box, SIGNAL(accepted()), this, SLOT(close()) );
    connect( box, SIGNAL(rejected()), this, SLOT(close()) );
    connect( box, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onBtnAction(QAbstractButton*)) );

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->m_grid->addWidget( box, row, 0, 1, 2 );
    row++;

//    priv__->m_grid->setSizeConstraint( QLayout::SetFixedSize );
    setLayout( priv__->m_grid );
}

TDialog::~TDialog()
{
    priv__->~TPrivDialog();
}

//------------------------------------------------------------------------------

void  TDialog::setDlgName( const QString& name )
{
    priv__->m_ptLineName->setText( name );
}

void  TDialog::setDlgPlaceholder( const QString&  name )
{
    priv__->m_ptLinePlaceholder->setText( name );
}

void  TDialog::setDlgNew( const QString&  name )
{
    priv__->m_ptLineNew->setText( name );
}

void  TDialog::setDlgAfter( const QString&  name )
{
    priv__->m_ptLineAfter->setText( name );
}

void  TDialog::setDlgBefore( const QString&  name )
{
    priv__->m_ptLineBefore->setText( name );
}

void  TDialog::setDlgUlink( const QString&  name )
{
    priv__->m_ptLineUlink->setText( name );
}

void  TDialog::setDlgUname( const QString&  name )
{
    priv__->m_ptLineUname->setText( name );
}

void  TDialog::setDlgMulti( const QString&  name )
{
    priv__->m_ptLineMulti->setText( name );
}

void  TDialog::setDlgCombo( QStringList  list )
{
    priv__->m_ptComboList->clear();

    for( auto& it : list )
    {
        priv__->m_ptComboList->addItem( it );
    }
}

void  TDialog::setDlgType( unsigned  val )
{
    priv__->m_ptSpinType->setValue( static_cast<int>(val) );
}

void  TDialog::setDlgMin( unsigned  val )
{
    priv__->m_ptSpinMin->setValue( static_cast<int>(val) );
}

void  TDialog::setDlgMax( unsigned  val )
{
    priv__->m_ptSpinMax->setValue( static_cast<int>(val) );
}

//------------------------------------------------------------------------------

void  TDialog::setDlgEmpty()
{
    int  val;
    QString  name;

    val = 0;
    name.clear();

    priv__->m_ptLineName->setText( name );
    priv__->m_ptLinePlaceholder->setText( name );
    priv__->m_ptLineNew->setText( name );
    priv__->m_ptLineAfter->setText( name );
    priv__->m_ptLineBefore->setText( name );
    priv__->m_ptLineUlink->setText( name );
    priv__->m_ptLineUname->setText( name );
    priv__->m_ptLineMulti->setText( name );

    priv__->m_ptSpinType->setValue( val );
    priv__->m_ptSpinMin->setValue( val );
    priv__->m_ptSpinMax->setValue( val );

    priv__->m_ptComboList->clear();
}

void  TDialog::setDlgEnabled( bool enabled )
{
    setDlgTypeEnabled( enabled );
    setDlgPlaceholderEnabled( enabled );
    setDlgNewEnabled( enabled );
    setDlgAfterEnabled( enabled );
    setDlgBeforeEnabled( enabled );
    setDlgUlinkEnabled( enabled );
    setDlgUnameEnabled( enabled );
    setDlgMultiEnabled( enabled );
    setDlgMinEnabled( enabled );
    setDlgMaxEnabled( enabled );
    setDlgComboEnabled( enabled );
}

void  TDialog::setDlgTypeEnabled( bool enabled )
{
    priv__->m_ptSpinType->setEnabled( enabled );
}

void  TDialog::setDlgPlaceholderEnabled( bool  enabled )
{
    priv__->m_ptLinePlaceholder->setEnabled( enabled );
}

void  TDialog::setDlgNewEnabled( bool  enabled )
{
    priv__->m_ptLineNew->setEnabled( enabled );
}

void  TDialog::setDlgAfterEnabled( bool  enabled )
{
    priv__->m_ptLineAfter->setEnabled( enabled );
}

void  TDialog::setDlgBeforeEnabled( bool  enabled )
{
    priv__->m_ptLineBefore->setEnabled( enabled );
}

void  TDialog::setDlgUlinkEnabled( bool  enabled )
{
    priv__->m_ptLineUlink->setEnabled( enabled );
}

void  TDialog::setDlgUnameEnabled( bool  enabled )
{
    priv__->m_ptLineUname->setEnabled( enabled );
}

void  TDialog::setDlgMultiEnabled( bool  enabled )
{
    priv__->m_ptLineMulti->setEnabled( enabled );
}

void  TDialog::setDlgMinEnabled( bool  enabled )
{
    priv__->m_ptSpinMin->setEnabled( enabled );
}

void  TDialog::setDlgMaxEnabled( bool  enabled )
{
    priv__->m_ptSpinMax->setEnabled( enabled );
}

void  TDialog::setDlgComboEnabled( bool  enabled )
{
    priv__->m_ptComboList->setEnabled( enabled );
}

//------------------------------------------------------------------------------

void TDialog::onBtnAction( QAbstractButton*  btn )
{
    // нажали Ok
    if( QDialogButtonBox::AcceptRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog Ok" << priv__->m_ptLineName->text();

        priv__->m_tValues.m_zName = priv__->m_ptLineName->text();
        priv__->m_tValues.m_zPlaceholder = priv__->m_ptLinePlaceholder->text();
        priv__->m_tValues.m_zNew = priv__->m_ptLineNew->text();
        priv__->m_tValues.m_zAfter = priv__->m_ptLineAfter->text();
        priv__->m_tValues.m_zBefore = priv__->m_ptLineBefore->text();
        priv__->m_tValues.m_zUlink = priv__->m_ptLineUlink->text();
        priv__->m_tValues.m_zUname = priv__->m_ptLineUname->text();
        priv__->m_tValues.m_zMulti = priv__->m_ptLineMulti->text();

        priv__->m_tValues.m_uType = static_cast<unsigned>(priv__->m_ptSpinType->value());
        priv__->m_tValues.m_uMin = static_cast<unsigned>(priv__->m_ptSpinMin->value());
        priv__->m_tValues.m_uMax = static_cast<unsigned>(priv__->m_ptSpinMax->value());

        // шлем сигнал с данными
        Q_EMIT sendValues( priv__->m_tValues );
    }

    // нажали Reset
    if( QDialogButtonBox::ResetRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog Reset" << priv__->m_ptLineName->text();
    }

    // нажали Cancel
    if( QDialogButtonBox::RejectRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog Cancel" << priv__->m_ptLineName->text();

        // шлем сигнал с данными
        Q_EMIT sendCancel();
    }
}

//------------------------------------------------------------------------------
