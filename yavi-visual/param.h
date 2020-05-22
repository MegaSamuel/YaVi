#ifndef PARAM_H
#define PARAM_H

#include <QtWidgets>

#include "dialog.h"
#include "values.h"

//------------------------------------------------------------------------------

class TCategory;

class TParam;

//------------------------------------------------------------------------------

class TCategories : public QWidget
{
   Q_OBJECT

public:
    TCategories( TParam  *pMentor = Q_NULLPTR, int  depth = 0 );
    ~TCategories();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

    void           setCategoriesName( const std::string&  name, bool  set_to_node = false );
    void           setCategoriesUlink( const std::string&  name, bool  set_to_node = false );
    void           setCategoriesUname( const std::string&  name, bool  set_to_node = false );

    QString        getCategoriesName();
    QString        getCategoriesUlink();
    QString        getCategoriesUname();

    void           setCategoriesDepth( int  depth );
    int            getCategoriesDepth();

    int            getCategoriesWidth();
    int            getCategoriesHeight();

    void           CategoriesDelete();

    void           widget_stretch( int width, int height ) noexcept;         // растягиваем виджет
    void           widget_shrink( int width, int height ) noexcept;          // сжимаем виджет

    QVBoxLayout   *m_vlayout;   // главный layout класса
    QList<TParam*> m_apParamList;

protected Q_SLOTS :
    void           onBtnDec();
    void           onBtnInc();
    void           onBtnName();
    void           onSendCancel();
    void           onSendValues( TValues& );

private:
    void           clear();

    void           clearNodeSequence();

    void           setIncBtnVisible( bool visible );

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    //QVBoxLayout   *m_vlayout;   // главный layout класса
    QHBoxLayout   *m_hlayout;   // вложенный layout

    QPushButton   *m_ptBtnDec;
    QPushButton   *m_ptBtnInc;
    QPushButton   *m_ptBtnName;

    QString        m_zName;     // categories Name
    QString        m_zBtnName;  // текст на кнопке

    QString        m_zUlink;
    QString        m_zUname;

    int            m_depth;

    TDialog       *m_ptDialog;

    TValues        m_tValues;

    TParam        *m_pMentor;

    int            m_width;     // ширина виджета
    int            m_height;    // высота виджета

    void           widget_parent_stretch( int width, int height ) noexcept;  // растягиваем виджет
    void           widget_parent_shrink( int width, int height ) noexcept;   // сжимаем виджет

    void           widget_size_reset() noexcept;  // сброс размера виджета

    bool           need_to_add; // необходимость создать новый набор параметров в ямле
};

//------------------------------------------------------------------------------


class TParam : public QWidget
{
   Q_OBJECT

public:
    TParam( TCategory  *pAncestor = Q_NULLPTR, TCategories  *pMentor = Q_NULLPTR, int  depth = 0 );
    ~TParam();

    void           setNode( const YAML::Node&  node );
    void           setNodeParent( const YAML::Node&  node );
    void           setNodeIndex( int  index );

    YAML::Node&    getNode();
    YAML::Node&    getNodeParent();
    int            getNodeIndex();

    void           setParamName( const std::string&  name, bool  set_to_node = false );
    void           setParamPlaceholder( const std::string&  name, bool  set_to_node = false );
    void           setParamNew( const std::string&  name, bool  set_to_node = false );
    void           setParamAfter( const std::string&  name, bool  set_to_node = false );
    void           setParamBefore( const std::string&  name, bool  set_to_node = false );
    void           setParamUlink( const std::string&  name, bool  set_to_node = false );
    void           setParamUname( const std::string&  name, bool  set_to_node = false );
    void           setParamMulti( const std::string&  name, bool  set_to_node = false );

    void           setParamType( unsigned  val, bool  set_to_node = false );
    void           setParamMin( unsigned  val, bool  set_to_node = false );
    void           setParamMax( unsigned  val, bool  set_to_node = false );

    // убрать запись из поля values
    void           remParamList( QString&  item, bool  set_to_node = false );
    // добавить запись в поле values
    void           addParamList( QString&  item, bool  set_to_node = false );
    // установить поле values
    void           setParamList( QStringList  vlist, const std::string&  list, bool  set_to_node = false );

    QString        getParamName();
    QString        getParamPlaceholder();
    QString        getParamNew();
    QString        getParamAfter();
    QString        getParamBefore();
    QString        getParamUlink();
    QString        getParamUname();
    QString        getParamMulti();

    unsigned       getParamType();
    unsigned       getParamMin();
    unsigned       getParamMax();

    QStringList    getParamList();

    void           setParamDepth( int  depth );
    int            getParamDepth();

    int            getParamWidth();
    int            getParamHeight();

    void           ParamDraw( TParam  *pParam );
    void           ParamDelete();

    void           widget_stretch( int width, int height ) noexcept;         // растягиваем виджет
    void           widget_shrink( int width, int height ) noexcept;          // сжимаем виджет

    QVBoxLayout   *m_vlayout;   // главный layout класса
    QList<TCategories*>  m_apCategoriesList;

protected Q_SLOTS :
    void           onBtnDec();
    void           onBtnValDec();
    void           onBtnInc();
    void           onBtnName();
    void           onSendCancel();
    void           onSendValues( TValues& );
    void           onSendValue( int );

private:
    void           clear();

    void           clearNodeSequence();

    void           setIncBtnVisible( bool visible );

    void           setParamValueAdd();
    void           setParamValueDel();
    void           setParamValueMin( int  min );
    void           setParamValueMax( int  max );

    bool           isChildrenAbsent(); // true если нет детей

    YAML::Node     m_node;        // текущий уровнь дерева ямла
    YAML::Node     m_node_parent; // родительский уровнь дерева ямла
    int            m_node_index;  // номер перечисления у родителя

    QString        m_zName;
    QString        m_zPlaceholder;
    QString        m_zNew;
    QString        m_zAfter;
    QString        m_zBefore;
    QString        m_zUlink;
    QString        m_zUname;
    QString        m_zMulti;

    unsigned       m_uType;
    unsigned       m_uMin;
    unsigned       m_uMax;

    std::string    m_zList;
    QStringList    m_vList;

    //QVBoxLayout   *m_vlayout;   // главный layout класса
    QHBoxLayout   *m_hlayout1;  // вложенный layout
    QHBoxLayout   *m_hlayout2;  // вложенный layout для второй строки

    QPushButton   *m_ptBtnDec;
    QPushButton   *m_ptBtnInc;
    QPushButton   *m_ptBtnName;

    QString        m_zBtnName;  // текст на кнопке

    bool           m_second_row_exist = false;
    QPushButton   *m_ptBtnValDec;
    QSpinBox      *m_ptSpinValue;

    TDialog       *m_ptDialog;

    TValues        m_tValues;

    TCategory     *m_pAncestor;
    TCategories   *m_pMentor;
    int            m_depth;

    int            m_width;     // ширина виджета
    int            m_height;    // высота виджета

    void           widget_parent_stretch( int width, int height ) noexcept;  // растягиваем виджет
    void           widget_parent_shrink( int width, int height ) noexcept;   // сжимаем виджет

    void           widget_size_reset() noexcept;  // сброс размера виджета

    bool           need_to_add; // необходимость создать новый набор параметров в ямле
};

//------------------------------------------------------------------------------

#endif // PARAM_H
