



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

};

