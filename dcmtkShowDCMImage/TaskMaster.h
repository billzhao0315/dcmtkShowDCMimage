#pragma once
#include "Observer.h"
#include "AttributeTag.h"
#include "TaskType.h"
#include <map>

class TaskMaster : public Subject
{
public:
    static const AttributeTag tagTaskActivated;
    static const AttributeTag tagTaskDeactivated;
    TaskMaster();

    bool setTask( Tasktype taskType );
    bool toggleTask();
    bool isActiveTask( Tasktype taskType );
private:
    typedef std::map< Tasktype, bool>  TaskMap;
    TaskMap m_mapTaskMap;


    TaskMaster( const TaskMaster& );
    TaskMaster& operator=( const TaskMaster& ); 
};