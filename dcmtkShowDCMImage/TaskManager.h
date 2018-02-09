#pragma once

#include "Observer.h"
#include "dcmtkShowDCMImageDoc.h"

class USSTBaseView;

class TaskManager: public Observer
{
public:
    TaskManager( USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument );

private:
    USSTBaseView* m_pView;
    CdcmtkShowDCMImageDoc* m_pDoc;
};