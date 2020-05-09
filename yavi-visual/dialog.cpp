#include "dialog.h"

//------------------------------------------------------------------------------

class TPrivDialog
{
  friend class  TDialog;
public:

    QPushButton 		*m_ptBtnConfigPath;
    QPushButton 		*m_ptBtnBinaryPath;
    QPushButton 		*m_ptBtnDevPath;
    QComboBox 			*m_ptComboNetwork;
    QCheckBox 			*m_ptCboxMilstdTest;
    QCheckBox 			*m_ptCboxTasrv;
    QCheckBox 			*m_ptCboxDd64Imit;
    QCheckBox 			*m_ptCboxPci429Imit;
    QCheckBox 			*m_ptCboxShowExchange;
    QCheckBox 			*m_ptCboxShowRoute;
    QDialogButtonBox 	*m_ptBtnBox;
    QPushButton 		*m_ptBtnRestart;
    QGridLayout 	    *layout;

    QSpinBox 			*m_ptRunningHours;
    QSpinBox 			*m_ptRunningCount;
    QSpinBox 			*m_ptUsedHours;
    QLabel 				*m_ptInfoName;
    QLabel 				*m_ptInfoBrief;
    QLabel 				*m_ptInfoSerial;

    int 				m_uRunningHours;
    int 				m_uRunningCount;
    double 				m_fRunningCoeff;
    QString 			m_zRunningSmart;
    QString 			m_zRunningInfo;

    inline TPrivDialog()
    {
        layout = new QGridLayout;

        m_ptRunningHours = Q_NULLPTR;
        m_ptRunningCount = Q_NULLPTR;
        m_ptUsedHours = Q_NULLPTR;
        m_ptInfoName = Q_NULLPTR;
        m_ptInfoBrief = Q_NULLPTR;
        m_ptInfoSerial = Q_NULLPTR;
    }
    ~TPrivDialog()
    {
        layout->deleteLater();
    }

};

//------------------------------------------------------------------------------

TDialog::TDialog( QWidget *parent)
    : QDialog( parent )
{
//    QPushButton 	*btn;
//    QComboBox       *combo;
//    QCheckBox 		*cbox;
    QLabel          *title;
    int              row = 0;

    // заголовок формы
    setWindowTitle("YAML Visualizer");

    // иконка формы
    setWindowIcon( QIcon( ":/favicon.ico" ) );

    priv__ = std::unique_ptr<TPrivDialog>(new TPrivDialog);

    title = new QLabel( "Developer", this );
    QLabel 	*pCompanyName = new QLabel( "JSC «Helicopter avionics»", this );
    priv__->layout->addWidget( title, row, 0, 1, 1 );
    priv__->layout->addWidget( pCompanyName, row, 1, 1, 1 );
    row++;

    // создаем диалоговые кнопки
    QDialogButtonBox  *box = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Reset | QDialogButtonBox::Cancel, this );

    connect( box, SIGNAL(accepted()), this, SIGNAL(accepted()) );
    connect( box, SIGNAL(rejected()), this, SIGNAL(rejected()) );
    connect( box, SIGNAL(accepted()), this, SLOT(close()) );
    connect( box, SIGNAL(rejected()), this, SLOT(close()) );
    connect( box, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onReset(QAbstractButton*)) );

    // добавляем диалоговые кнопки в окно
    priv__->m_ptBtnBox = box;
    priv__->layout->addWidget( box, row, 0, 1, 2 );
    row++;

    priv__->layout->setSizeConstraint( QLayout::SetFixedSize );
    setLayout( priv__->layout );
}

TDialog::~TDialog()
{

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
        qDebug() << "dialog accept";
    }

    if( QDialogButtonBox::ResetRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog reset";
    }

    if( QDialogButtonBox::RejectRole == priv__->m_ptBtnBox->buttonRole( btn ) )
    {
        qDebug() << "dialog reject";
    }
}

//------------------------------------------------------------------------------
