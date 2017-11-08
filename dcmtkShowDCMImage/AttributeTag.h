#pragma once

class AttributeTag
{
public:
    AttributeTag();
    AttributeTag(const AttributeTag& tagFrom );
    AttributeTag& operator =( const AttributeTag& tagFrom  );
    bool operator ==( AttributeTag tagFrom ) const;
    bool operator !=( AttributeTag tagFrom ) const;
    int getValue() const;
private:
    int m_nValue;
    static int m_nNextValue;
};