
#pragma once
#include "Task.h"
#include "dcmtkShowDCMImageDoc.h"
class USSTBaseView;
class SpvViewRotateImptTask : public Task
{
public:
    SpvViewRotateImptTask( USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument );

    virtual ~SpvViewRotateImptTask();


    // Task starting/stopping/status query

    virtual void start();
    virtual void stop();

    // Keyboard handlers

    virtual bool onKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    virtual bool onKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );

    // Mouse handlers

    virtual bool onMouseLBtnDblClk( UINT nFlags, const CPoint& point );
    virtual bool onMouseLBtnDown( UINT nFlags, const CPoint& point );
    virtual bool onMouseLBtnUp( UINT nFlags, const CPoint& point);

    virtual bool onMouseLeave( WPARAM wParam, LPARAM lParam );

    virtual bool onMouseMBtnDown( UINT nFlags, const CPoint& point );

    virtual bool onMouseMove( UINT nFlags, const CPoint& point );

    virtual bool onMouseRBtnDown( UINT nFlags, const CPoint& point );
    virtual bool onMouseRBtnUp( UINT nFlags, const CPoint& point );

    virtual bool onMouseWheel( UINT nFlags, short zDelta, const CPoint& pt );

    // Update handler

	virtual void onUpdate( CView* pSender, LPARAM lHint, CObject* pHint );


    // Cursor handler

    virtual bool onSetCursor(  CWnd* pWnd, UINT nHitTest, UINT message );


private:

    USSTBaseView* m_pView;
    CdcmtkShowDCMImageDoc* m_pDoc;

};