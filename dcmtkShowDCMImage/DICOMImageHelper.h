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
private:
   std::shared_ptr<DICOMVolume> m_pDICOMVolume;
};