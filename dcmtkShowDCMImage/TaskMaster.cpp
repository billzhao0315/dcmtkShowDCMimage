#include "stdafx.h"
#include "TaskMaster.h"

const AttributeTag TaskMaster::tagTaskActivated;

const AttributeTag TaskMaster::tagTaskDeactivated;

TaskMaster::TaskMaster()
{
    
}

bool TaskMaster::setTask( Tasktype taskType )
{
    TaskMap::iterator itr = m_mapTaskMap.find( taskType );

    if( itr == m_mapTaskMap.end() )
    {
        m_mapTaskMap.insert( std::make_pair( taskType, true ) );

    }
    else
    {
        if( isActiveTask( taskType ) )
        {
            itr->second = false;
            onNotifyObservers( tagTaskDeactivated, NULL, (void*)(&taskType) );
        }
        else
        {
            itr->second = true;
        }
    }

    onNotifyObservers( tagTaskActivated, NULL, (void*)(&taskType) );
    return true;
}

bool TaskMaster::isActiveTask(Tasktype taskType)
{
    auto itr = m_mapTaskMap.find( taskType );

    if( itr == m_mapTaskMap.end() )
    {
        return false;
    }

    return itr->second;
}


