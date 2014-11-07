#include "CThread.h"

CThread::CThread() 
{
    m_thread = new pthread_t;
}

CThread::~CThread()
{
    if(m_thread != NULL) 
        delete m_thread;
    m_thread = NULL;
}

void* CThread::ThreadFunction(void* param) 
{
    CThread* t = (CThread*) param;
    t->Run();
    return NULL;    
}

bool CThread::Start(CThread* cthread)
{
    int rc = pthread_create(m_thread, NULL, ThreadFunction, (void*)cthread);
    m_ThreadID = *m_thread;
    printf("CThread::CThread|thread has been created, id is:%lu\n",GetThreadID());
    assert(rc == 0);
    return true;
    
}


