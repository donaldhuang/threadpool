#include "CWorkerThread.h"
#include "CThreadPool.h"

CWorkerThread::CWorkerThread()
{
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
}

CWorkerThread::~CWorkerThread()
{
    if(m_Job != NULL)
        delete m_Job;
    if(m_JobData != NULL)
        delete m_JobData;
    
    m_Job = NULL;
    m_JobData = NULL;
    m_ThreadPool = NULL;
}

void CWorkerThread::Run()
{
    printf("CWorkerThread::Run|thread is running\n");
//    SetThreadState(THREAD_RUNNING);//from cthread
    for(;true;usleep(10))
    {
        printf("CWorkerThread::Run|........wait a job.\n");
        while(m_Job == NULL)
            usleep(20);
////            m_JobCond.Wait();//wait until been set a job
        printf("CWorkerThread::Run|........job comes do it...\n");      
        m_Job->Run(m_JobData);
        m_Job->SetWorkThread(NULL);
        m_Job = NULL;
        m_ThreadPool->MoveToIdleList(this);
    }
}

void CWorkerThread::SetJob(CJob* job,void* jobdata)
{
    m_Job = job;
    m_JobData = jobdata;
    m_Job->SetWorkThread(this);
    
}

void CWorkerThread::SetThreadPool(CThreadPool* thrpool)
{
    m_ThreadPool = thrpool;
}