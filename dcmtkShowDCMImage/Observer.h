#pragma once
#include "AttributeTag.h"
#include <vector>
class Subject;

class Observer
{
public:
    virtual ~Observer();
    virtual void onSubjectNotify(
                         Subject* pSubject,
                         AttributeTag tag,
                         void* pOldValue = NULL,
                         void* pNewValue = NULL
                        );
protected:
    Observer();
};


class Subject
{
public:
    virtual ~Subject();
    virtual void attachObserver( Observer* pObserver );
    virtual void detachObserver( Observer* pObserver );
    virtual void onNotifyObservers(
                                   AttributeTag tag,
                                   void* pOldValue = NULL,
                                   void* pNewValue = NULL
                                  );
    bool hasObserver();
protected:
    Subject();
private:
    std::vector< Observer* > m_arrObservers;
};