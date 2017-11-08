#include "stdafx.h"
#include "AttributeTag.h"
int AttributeTag::m_nNextValue = 1;
AttributeTag::AttributeTag():m_nValue(m_nNextValue++)
{
    
}

AttributeTag::AttributeTag(const AttributeTag& tagFrom)
{
    m_nValue = tagFrom.getValue();
}

AttributeTag& AttributeTag::operator=( const AttributeTag& tagFrom)
{
    if( &tagFrom != this )
    {
        m_nValue = tagFrom.getValue();
    }
    return (*this);
}


int AttributeTag::getValue() const
{
    return m_nValue;
}

bool AttributeTag::operator!=(AttributeTag tagFrom) const
{
    if( tagFrom.getValue() != m_nValue )
    {
        return true;
    }

    return false;
}

bool AttributeTag::operator==(AttributeTag tagFrom) const
{
    if( tagFrom.getValue() == m_nValue )
    {
        return true;
    }

    return false;
}



