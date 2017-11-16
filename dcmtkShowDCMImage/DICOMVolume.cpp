#include "stdafx.h"
#include "DICOMVolume.h"
#include <algorithm>

//DICOMVolume::DICOMVolume():m_DicomSerieImage(0)
//{
//
//}
DICOMSerieImage::~DICOMSerieImage()
{
    if( m_pPixelData != NULL )
    {
        delete[] m_pPixelData;
    }
    if( m_pOriginPixelData != NULL )
    {
        delete[] m_pOriginPixelData;
    }
    if( m_pRGBAPixelData != NULL )
    {
        delete[] m_pRGBAPixelData;
    }
    
}

void DICOMSerieImage::mapToWindowLevel()
{
    int imageLen = m_nWidth * m_nHeight;
    double maxWindow = m_nLevel + m_nWindow/2;
    double minWindow = m_nLevel - m_nWindow/2;
    m_pRGBAPixelData = new unsigned char[ imageLen *4 ];
    unsigned short* pTmep = (unsigned short*)m_pOriginPixelData;
    for(int i = 0; i < imageLen; ++i)
    {
        if( pTmep[i] < minWindow )
        {
            pTmep[i] = 0;
        }
        else if( pTmep[i] > maxWindow )
        {
            pTmep[i] = 255;
        }
        else
        {
            pTmep[i] = ( pTmep[i] - minWindow )*255 / m_nWindow; 
        }

        m_pRGBAPixelData[ 4*i ] = pTmep[i];
        m_pRGBAPixelData[ 4*i +1 ] = pTmep[i];
        m_pRGBAPixelData[ 4*i +2 ] = pTmep[i];
        m_pRGBAPixelData[ 4*i +3 ] = pTmep[i];
    }
    m_minValue = 0;
    m_maxValue = 255;
}

void DICOMSerieImage::mapToGray()
{
    int imageLen = m_nWidth * m_nHeight;
    double Temp = (double)255 /( m_maxValue - m_minValue );
    m_pRGBAPixelData = new unsigned char[ imageLen *4 ];
    for( int i = 0; i < imageLen; ++i )
    {
        
        m_pRGBAPixelData[ 4*i ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
        m_pRGBAPixelData[ 4*i +1 ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
        m_pRGBAPixelData[ 4*i +2 ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
        m_pRGBAPixelData[ 4*i +3 ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
    }
}


void DICOMVolume::setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage )
{
    m_DicomSerieImage = arrDicomSerieImage;
    m_nDepth = m_DicomSerieImage.size();
    sortDICOMSeriesByIPP();
}

void DICOMVolume::sortDICOMSeriesByIPP()
{
    auto greater = []( const std::shared_ptr<DICOMSerieImage> firstData, const std::shared_ptr<DICOMSerieImage> lasttData )
    {
        return firstData->m_nImagePositionPatient[2] > lasttData->m_nImagePositionPatient[2];
    };
    std::sort( m_DicomSerieImage.begin(), m_DicomSerieImage.end(), greater );
}

unsigned int DICOMVolume::getDepth() const
{
    return m_nDepth;
}