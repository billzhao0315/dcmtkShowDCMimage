#include "stdafx.h"
#include "Observer.h"

Observer::Observer()
{
    
}

Observer::~Observer()
{
    
}

void Observer::onSubjectNotify(Subject* /*pSubject*/, AttributeTag /*tag*/, void* /*pOldValue*/, void* /*pNewValue*/)
{
    
}

Subject::Subject()
{
    
}

Subject::~Subject()
{
    
}

void Subject::attachObserver( Observer* pObserver )
{
    if( pObserver != static_cast<Observer*>(NULL) )
    {
        std::vector<Observer*>::iterator pItr = m_arrObservers.begin();
        std::vector<Observer*>::iterator pItrSlot = m_arrObservers.end();
        while( pItr != m_arrObservers.end() )
        {
            if( (*pItr) == pObserver )
            {
                return;
            }
            else if( (*pItr) == NULL )
            {
                pItr = pItr;
            }

            ++pItr;
        }

        if( pItrSlot == m_arrObservers.end() )
        {
            m_arrObservers.push_back( pObserver );
        }
        else
        {
            (*pItrSlot) = pObserver; 
        }
    }
    
}

void Subject::detachObserver( Observer* pObserver )
{
    if( pObserver != static_cast<Observer*>(NULL) )
    {
        std::vector<Observer*>::iterator pObserverItr = m_arrObservers.begin();

        while( pObserverItr != m_arrObservers.end() )
        {
            // Empty the slot for potential re-use.
            // We empty it rather than delete it to avoid problems that arise when
            // an observer is detached as part of the response to a notification.
            // (The observer array bounds and indices would change, possibly causing
            // observers to be skipped or an attempt to traverse past the end of
            // the array.)
            if( *pObserverItr == pObserver )
            {
                *pObserverItr = NULL;
            }
            ++pObserverItr;
        }
    }
}

bool Subject::hasObserver()
{
    return !m_arrObservers.empty();
}

void Subject::onNotifyObservers( AttributeTag tag,
                                 void* pOldValue,
                                 void* pNewValue )
{
    // We use indices rather than iterators here because recursive notification
    // calls can result in changes to the list of observers while we traverse it
    // (additional observers may be attached).
    // This is also the reason for explicitly calling size() on each iteration,
    // rather than calling once and saving the value.
    for( int i = 0; i < m_arrObservers.size(); ++i )
    {
        if( m_arrObservers[i] != NULL )
        {
            m_arrObservers[i]->onSubjectNotify( this, tag, pOldValue, pNewValue );
        }
    }
}

