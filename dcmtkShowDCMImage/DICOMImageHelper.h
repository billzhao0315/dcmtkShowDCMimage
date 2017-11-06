#pragma once
#include <iostream>
#include <vector>
#include <memory>
class DICOMVolume;
class DICOMImageHelper
{
public:
    DICOMImageHelper()
    {

    }
    DICOMVolume* getDICOMVolume()
    {
        return m_pDICOMVolume;
    }
    bool DicomParse( std::vector<std::string> pathNames );
    void initMemory();
private:
    DICOMVolume* m_pDICOMVolume;
};