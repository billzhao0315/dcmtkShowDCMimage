#include "stdafx.h"
#include "DICOMImageHelper.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"

#include "DICOMVolume.h"
#include <algorithm>


DICOMImageHelper::DICOMImageHelper()
{
    m_pDICOMVolume = std::make_shared<DICOMVolume>();
}

void DICOMImageHelper::initMemory()
{
    
}

bool DICOMImageHelper::DicomParse( std::vector<std::string> pathNames )
{
       std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSeriesImage;
       

       for( int i = 0; i < pathNames.size(); ++i )
       {
           //读DICOM文件
            DcmFileFormat* pDicomFile = new DcmFileFormat();
            pDicomFile->loadFile( pathNames[i].c_str() );
            //得到数据集
            DcmDataset* pDataSet = pDicomFile->getDataset();

            E_TransferSyntax xfer = pDataSet->getOriginalXfer();
            //根据传输语法构造DicomImage从fstart帧开始一共fcount帧
            DicomImage* pDicomImg = new DicomImage (pDataSet, xfer/*, 0, 0, 1*/);
            const char* pElementValue;
            pDataSet->findAndGetString( DCM_WindowWidth, pElementValue );
            std::string sTemp = std::string( pElementValue );
            auto iPos = sTemp.find_first_of("\\");
            int dcmWidth = atoi( sTemp.substr(0,iPos).c_str() );

            pDataSet->findAndGetString( DCM_WindowCenter, pElementValue );
            sTemp = std::string( pElementValue );
            iPos = sTemp.find_first_of("\\");
            int dcmCenter = atoi( sTemp.substr(0,iPos).c_str() );
            pDicomImg->setWindow( dcmCenter, dcmWidth );

            // get the Image Position Patient
            double IPP[3];
            pDataSet->findAndGetString( DCM_ImagePositionPatient, pElementValue );
            sTemp = std::string( pElementValue );
            iPos = sTemp.find_first_of("\\");
            IPP[0] = atof( sTemp.substr(0,iPos).c_str() );
            std::string str( sTemp.begin() + iPos + 1, sTemp.end() );
            iPos = str.find_first_of( "\\" );
            IPP[1] = atof( str.substr(0,iPos).c_str() );
            str = std::string( str.begin() + iPos + 1, str.end() );
            iPos = str.find_first_of( "\\" );
            IPP[2] = atof( str.substr( 0, iPos ).c_str() );

            // get the Pixel Data Type
            DICOMSerieImage::eDataType ePixelType = DICOMSerieImage::eDataType::UNKNOWN;
            UINT16 u16PixelRepresentation;
            UINT16 U16BitsAllocated;
            const UINT16* pU16Pixel = 0;
            const UINT8* pU8Pixel = 0;
            unsigned long pixelDataLen = 0;
            pDataSet->findAndGetUint16( DCM_PixelRepresentation, u16PixelRepresentation );
            pDataSet->findAndGetUint16( DCM_BitsAllocated, U16BitsAllocated );
            if( u16PixelRepresentation == 0 )
            {
                if( U16BitsAllocated ==16 )
                {
                    ePixelType = DICOMSerieImage::eDataType::DATATYPE_uSHORT;
                }
                else
                {
                    ePixelType = DICOMSerieImage::eDataType::DATATYPE_uCHAR;
                }
            }
            else
            {
                if( U16BitsAllocated == 16 )
                {
                    ePixelType = DICOMSerieImage::eDataType::DATATYPE_SHORT;
                }
                else
                {
                    ePixelType = DICOMSerieImage::eDataType::DATATYPE_CHAR;
                }
            }
           
            if( ePixelType == DICOMSerieImage::eDataType::DATATYPE_uSHORT )
            {
                pDataSet->findAndGetUint16Array( DCM_PixelData, pU16Pixel, &pixelDataLen );
            }
            else if( ePixelType == DICOMSerieImage::eDataType::DATATYPE_uCHAR )
            {
                pDataSet->findAndGetUint8Array( DCM_PixelData, pU8Pixel, &pixelDataLen );
            }
            else
            {
               assert( false );
            }
            


            //通过以下的方法得到并用BitmapHeadInformation的结构体来保存DICOM文件的信息
            LPBITMAPINFOHEADER m_lpBMIH;
            m_lpBMIH = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
            m_lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
            m_lpBMIH->biWidth = pDicomImg->getWidth();
            m_lpBMIH->biHeight = pDicomImg->getHeight();
            m_lpBMIH->biPlanes = 1;
            m_lpBMIH->biBitCount = 24;
            m_lpBMIH->biCompression = BI_RGB;
            m_lpBMIH->biSizeImage = 0;
            m_lpBMIH->biXPelsPerMeter = 0;
            m_lpBMIH->biYPelsPerMeter = 0;
        
            //得到DICOM文件第frame的DIB数据(假设是24位的)
            void* pDicomDibits = NULL;
            unsigned long pixelLen = pDicomImg->createWindowsDIB(pDicomDibits, 0, 0, 24, 1, 1);
            //unsigned long imageLen = pDicomImg->getWidth() * pDicomImg->getHeight();
            std::shared_ptr<DICOMSerieImage> pDicomSeries = std::make_shared<DICOMSerieImage>();
            pDicomSeries->m_pPixelData = new unsigned char[pixelLen];
            //pDicomSeries->m_pRGBAPixelData = new unsigned char[ imageLen *4 ];
            pDicomSeries->m_nLength = pixelLen;
            pDicomSeries->m_pBitMapInfoHeader = m_lpBMIH;
            pDicomSeries->m_nWidth = pDicomImg->getWidth();
            pDicomSeries->m_nHeight = pDicomImg->getHeight();
            pDicomSeries->m_nImagePositionPatient[0] = IPP[0];
            pDicomSeries->m_nImagePositionPatient[1] = IPP[1];
            pDicomSeries->m_nImagePositionPatient[2] = IPP[2];
            pDicomSeries->m_eDataType = ePixelType;
            pDicomSeries->m_nWindow = dcmWidth;
            pDicomSeries->m_nLevel = dcmCenter;


            pDataSet->findAndGetString( DCM_SmallestImagePixelValue, pElementValue );
            if( pElementValue != NULL )
            {
                pDicomSeries->m_minValue = atof( pElementValue );
            }
            
            pDataSet->findAndGetString( DCM_LargestImagePixelValue, pElementValue );
            if( pElementValue != NULL )
            {
                pDicomSeries->m_maxValue = atof( pElementValue );
            }

            if( pDicomSeries->m_eDataType == DICOMSerieImage::eDataType::DATATYPE_uSHORT )
            {
                pDicomSeries->m_pOriginPixelData = new unsigned short[pixelDataLen];
                memcpy( pDicomSeries->m_pOriginPixelData, pU16Pixel, pixelDataLen*2 );
            }
            else if( pDicomSeries->m_eDataType == DICOMSerieImage::eDataType::DATATYPE_uCHAR )
            {
                pDicomSeries->m_pOriginPixelData = new unsigned char[pixelDataLen];
                memcpy( pDicomSeries->m_pOriginPixelData, pU8Pixel, pixelDataLen );
            }
            else
            {
                assert( false );
            }
            
            if( pElementValue == NULL )
            {
                pDicomSeries->m_maxValue = *std::max_element( (unsigned short*)pDicomSeries->m_pOriginPixelData, (unsigned short*)pDicomSeries->m_pOriginPixelData + pixelDataLen );
                pDicomSeries->m_minValue = *std::min_element( (unsigned short*)pDicomSeries->m_pOriginPixelData, (unsigned short*)pDicomSeries->m_pOriginPixelData + pixelDataLen ); 
            }


            /*for(unsigned long j = 0; j < imageLen; ++j )
            {
                pDicomSeries->m_pPixelData[3*j] = ((unsigned char*)pDicomDibits)[3*j];
                pDicomSeries->m_pPixelData[3*j+1] = ((unsigned char*)pDicomDibits)[3*j];
                pDicomSeries->m_pPixelData[3*j+2] = ((unsigned char*)pDicomDibits)[3*j];
                pDicomSeries->m_pRGBAPixelData[4*j] = ((unsigned char*)pDicomDibits)[3*j];
                pDicomSeries->m_pRGBAPixelData[4*j+1] = ((unsigned char*)pDicomDibits)[3*j];
                pDicomSeries->m_pRGBAPixelData[4*j+2] = ((unsigned char*)pDicomDibits)[3*j];
                pDicomSeries->m_pRGBAPixelData[4*j+3] = ((unsigned char*)pDicomDibits)[3*j];
            }*/
            memcpy( pDicomSeries->m_pPixelData, pDicomDibits, pixelLen );

            delete m_lpBMIH;
            delete pDicomImg;
            delete pDicomFile;
            delete pDicomDibits;
            arrDicomSeriesImage.push_back( pDicomSeries );
       }
       //initMemory();
       m_pDICOMVolume->setDICOMSeriesImage(arrDicomSeriesImage);
       return true;
}