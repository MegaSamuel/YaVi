#include "tabdialog.h"

#include "mainwindow.h"

//------------------------------------------------------------------------------

class TPrivTabDialog
{
    friend class  TTabDialog;
public:

    QLineEdit        *m_ptLineId;
    QLineEdit        *m_ptLineName;
    QComboBox        *m_ptComboType;

    QDialogButtonBox *m_ptBtnBox;

    QGridLayout      *m_grid;

    QString           m_zValue;
    TValues           m_tValues;

    inline TPrivTabDialog()
    {
        m_grid = new QGridLayout;
    }
    ~TPrivTabDialog()
    {
        m_grid->deleteLater();
    }

};

//------------------------------------------------------------------------------

TTabDialog::TTabDialog( bool fullsize, QString name, QWidget *parent, QString value )
    : QDialog( parent )
{
    int  row = 0;

    m_bFullSize = fullsize;

    // заголовок формы
    setWindowTitle("YAML Visualizer");

    // иконка формы
    setWindowIcon( QIcon( ":/favicon.ico" ) );

    // цепляем местный сигнал к слоту MainWindow
    connect( this, &TTabDialog::sendChanged, MainWindow::getMainWinPtr(), &MainWindow::onYamlChanged );

    priv__ = std::unique_ptr<TPrivTabDialog>(new TPrivTabDialog);

    QLabel *title = new QLabel( name, this );
    title->setMinimumWidth( 93 );
    priv__->m_grid->addWidget( title, row, 0, 1, 1 );
    row++;

    if( m_bFullSize )
    {
        QLabel *lblId = new QLabel( QString( "%1%2" ).arg("ID").arg(":"), this );
        priv__->m_grid->addWidget( lblId, row, 0, 1, 1 );
        QLineEdit *ptLineId = new QLineEdit( this );
        priv__->m_ptLineId = ptLineId;
        priv__->m_grid->addWidget( ptLineId, row, 1, 1, 1 );
        row++;

        QLabel *lblName = new QLabel( QString( "%1%2" ).arg("Name").arg(":"), this );
        priv__->m_grid->addWidget( lblName, row, 0, 1, 1 );
        QLineEdit *ptLineName = new QLineEdit( this );
        priv__->m_ptLineName = ptLineName;
        priv__->m_grid->addWidget( ptLineName, row, 1, 1, 1 );
        row++;

        QLabel *lblType = new QLabel( QString( "%1%2" ).arg("Type").arg(":"), this );
        priv__->m_grid->addWidget( lblType, row, 0, 1, 1 );
        QComboBox *ptComboType = new QComboBox( this );
        ptComboType->addItem( "link" );
        ptComboType->addItem( "row" );
        ptComboType->addItem( "col" );
        priv__->m_ptComboType = ptComboType;
        priv__->m_grid->addWidget( ptComboType, row, 1, 1, 1 );
        row++;
    }
    else
    {
        QString  str;

        if( 0 != value.length() )
        {
            str = value;
        }
        else
        {
            str = "Name";
        }

        QLabel *lblName = new QLabel( QString( "%1%2" ).arg(str).arg(":"), this );
        priv__->m_grid->addWidget( lblName, row, 0, 1, 1 );
        QLineEdit *ptLineName = new QLineEdit( this );
        priv__->m_ptLineName = ptLineName;
        priv__->m_grid->addWidget( ptLineName, row, 1, 1, 1 );
        row++;
    }

    QLabel *empty = new QLabel( this );
    priv__->m_grid->addWidget( empty, row, 0, 1, 2 );
    row++;

    // создаем диалоговые кнопки
    QDialogButtonBox  *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );

    connect( box, &QDialogButtonBox::accepted, this, &TDialog::close );
    connect( box, &QDialogButtonBox::rejected, this, &TDialog::close );

    connect( box, &QDialogButtonBox::clicked, this, &TTabDialog::onBtnAction );

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->m_grid->addWidget( box, row, 0, 1, 2 );
    row++;

    setLayout( priv__->m_grid );

    setMinimumSize( priv__->m_grid->minimumSize().width(), priv__->m_grid->minimumSize().height() );
}

TTabDialog::~TTabDialog()
{

}

//------------------------------------------------------------------------------

void  TTabDialog::setDlgId( const QString& name )
{
    priv__->m_ptLineId->setText( name );
}

void  TTabDialog::setDlgName( const QString& name )
{
    priv__->m_ptLineName->setText( name );
}

//------------------------------------------------------------------------------

void  TTabDialog::setDlgEmpty()
{
    QString  name;

    name.clear();

    if( m_bFullSize )
    {
        priv__->m_ptLineId->setText( name );
        priv__->m_ptLineName->setText( name );
    }
    else
    {
        priv__->m_ptLineName->setText( name );
    }
}

//------------------------------------------------------------------------------

void  TTabDialog::onBtnAction( QAbstractButton*  btn )
{
    // нажали Ok
    if( QDialogButtonBox::AcceptRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        //qDebug() << "dialog Ok" << priv__->m_ptLineName->text();

        if( m_bFullSize )
        {
            priv__->m_tValues.m_zId = priv__->m_ptLineId->text();
            priv__->m_tValues.m_zName = priv__->m_ptLineName->text();
            priv__->m_tValues.m_uType = static_cast<unsigned>(priv__->m_ptComboType->currentIndex());

            // шлем сигнал с данными
            Q_EMIT sendValues( priv__->m_tValues );
        }
        else
        {
            priv__->m_zValue = priv__->m_ptLineName->text();

            priv__->m_tValues.m_zName.clear();
            priv__->m_tValues.m_zId.clear();
            priv__->m_tValues.m_uType = 0;

            // шлем сигнал с данными
            Q_EMIT sendValue( priv__->m_zValue );
        }

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

//------------------------------------------------------------------------------

