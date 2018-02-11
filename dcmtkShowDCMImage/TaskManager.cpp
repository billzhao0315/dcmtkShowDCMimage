#include "stdafx.h"
#include "TaskManager.h"

TaskManager::TaskManager(USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument)
{
    m_pView = pView;
    m_pDoc = pDocument;
}

bool TaskManager::onMouseWheel(UINT /*nFlags*/, short /*zDelta*/, const CPoint& /*pt*/)
{
    return false;
}

bool TaskManager::onMouseLBtnDown(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool TaskManager::onMouseMBtnDown(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool TaskManager::onMouseMove(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

void TaskManager::onUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    
}

void TaskManager::onDraw(CDC* /*pDC*/)
{
    
}

bool TaskManager::onKeyDown(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
    return false;
}

bool TaskManager::onKeyUp(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
    return false;
}

bool TaskManager::onMouseLBtnDblClk(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool TaskManager::onMouseLBtnUp(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool TaskManager::onMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool TaskManager::onMouseRBtnDown(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool TaskManager::onMouseRBtnUp(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool TaskManager::onSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
    return false;
}










