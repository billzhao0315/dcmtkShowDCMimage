#pragma once
#include <iostream>
#include <vector>
#include <memory>
class DICOMVolume;
class DICOMImageHelper
{
public:
    DICOMImageHelper(): m_pDICOMVolume( nullptr )
    {
    }
    std::shared_ptr< DICOMVolume > getDICOMVolume()
    {
        return m_pDICOMVolume;
    }
    bool DicomParse( std::vector<std::string> pathNames );
private:
    std::shared_ptr< DICOMVolume > m_pDICOMVolume;
};