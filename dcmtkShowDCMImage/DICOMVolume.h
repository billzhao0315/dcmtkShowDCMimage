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
    enum eDataType
    {
        DATATYPE_uSHORT,
        DATATYPE_uCHAR,
        DATATYPE_SHORT,
        DATATYPE_CHAR,
        UNKNOWN
    };
    DICOMSerieImage()
    {
        m_nWidth = 0;
        m_nHeight = 0;
        m_nLength = 0;
        m_eDataType = UNKNOWN;
        m_pPixelData = NULL;
        m_pBitMapInfoHeader = NULL;
        m_pOriginPixelData = NULL;
    }
    ~DICOMSerieImage();
    //double m_ImagePositionPatient[3];
    unsigned char* m_pPixelData;
    void* m_pOriginPixelData;
    //unsigned char* m_pRGBAPixelData;
    unsigned long  m_nLength;
    int m_nWidth;
    int m_nHeight;
    double m_nImagePositionPatient[3];
    eDataType m_eDataType;
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
    void sortDICOMSeriesByIPP();
private:
    DICOMHeader m_DiconHeader;
    std::vector<std::shared_ptr<DICOMSerieImage>> m_DicomSerieImage;
    unsigned int m_nDepth;
};