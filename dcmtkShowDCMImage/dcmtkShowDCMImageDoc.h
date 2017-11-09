
// dcmtkShowDCMImageDoc.h : interface of the CdcmtkShowDCMImageDoc class
//


#pragma once
#include <memory>
#include <vector>
#include <string>
#include "AttributeTag.h"
class DICOMImageHelper;
class CdcmtkShowDCMImageDoc : public CDocument
{
protected: // create from serialization only
	CdcmtkShowDCMImageDoc();
	DECLARE_DYNCREATE(CdcmtkShowDCMImageDoc)

    //not system function----begin


public:
    static const AttributeTag tagDICOMImport;
private:
    void openDicoms( std::string pDicomFileIndex );
private:
    std::vector<std::string> m_vDicomFileSet;
    std::shared_ptr<DICOMImageHelper> m_pDicomImageHelper;
    unsigned int m_nSeriesImageIndex;
    //not system function----end








// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CdcmtkShowDCMImageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
    afx_msg void OnFileOpendicom();
};
