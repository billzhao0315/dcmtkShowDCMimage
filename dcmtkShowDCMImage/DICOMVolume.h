#pragma once
#include <iostream>
#include <vector>
#include <memory>
struct DICOMHeader
{
    
};

struct DICOMSerieImage
{
    double m_ImagePositionPatient[3];
    unsigned char* m_pPixelData;
    unsigned long  m_nLength;
};

class DICOMVolume
{
public:
    DICOMVolume();
    std::vector<std::shared_ptr<DICOMSerieImage>> getDICOMSeriesImage() const
    {
        return m_DicomSerieImage;
    }
private:
    DICOMHeader m_DiconHeader;
    std::vector<std::shared_ptr<DICOMSerieImage>> m_DicomSerieImage;
};