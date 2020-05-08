#include "param.h"

//------------------------------------------------------------------------------

TParam::TParam()
{

}

TParam::~TParam()
{

}

//------------------------------------------------------------------------------

void  TParam::onBtnDec()
{
    qDebug() << "Dec button";
}

void  TParam::onBtnInc()
{
    qDebug() << "Inc button";
}

//------------------------------------------------------------------------------

void  TParam::setParamName( const std::string&  name )
{
    m_zName = QString::fromStdString(name);
}

void  TParam::setParamPlaceholder( const std::string&  name )
{
    m_zPlaceholder = QString::fromStdString(name);
}

void  TParam::setParamNew( const std::string&  name )
{
    m_zNew = QString::fromStdString(name);
}

void  TParam::setParamAfter( const std::string&  name )
{
    m_zAfter = QString::fromStdString(name);
}

void  TParam::setParamBefore( const std::string&  name )
{
    m_zBefore = QString::fromStdString(name);
}

void  TParam::setParamUlink( const std::string&  name )
{
    m_zUlink = QString::fromStdString(name);
}

void  TParam::setParamUname( const std::string&  name )
{
    m_zUname = QString::fromStdString(name);
}

void  TParam::setParamMulti( const std::string&  name )
{
    m_zMulti = QString::fromStdString(name);
}


void  TParam::setParamType( unsigned  val )
{
    m_uType = val;
}

void  TParam::setParamMin( unsigned  val )
{
    m_uMin = val;
}

void  TParam::setParamMax( unsigned  val )
{
    m_uMax = val;
}

//------------------------------------------------------------------------------

QString  TParam::getParamName()
{
    return m_zName;
}

QString  TParam::getParamPlaceholder()
{
    return m_zPlaceholder;
}

QString  TParam::getParamNew()
{
    return m_zNew;
}

QString  TParam::getParamAfter()
{
    return m_zAfter;
}

QString  TParam::getParamBefore()
{
    return m_zBefore;
}

QString  TParam::getParamUlink()
{
    return m_zUlink;
}

QString  TParam::getParamUname()
{
    return m_zUname;
}

QString  TParam::getParamMulti()
{
    return m_zMulti;
}


unsigned  TParam::getParamType()
{
    return m_uType;
}

unsigned  TParam::getParamMin()
{
    return m_uMin;
}

unsigned  TParam::getParamMax()
{
    return m_uMax;
}

//------------------------------------------------------------------------------
