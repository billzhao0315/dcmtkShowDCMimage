
// dcmtkShowDCMImageDoc.h : interface of the CdcmtkShowDCMImageDoc class
//


#pragma once
#include <memory>
#include <vector>
#include <string>
#include "AttributeTag.h"
#include "Observer.h"
#include <gl/GL.h>
class DICOMImageHelper;
class CdcmtkShowDCMImageDoc : public CDocument,public Observer, public Subject
{
protected: // create from serialization only
	CdcmtkShowDCMImageDoc();
	DECLARE_DYNCREATE(CdcmtkShowDCMImageDoc)

    //not system function----begin


public:
    static const AttributeTag tagDICOMImport;
    static const AttributeTag tagDICOMSliceChange;
public:
    std::shared_ptr<DICOMImageHelper> getDicomImageHelper()const;
    unsigned int getCurrentImageIndex()const;
    virtual void onNotifyObservers(
        AttributeTag tag,
        void* pOldValue = NULL,
        void* pNewValue = NULL
        );
    void increaseImageIndex();
    void decreaseImageIndex();

    bool setTexture3DSPV( GLuint n3DTextureID );

    GLuint getTexture3DSPV();

private:
    void openDicoms( std::string pDicomFileIndex );
    void processSpvView(AttributeTag tag,
        void* pOldValue = NULL,
        void* pNewValue = NULL);
private:
    std::vector<std::string> m_vDicomFileSet;
    std::shared_ptr<DICOMImageHelper> m_pDicomImageHelper;
    unsigned int m_nSeriesImageIndex;

    GLuint m_n3DTextureID;

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
