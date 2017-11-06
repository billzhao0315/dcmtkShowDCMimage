#pragma once
#include <iostream>
#include <vector>
#include <memory>
struct DICOMHeader
{
    DICOMHeader(){}
};

struct DICOMSerieImage
{
    DICOMSerieImage()
    {
        m_nLength = 0;
        m_pPixelData = NULL;
    }
    double m_ImagePositionPatient[3];
    unsigned char* m_pPixelData;
    unsigned long  m_nLength;
};

class DICOMVolume
{
public:
    DICOMVolume():m_DicomSerieImage(0)
    {

    }
    std::vector<std::shared_ptr<DICOMSerieImage>> getDICOMSeriesImage() const
    {
        return m_DicomSerieImage;
    }
    void setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage );
private:
    DICOMHeader m_DiconHeader;
    std::vector<std::shared_ptr<DICOMSerieImage>> m_DicomSerieImage;
};