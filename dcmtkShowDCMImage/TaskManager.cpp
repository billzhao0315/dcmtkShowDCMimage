#include "stdafx.h"
#include "TaskManager.h"

TaskManager::TaskManager(USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument)
{
    m_pView = pView;
    m_pDoc = pDocument;
}
