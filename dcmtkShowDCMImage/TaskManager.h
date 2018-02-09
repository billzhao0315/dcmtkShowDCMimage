#pragma once
#include <vector>
#include "Observer.h"
#include "dcmtkShowDCMImageDoc.h"

class USSTBaseView;
class Task;
class TaskManager: public Observer
{
public:
    TaskManager( USSTBaseView* pView, CdcmtkShowDCMImageDoc* pDocument );

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