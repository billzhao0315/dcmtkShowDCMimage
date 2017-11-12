#include "stdafx.h"
#include "DICOMVolume.h"

//DICOMVolume::DICOMVolume():m_DicomSerieImage(0)
//{
//
//}
DICOMSerieImage::~DICOMSerieImage()
{
    delete[] m_pPixelData;
}


void DICOMVolume::setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage )
{
    m_DicomSerieImage = arrDicomSerieImage;
    m_nDepth = m_DicomSerieImage.size();
}

unsigned int DICOMVolume::getDepth() const
{
    return m_nDepth;
}