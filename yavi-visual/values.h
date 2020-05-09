#ifndef VALUES_H
#define VALUES_H

#include <QtWidgets>

//------------------------------------------------------------------------------

class TValues
{
public:
    TValues()
    {
        m_zName.clear();
        m_zType.clear();
        m_zPlaceholder.clear();
        m_zNew.clear();
        m_zAfter.clear();
        m_zBefore.clear();
        m_zUlink.clear();
        m_zUname.clear();
        m_zMin.clear();
        m_zMax.clear();
        m_zMulti.clear();

        m_vList.clear();
    }
    ~TValues()
    {

    }

    QString      m_zName;
    QString      m_zType;
    QString      m_zPlaceholder;
    QString      m_zNew;
    QString      m_zAfter;
    QString      m_zBefore;
    QString      m_zUlink;
    QString      m_zUname;
    QString      m_zMin;
    QString      m_zMax;
    QString      m_zMulti;

    QStringList  m_vList;
};

Q_DECLARE_METATYPE( TValues )

//------------------------------------------------------------------------------

#endif // VALUES_H
