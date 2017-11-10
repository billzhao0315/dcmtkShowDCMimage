#pragma once

#include "Observer.h"

class CMainFrame;

class DisplayMgr:public Observer
{
public:
    DisplayMgr( CMainFrame* pCMainFrame );

private:
    CMainFrame* m_pCMainFrame;

};