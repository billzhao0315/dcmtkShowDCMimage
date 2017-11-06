#include "stdafx.h"
#include "DICOMVolume.h"

//DICOMVolume::DICOMVolume():m_DicomSerieImage(0)
//{
//
//}

void DICOMVolume::setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage )
{
    m_DicomSerieImage = arrDicomSerieImage;
}