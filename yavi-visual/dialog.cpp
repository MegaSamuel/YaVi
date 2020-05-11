#include "dialog.h"

//------------------------------------------------------------------------------

class TPrivDialog
{
    friend class  TDialog;
public:

    QLineEdit    *m_ptTextName;
    QSpinBox     *m_ptSpinType;
    QTextEdit    *m_ptTextPlaceholder;
    QTextEdit    *m_ptTextNew;
    QTextEdit    *m_ptTextAfter;
    QTextEdit    *m_ptTextBefore;
    QTextEdit    *m_ptTextUlink;
    QTextEdit    *m_ptTextUname;
    QSpinBox     *m_ptSpinMin;
    QSpinBox     *m_ptSpinMax;
    QTextEdit    *m_ptTextMulti;
    QComboBox    *m_ptComboList;

    QDialogButtonBox  *m_ptBtnBox;

    QGridLayout  *m_grid;

    inline TPrivDialog()
    {
        m_grid = new QGridLayout;

        m_ptTextName = new QLineEdit;
    }
    ~TPrivDialog()
    {
        m_grid->deleteLater();
    }

};

//------------------------------------------------------------------------------

TDialog::TDialog( QWidget *parent)
    : QDialog( parent )
{
//    QPushButton 	*btn;
//    QComboBox       *combo;
//    QCheckBox 		*cbox;
    QLabel    *title;
    int        row = 0;

    // заголовок формы
    setWindowTitle("YAML Visualizer");

    // иконка формы
    setWindowIcon( QIcon( ":/favicon.ico" ) );

    priv__ = std::unique_ptr<TPrivDialog>(new TPrivDialog);

    title = new QLabel( QString( "%1%2" ).arg("Name").arg(":"), this );
    title->setMinimumWidth( 93 );
    priv__->m_grid->addWidget( title, row, 0, 1, 1 );
    priv__->m_grid->addWidget( priv__->m_ptTextName, row, 1, 1, 1 );
    row++;

    // создаем диалоговые кнопки
    QDialogButtonBox  *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this );

    connect( box, SIGNAL(accepted()), this, SIGNAL(accepted()) );
    connect( box, SIGNAL(rejected()), this, SIGNAL(rejected()) );
    connect( box, SIGNAL(accepted()), this, SLOT(close()) );
    connect( box, SIGNAL(rejected()), this, SLOT(close()) );
    connect( box, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onReset(QAbstractButton*)) );

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->m_grid->addWidget( box, row, 0, 1, 2 );
    row++;

//    priv__->m_grid->setSizeConstraint( QLayout::SetFixedSize );
    setLayout( priv__->m_grid );
}

TDialog::~TDialog()
{

}

//------------------------------------------------------------------------------

void  TDialog::setName( const QString& name )
{
    priv__->m_ptTextName->setText( name );
}

//------------------------------------------------------------------------------

void TDialog::onReset( QAbstractButton*  btn )
{
//    qDebug() << "onReset";
//    if( QDialogButtonBox::ResetRole != priv__->m_ptBtnBox->buttonRole( btn ) )
//        return;

//    qDebug() << "onReset after";

    if( QDialogButtonBox::AcceptRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog Ok" << priv__->m_ptTextName->text();
    }

    if( QDialogButtonBox::ResetRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog Reset";
    }

    if( QDialogButtonBox::RejectRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog Cancel";
    }
}

//------------------------------------------------------------------------------
