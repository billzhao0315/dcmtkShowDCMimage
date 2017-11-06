#include "DICOMImageHelper.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmimgle/dcmimage.h"
#include "dcmtk/dcmdata/dcfilefo.h"

#include "DICOMVolume.h"

bool DICOMImageHelper::DicomParse( std::vector<std::string> pathNames )
{
       std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSeriesImage = m_pDICOMVolume->getDICOMSeriesImage();
       DcmFileFormat* pDicomFile = new DcmFileFormat();

       for( int i = 0; i < pathNames.size(); ++i )
       {
           //读DICOM文件
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
            arrDicomSeriesImage[i]->m_pPixelData = new unsigned char[pixelLen];
            arrDicomSeriesImage[i]->m_nLength = pixelLen;
            for(int i = 0; i < pixelLen; ++i )
            {
                arrDicomSeriesImage[i]->m_pPixelData[i] = ((unsigned char*)pDicomDibits)[i];
            }
       }
        
}