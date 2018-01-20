#pragma once
#include <iostream>
#include <vector>
#include <memory>
class DICOMVolume;
class DICOMImageHelper
{
public:
    DICOMImageHelper();
    std::shared_ptr<DICOMVolume> getDICOMVolume()
    {
        return m_pDICOMVolume;
    }
    bool DicomParse( std::vector<std::string> pathNames );
    void initMemory();
    bool isDICOMFile( std::string fileName );
private:
   std::shared_ptr<DICOMVolume> m_pDICOMVolume;
};