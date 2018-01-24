#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"

#include "USSTBaseView.h"

class CoronalView : public USSTBaseView
{
public:
    DECLARE_DYNCREATE(CoronalView)
    CoronalView(void);
    ~CoronalView(void);
    virtual void OnUpdate( CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/ );
    public:
	virtual void OnDraw(CDC* pDC);

public:
    virtual std::unique_ptr<CMenu> createPopUpMenu();

    void setGLRenderContext( HGLRC  m_hGLrcCoronal );

    void setClientDC( CClientDC* pClientDCCoronal );

    HGLRC getGLRenderContext();


    CdcmtkShowDCMImageDoc* GetDocument() const;
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif
private:
    HGLRC      m_hGLrcCoronal;
    CClientDC* m_pClientDCCoronal;
    GLuint m_n3DTextureID;
    void drawCube();
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in dcmtkShowDCMImageView.cpp
inline CdcmtkShowDCMImageDoc* TransverseView::GetDocument() const
   { return reinterpret_cast<CdcmtkShowDCMImageDoc*>(m_pDocument); }
#endif

