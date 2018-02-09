#pragma once


#include "Observer.h"

class Task : public Subject
{
public:
    Task();
    virtual ~Task();
    
    // Task starting/stopping/status query

    virtual void start() = 0;
    virtual void stop() = 0;

    // Keyboard handlers

    virtual bool onKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) = 0;
    virtual bool onKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags ) = 0;

    // Mouse handlers

    virtual bool onMouseLBtnDblClk( UINT nFlags, const CPoint& point ) = 0;
    virtual bool onMouseLBtnDown( UINT nFlags, const CPoint& point ) = 0;
    virtual bool onMouseLBtnUp( UINT nFlags, const CPoint& point) = 0;

    virtual bool onMouseLeave( WPARAM wParam, LPARAM lParam ) = 0;

    virtual bool onMouseMBtnDown( UINT nFlags, const CPoint& point );

    virtual bool onMouseMove( UINT nFlags, const CPoint& point ) = 0;

    virtual bool onMouseRBtnDown( UINT nFlags, const CPoint& point ) = 0;
    virtual bool onMouseRBtnUp( UINT nFlags, const CPoint& point ) = 0;

    virtual bool onMouseWheel( UINT nFlags, short zDelta, const CPoint& pt );

    // Update handler

	virtual void onUpdate( CView* pSender, LPARAM lHint, CObject* pHint );


    // Cursor handler

    virtual bool onSetCursor(  CWnd* pWnd, UINT nHitTest, UINT message ) = 0;

    // Drawing

    virtual void onDraw( CDC* pDC );



protected:


private:

    // Not implemented:

    Task( const Task& rhs );
    Task& operator=( const Task& rhs );
};
