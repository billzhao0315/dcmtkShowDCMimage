#include "stdafx.h"
#include "DICOMVolume.h"
#include <algorithm>
#include <fstream>

//DICOMVolume::DICOMVolume():m_DicomSerieImage(0)
//{
//
//}
unsigned char* tempTffFile = NULL;

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
            pTmep[i] = static_cast< unsigned short >( ( pTmep[i] - minWindow )*255 / m_nWindow ); 
        }

        if( tempTffFile != NULL )
        {
            m_pRGBAPixelData[ 4*i ] = tempTffFile[4*pTmep[i]];
            m_pRGBAPixelData[ 4*i +1 ] = tempTffFile[4*pTmep[i]+1];
            m_pRGBAPixelData[ 4*i +2 ] = tempTffFile[4*pTmep[i]+2];
            m_pRGBAPixelData[ 4*i +3 ] = tempTffFile[4*pTmep[i]+3];
        }
        else
        {
            m_pRGBAPixelData[ 4*i ] = static_cast< unsigned char >( pTmep[i] );
            m_pRGBAPixelData[ 4*i +1 ] = static_cast< unsigned char >( pTmep[i] );
            m_pRGBAPixelData[ 4*i +2 ] = static_cast< unsigned char >( pTmep[i] );
            m_pRGBAPixelData[ 4*i +3 ] = static_cast< unsigned char >( pTmep[i] );
        }
        
    }
    m_minValue = 0;
    m_maxValue = 255;
    //m_nLevel = 127;
    //m_nWindow = 255;
}

void DICOMSerieImage::mapToGray()
{
    int imageLen = m_nWidth * m_nHeight;
    double Temp = (double)255 /( m_maxValue - m_minValue );
    m_pRGBAPixelData = new unsigned char[ imageLen *4 ];
    for( int i = 0; i < imageLen; ++i )
    {
        unsigned char tempGray = static_cast<unsigned char>( ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp );
        //unsigned char tempGray = m_pPixelData[ i ];

        if( tempTffFile != NULL )
        {
            m_pRGBAPixelData[ 4*i ]    = tempTffFile[4*tempGray];
            m_pRGBAPixelData[ 4*i +1 ] = tempTffFile[4*tempGray+1];
            m_pRGBAPixelData[ 4*i +2 ] = tempTffFile[4*tempGray+2];
            m_pRGBAPixelData[ 4*i +3 ] = tempTffFile[4*tempGray+3];
        }
        else
        {
            /*m_pRGBAPixelData[ 4*i ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
            m_pRGBAPixelData[ 4*i +1 ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
            m_pRGBAPixelData[ 4*i +2 ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;
            m_pRGBAPixelData[ 4*i +3 ] = ( ((unsigned short*)m_pOriginPixelData)[i] - m_minValue ) * Temp;*/
            m_pRGBAPixelData[ 4*i ] = tempGray;
            m_pRGBAPixelData[ 4*i +1 ] = tempGray;
            m_pRGBAPixelData[ 4*i +2 ] = tempGray;
            m_pRGBAPixelData[ 4*i +3 ] = tempGray;
        }
        
    }
}


void DICOMVolume::setDICOMSeriesImage( std::vector<std::shared_ptr<DICOMSerieImage>> arrDicomSerieImage )
{
    m_DicomSerieImage = arrDicomSerieImage;
    m_nDepth = static_cast< unsigned int >( m_DicomSerieImage.size() );
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

void DICOMVolume::loadtffFile( std::string tffFileName )
{
    std::ifstream file(tffFileName.c_str(), std::ios::binary);
    if( !file )
    {
        MessageBoxA(NULL, _T("fail to read tff"),_T("error"),MB_OK);
    }
    const int MAX_CNT = 10000;
    unsigned char* tffData = reinterpret_cast< unsigned char* >(calloc( MAX_CNT, sizeof(unsigned char) ));
    file.read( reinterpret_cast<char *>(tffData), MAX_CNT );
    if( file.eof() )
    {
         size_t count =  file.gcount();
         //*( tffData +count ) = '\0';
         //std::cout<<"count: "<< count<<std::endl;
         file.close();
         m_ptffFile = new unsigned char[count];
         //std::ofstream out("exportTff.txt");
         for( int i =0 ;i<256;++i )
         {
             m_ptffFile[4*i] = (int)tffData[4*i];
             m_ptffFile[4*i +1 ] = (int)tffData[4*i+1];
             m_ptffFile[4*i +2 ] = (int)tffData[4*i+2];
             m_ptffFile[4*i +3 ] = (int)tffData[4*i+3];
         }

         tempTffFile = m_ptffFile;
         //out.close();
         delete[] tffData;
    }
}