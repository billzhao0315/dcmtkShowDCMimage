



#pragma once
#include "afxwin.h"
#include <memory>


class USSTBaseView : public CView
{
public:
    USSTBaseView(void);
    ~USSTBaseView(void);

    virtual void OnUpdate( CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/ );
    virtual std::unique_ptr<CMenu> createPopUpMenu();

protected:



    DECLARE_DYNAMIC( USSTBaseView );

public:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};

