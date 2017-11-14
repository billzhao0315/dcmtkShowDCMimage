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
    //delete[] m_pRGBAPixelData;
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