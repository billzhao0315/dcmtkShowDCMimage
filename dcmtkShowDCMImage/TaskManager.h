#pragma once
#include <vector>
#include "Observer.h"
#include "dcmtkShowDCMImageDoc.h"
#include "TaskType.h"

class USSTBaseView;
class Task;
class TaskManager: public Observer
{
public:
    TaskManager( USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument );


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
    virtual bool onSetCursor(  CWnd* pWnd, UINT nHitTest, UINT message );

    // Update handler
    virtual void onUpdate( CView* pSender, LPARAM lHint, CObject* pHint );

    // Drawing
    virtual void onDraw( CDC* pDC );

    virtual void initTasks( const std::vector<Tasktype>& arrEnabledTasks ) = 0;


private:

    struct TaskEntry
    {
        std::string m_strName;
        std::shared_ptr<Task> m_pTaskInstance;
    };

    std::vector< TaskEntry > m_arrTasks;

    USSTBaseView* m_pView;
    CdcmtkShowDCMImageDoc* m_pDoc;
};