#pragma once
#include "afxwin.h"
#include "dcmtkShowDCMImageDoc.h"
#include "dcmtkShowDCMImageView.h"

#include "USSTBaseView.h"

class GLShaderMgr;

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
    GLuint m_nBufferVBO;
    GLuint m_nLineIndexVAO;
    GLuint m_nFrameDataVBO;
    GLuint m_nCoronalViewVAO;
    std::shared_ptr<GLShaderMgr> m_pGLShaderMgr;
    //float  m_modelMatrix[16];
    void drawCube();
    bool initializeData();
protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in dcmtkShowDCMImageView.cpp
inline CdcmtkShowDCMImageDoc* TransverseView::GetDocument() const
   { return reinterpret_cast<CdcmtkShowDCMImageDoc*>(m_pDocument); }
#endif

