
// dcmtkShowDCMImageDoc.cpp : implementation of the CdcmtkShowDCMImageDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "dcmtkShowDCMImage.h"
#endif

#include "dcmtkShowDCMImageDoc.h"
#include "DICOMImageHelper.h"

#include <propkey.h>
#include <io.h>
#include <winuser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CdcmtkShowDCMImageDoc

const AttributeTag CdcmtkShowDCMImageDoc::tagDICOMImport;

IMPLEMENT_DYNCREATE(CdcmtkShowDCMImageDoc, CDocument)

BEGIN_MESSAGE_MAP(CdcmtkShowDCMImageDoc, CDocument)
    ON_COMMAND(ID_FILE_OPENDICOM, &CdcmtkShowDCMImageDoc::OnFileOpendicom)
END_MESSAGE_MAP()


// CdcmtkShowDCMImageDoc construction/destruction

CdcmtkShowDCMImageDoc::CdcmtkShowDCMImageDoc()
{
	// TODO: add one-time construction code here

}

CdcmtkShowDCMImageDoc::~CdcmtkShowDCMImageDoc()
{
}

BOOL CdcmtkShowDCMImageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CdcmtkShowDCMImageDoc serialization

void CdcmtkShowDCMImageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CdcmtkShowDCMImageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CdcmtkShowDCMImageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CdcmtkShowDCMImageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CdcmtkShowDCMImageDoc diagnostics

#ifdef _DEBUG
void CdcmtkShowDCMImageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CdcmtkShowDCMImageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CdcmtkShowDCMImageDoc commands

void CdcmtkShowDCMImageDoc::openDicoms( std::string pDicomFileIndex)
{
    
    std::string sDicomPath;
    int iPos = pDicomFileIndex.find_last_of("\\");
    sDicomPath = pDicomFileIndex.substr(0,iPos+1);
    iPos = pDicomFileIndex.rfind(".DCM");
    std::string strDicomIndex = sDicomPath +"*"+ pDicomFileIndex.substr(iPos, pDicomFileIndex.length());
    intptr_t hFile = 0;
    _finddata_t fileinfo;
    hFile = _findfirst(strDicomIndex.c_str(), &fileinfo);
    if (hFile == -1)
    {
        MessageBoxA(NULL,_T("fail to load the dicom file"),_T("error"), MB_OK|MB_ICONERROR);
        return;
    }

    do
    {
        m_vDicomFileSet.push_back(sDicomPath + std::string(fileinfo.name));
    } while (_findnext(hFile, &fileinfo) == 0);

    _findclose(hFile);
}

void CdcmtkShowDCMImageDoc::OnFileOpendicom()
{
    // TODO: Add your command handler code here
    CString m_sFile;
    CString szFilter = _T("Text Files (*.dcm)|*.dcm||"); //使打开文件对话框仅显示dcm文件
    CFileDialog  dicomFile(TRUE,
                          (LPCTSTR)".dcm",
                          m_sFile,
                          OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
                          szFilter);
    if( dicomFile.DoModal()==TRUE )
    {
        m_sFile = dicomFile.GetPathName();
        openDicoms( m_sFile.GetString() );
        
        BeginWaitCursor();
        m_pDicomImageHelper =std::make_shared<DICOMImageHelper>();
        m_pDicomImageHelper->DicomParse(m_vDicomFileSet);
        EndWaitCursor();
        UpdateAllViews( NULL, static_cast<LPARAM>( tagDICOMImport.getValue() ) );
    }
}
