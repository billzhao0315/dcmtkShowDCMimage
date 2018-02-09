#include "stdafx.h"
#include "SpvViewRotateImptTask.h"
#include "USSTBaseView.h"

SpvViewRotateImptTask::SpvViewRotateImptTask(USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument)
{
    m_pView = pView;
    m_pDoc = pDocument;
}

SpvViewRotateImptTask::~SpvViewRotateImptTask()
{
    
}

bool SpvViewRotateImptTask::onMouseLBtnDown(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseMove(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

void SpvViewRotateImptTask::onUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
    
}

bool SpvViewRotateImptTask::onKeyDown(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
    return false;
}

bool SpvViewRotateImptTask::onKeyUp(UINT /*nChar*/, UINT /*nRepCnt*/, UINT /*nFlags*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseLBtnDblClk(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseLBtnUp(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseMBtnDown(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseRBtnDown(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseRBtnUp(UINT /*nFlags*/, const CPoint& /*point*/)
{
    return false;
}

bool SpvViewRotateImptTask::onMouseWheel(UINT /*nFlags*/, short /*zDelta*/, const CPoint& /*pt*/)
{
    return false;
}

bool SpvViewRotateImptTask::onSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
    return false;
}

void SpvViewRotateImptTask::start()
{
    
}

void SpvViewRotateImptTask::stop()
{
    
}

