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
        m_pRGBAPixelData = NULL;
        m_minValue = 0;
        m_maxValue = 0;
        m_nWindow = 0;
        m_nLevel = 0;
    }
    void mapToWindowLevel();
    void mapToGray();

    ~DICOMSerieImage();
    //double m_ImagePositionPatient[3];
    unsigned char* m_pPixelData;
    void* m_pOriginPixelData;
    unsigned char* m_pRGBAPixelData;
    unsigned long  m_nLength;
    int m_nWidth;
    int m_nHeight;
    int m_nWindow;
    int m_nLevel;
    double m_maxValue;
    double m_minValue;
    double m_nImagePositionPatient[3];
    eDataType m_eDataType;
    LPBITMAPINFOHEADER m_pBitMapInfoHeader;
};

class DICOMVolume
{
public:
    DICOMVolume():m_DicomSerieImage(0), m_nDepth(0),m_ptffFile(NULL)
    {

    }
    ~DICOMVolume()
    {
        if( m_ptffFile != NULL )
        {
            delete[] m_ptffFile;
        }
    }
    std::vector<std::shared_ptr<DICOMSerieImage>> getDICOMSeriesImage() const
    {
        return m_DicomSerieImage;
    }
    unsigned int getDepth() const;
    void setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage );
    void loadtffFile( std::string tffFileName );
private:
    void sortDICOMSeriesByIPP();
private:
    DICOMHeader m_DiconHeader;
    std::vector<std::shared_ptr<DICOMSerieImage>> m_DicomSerieImage;
    unsigned char* m_ptffFile;
    unsigned int m_nDepth;
};