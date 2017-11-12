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
    ~DICOMSerieImage();
    double m_ImagePositionPatient[3];
    unsigned char* m_pPixelData;
    unsigned long  m_nLength;
    int m_nWidth;
    int m_nHeight;
    LPBITMAPINFOHEADER m_pBitMapInfoHeader;
};

class DICOMVolume
{
public:
    DICOMVolume():m_DicomSerieImage(0), m_nDepth(0)
    {

    }
    std::vector<std::shared_ptr<DICOMSerieImage>> getDICOMSeriesImage() const
    {
        return m_DicomSerieImage;
    }
    unsigned int getDepth() const;
    void setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage );
private:
    DICOMHeader m_DiconHeader;
    std::vector<std::shared_ptr<DICOMSerieImage>> m_DicomSerieImage;
    unsigned int m_nDepth;
};